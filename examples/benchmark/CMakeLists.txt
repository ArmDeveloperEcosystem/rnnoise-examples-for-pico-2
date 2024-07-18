cmake_minimum_required(VERSION 3.13...3.27)

add_executable(rnnoise-benchmark
    main.c
)

target_compile_definitions(rnnoise-benchmark PRIVATE
    -DPICO_STACK_SIZE=0xc000
)

target_link_libraries(rnnoise-benchmark
    pico_stdlib
    rnnoise
)

pico_enable_stdio_usb(rnnoise-benchmark 1)
pico_enable_stdio_uart(rnnoise-benchmark 0)

pico_add_extra_outputs(rnnoise-benchmark)
