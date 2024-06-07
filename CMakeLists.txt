set(PROJECT gantry)
cmake_minimum_required(VERSION 3.12)

# pull in SDK (very first thing)
set(PICO_SDK_FETCH_FROM_GIT ON)
set(PICO_BOARD pico)
include(pico_sdk_import.cmake)

# setup project (after pulling SDK, before initialising SDK and before pulling the display library)
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(${PROJECT}
	main.c)

#target_include_directories(${PROJECT} PRIVATE
#	${CMAKE_CURRENT_LIST_DIR}
#)

target_link_libraries(${PROJECT}
	pico_stdlib
	pico_time
	hardware_gpio
)

pico_enable_stdio_usb(${PROJECT} 1)
# pico_enable_stdio_uart(${PROJECT} 1)

pico_add_extra_outputs(${PROJECT})
