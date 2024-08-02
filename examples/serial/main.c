//
// SPDX-FileCopyrightText: Copyright 2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
// SPDX-License-Identifier: BSD-3-Clause
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <pico/stdlib.h>

#include "rnnoise.h"

int main()
{
  stdio_init_all();

  while (!stdio_usb_connected()) {
    tight_loop_contents();
  }

  #define FRAME_SIZE 480
  int16_t x_i16[FRAME_SIZE];
  float x_f32[FRAME_SIZE];
  uint8_t* x_i8 = (uint8_t*)x_i16;

  DenoiseState* st = rnnoise_create(NULL);

  // run once to init
  memset(x_f32, 0x00, sizeof(x_f32));
  rnnoise_process_frame(st, x_f32, x_f32);

  while (1) {
    for (size_t i = 0; i < sizeof(x_i16); i++) {
      x_i8[i] = getchar();
    }

    for (int i = 0; i < FRAME_SIZE; i++) {
      x_f32[i] = x_i16[i];
    }

    float vad = rnnoise_process_frame(st, x_f32, x_f32);

    for (int i = 0; i < FRAME_SIZE; i++) {
      x_i16[i] = x_f32[i];
    }

    for (int i = 0; i < sizeof(x_i16); i++) {
      putchar_raw(x_i8[i]);
    }
  }

  return 0;
}
