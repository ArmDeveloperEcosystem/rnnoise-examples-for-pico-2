cmake_minimum_required(VERSION 3.13...3.27)

add_executable(rnnoise_usb_pdm_microphone
    main.c
    ${CMAKE_CURRENT_LIST_DIR}/../../lib/microphone-library-for-pico/examples/usb_microphone/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/../../lib/microphone-library-for-pico/examples/usb_microphone/usb_microphone.c
)

target_include_directories(rnnoise_usb_pdm_microphone PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../lib/microphone-library-for-pico/examples/usb_microphone
)

target_link_libraries(rnnoise_usb_pdm_microphone PRIVATE
    hardware_pwm
    pico_stdlib
    pico_multicore
    tinyusb_device
    tinyusb_board
    pico_pdm_microphone
    rnnoise
)

add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/../../lib/microphone-library-for-pico
    microphone-library-for-pico
    EXCLUDE_FROM_ALL
)

pico_enable_stdio_usb(rnnoise_usb_pdm_microphone 0)
pico_enable_stdio_uart(rnnoise_usb_pdm_microphone 1)

pico_add_extra_outputs(rnnoise_usb_pdm_microphone)
