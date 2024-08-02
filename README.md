# RNNoise Examples for Pico 2

Example project to demonstrate the [RNNoise](https://github.com/xiph/rnnoise) audio noise suppression algorithm running on a Raspberry Pi Pico 2 board, using the RP2350's Cortex-M33 processor.


## Hardware

 * RP2350 board
   * Raspberry Pi Pico 2
 * Microphones
   * PDM
     * [Adafruit PDM MEMS Microphone Breakout](https://www.adafruit.com/product/3492)

### Default Pinout


#### PDM Microphone

| Raspberry Pi Pico 2 / RP2350 | PDM Microphone |
| ---------------------------- | ----------------- |
| 3.3V | VCC |
| GND | GND |
| GND | SEL |
| GPIO 21 | DAT |
| GPIO 20 | CLK |

GPIO pins are configurable in example.

## Examples

See [examples](examples/) folder.


## Cloning

```sh
git clone https://github.com/ArmDeveloperEcosystem/rnnoise-examples-for-pico-2.git
```

## Building

1. [Set up the Pico C/C++ SDK](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)
2. Set `PICO_SDK_PATH`
```sh
export PICO_SDK_PATH=/path/to/pico-sdk
```
3. Create `build` dir, run `cmake` and `make`:
```
mkdir build
cd build
cmake .. -DPICO_BOARD=pico2
make
```
4. Copy example `.uf2` to Pico when in BOOT mode.

## License

[BSD-3-Clause license](LICENSE)


---

Disclaimer: This is not an official Arm product.