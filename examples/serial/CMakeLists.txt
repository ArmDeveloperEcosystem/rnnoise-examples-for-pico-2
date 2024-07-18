cmake_minimum_required(VERSION 3.13...3.27)

add_executable(rnnoise-serial
    main.c
)

target_compile_definitions(rnnoise-serial PRIVATE
    -DPICO_STACK_SIZE=0xc000
)

target_link_libraries(rnnoise-serial
    pico_stdlib
    rnnoise
)

pico_enable_stdio_usb(rnnoise-serial 1)
pico_enable_stdio_uart(rnnoise-serial 0)

pico_add_extra_outputs(rnnoise-serial)
