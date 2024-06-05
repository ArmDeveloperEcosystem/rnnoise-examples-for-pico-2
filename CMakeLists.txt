cmake_minimum_required(VERSION 3.13...3.27)

include(pico_sdk_import.cmake)

project(rnn-noise-examples-for-pico-2)

pico_sdk_init()

add_library(rnnoise INTERFACE)

target_sources(rnnoise INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/src/celt_lpc.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/src/denoise.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/src/kiss_fft.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/src/pitch.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/src/rnn.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/src/rnn_data.c
)

target_include_directories(rnnoise INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/src
    ${CMAKE_CURRENT_LIST_DIR}/lib/rnnoise/include
)

add_subdirectory("examples/benchmark")
add_subdirectory("examples/serial")
add_subdirectory("examples/usb_pdm_microphone")
