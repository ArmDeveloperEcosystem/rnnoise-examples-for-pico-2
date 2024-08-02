//
// SPDX-FileCopyrightText: Copyright 2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
// SPDX-License-Identifier: BSD-3-Clause
//

#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>

#include "pico/pdm_microphone.h"

#include "usb_microphone.h"

#include "rnnoise.h"

const int noise_suppression_enabled_pin = 18;

// configuration
const struct pdm_microphone_config pdm_config = {
  .gpio_data = 21,
  .gpio_clk = 20,
  .pio = pio0,
  .pio_sm = 0,
  .sample_rate = SAMPLE_RATE,
  .sample_buffer_size = 480,
};

// variables
int16_t denoise_buffer[480];
volatile bool new_samples = false;

uint16_t sample_buffer[480 * 2];


volatile bool reset_indexes = false;
int in_index = 0;
int out_index = 0;
absolute_time_t last_tx = 0;

// callback functions
void on_pdm_samples_ready();
void on_usb_microphone_tx_ready();

uint32_t core1_stack[0xc000 / sizeof(uint32_t)];
void core1_entry();


int main(void)
{
  stdio_init_all();

  // initialized GPIO pin for switch
  gpio_init(noise_suppression_enabled_pin);
  gpio_pull_up(noise_suppression_enabled_pin);
  gpio_set_dir(noise_suppression_enabled_pin, GPIO_IN);

  // initialize the USB microphone interface
  usb_microphone_init();
  usb_microphone_set_tx_ready_handler(on_usb_microphone_tx_ready);

  multicore_launch_core1_with_stack(core1_entry, core1_stack, sizeof(core1_stack));

  while (1) {
    // run the USB microphone task continuously
    usb_microphone_task();
  }

  return 0;
}

void core1_entry()
{
  gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
  pwm_config pwn_cfg = pwm_get_default_config();
  pwm_config_set_clkdiv(&pwn_cfg, 4.0f);
  pwm_init(slice_num, &pwn_cfg, true);

  float x_f32[480];
  DenoiseState* st = rnnoise_create(NULL);

  // run once to init
  memset(x_f32, 0x00, sizeof(x_f32));
  rnnoise_process_frame(st, x_f32, x_f32);

  // initialize and start the PDM microphone
  pdm_microphone_init(&pdm_config);
  pdm_microphone_set_samples_ready_handler(on_pdm_samples_ready);
  pdm_microphone_set_filter_gain(16);
  pdm_microphone_start();

  while (1) {
    while(!new_samples) {
      tight_loop_contents();
    }

    new_samples = false;

    if (reset_indexes) {
      out_index = 0;
      in_index = 480;
      reset_indexes = false;
    }
    
    float vad = 0.0;
    float* f32 = x_f32;
    int16_t* i16 = denoise_buffer;

    // copy new 16-bit samples to 32-bit floating point buffer
    for (int i = 0; i < 480; i++) {
      *f32++ = *i16++;
    }

    if (gpio_get(noise_suppression_enabled_pin)) {
      vad = rnnoise_process_frame(st, x_f32, x_f32);
    } else {
      // noise supression disabled
    }

    i16 = &sample_buffer[in_index];
    f32 = x_f32;

    // copy processed 32-bit floating point buffer to 16-bit with gain
    for (int i = 0; i < 480; i++) {
      *i16++ = *f32++;
    }

    in_index = (in_index + 480) % (480 * 2);

    pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, (vad * 0xffff));
  }
}

void on_pdm_samples_ready()
{
  pdm_microphone_read(denoise_buffer, 480);

  new_samples = true;
}

void on_usb_microphone_tx_ready()
{
  absolute_time_t now = get_absolute_time();
  if ((now - last_tx) > 2000) {
    reset_indexes = true;
  }
  last_tx = now;

  usb_microphone_write(&sample_buffer[out_index], 16 * 2);
  out_index = (out_index + 16) % (480 * 2);
}
