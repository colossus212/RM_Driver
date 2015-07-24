cmake_minimum_required(VERSION 3.1)
project(RoboMaster_Driver C CXX)
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_VERBOSE_MAKEFILE off)

set(tools /usr/local/gcc-arm-none-eabi)
set(CROSS_COMPILER_PREFIX arm-none-eabi-)

include (CMakeForceCompiler)
cmake_force_c_compiler(${tools}/bin/${CROSS_COMPILER_PREFIX}gcc GNU)
cmake_force_cxx_compiler(${tools}/bin/${CROSS_COMPILER_PREFIX}g++ GNU)

set(SOURCE_FILES
    include/mylib/ad.h
    include/mylib/beep.h
    include/mylib/can1.h
    include/mylib/delay.h
    include/mylib/encoder.h
    include/mylib/flash.h
    include/mylib/key.h
    include/mylib/led.h
    include/mylib/maincontrol.h
    include/mylib/motor.h
    include/mylib/ticker.h
    include/mylib/tim2.h
    include/mylib/usart2.h
    include/user/debug.h
    include/user/main.h
    system/include/cmsis/stm32f10x_conf.h
    include/user/stm32f10x_it.h
    include/user/util.h
    src/mylib/ad.c
    src/mylib/beep.c
    src/mylib/can1.c
    src/mylib/delay.c
    src/mylib/encoder.c
    src/mylib/flash.c
    src/mylib/key.c
    src/mylib/led.c
    src/mylib/maincontrol.c
    src/mylib/motor.c
    src/mylib/ticker.c
    src/mylib/tim2.c
    src/mylib/usart2.c
    src/user/debug.c
    src/user/main.c
    src/user/util.c
    system/include/arm/semihosting.h
    system/include/cmsis/arm_common_tables.h
    system/include/cmsis/arm_const_structs.h
    system/include/cmsis/arm_math.h
    system/include/cmsis/cmsis_device.h
    system/include/cmsis/core_cm3.h
    system/include/cmsis/core_cmFunc.h
    system/include/cmsis/core_cmInstr.h
    system/include/cmsis/stm32f10x.h
    system/include/cmsis/system_stm32f10x.h
    system/include/cortexm/ExceptionHandlers.h
    system/include/diag/Trace.h
    system/include/stm32f1-stdperiph/misc.h
    system/include/stm32f1-stdperiph/stm32f10x_adc.h
    system/include/stm32f1-stdperiph/stm32f10x_bkp.h
    system/include/stm32f1-stdperiph/stm32f10x_can.h
    system/include/stm32f1-stdperiph/stm32f10x_cec.h
    system/include/stm32f1-stdperiph/stm32f10x_crc.h
    system/include/stm32f1-stdperiph/stm32f10x_dac.h
    system/include/stm32f1-stdperiph/stm32f10x_dbgmcu.h
    system/include/stm32f1-stdperiph/stm32f10x_dma.h
    system/include/stm32f1-stdperiph/stm32f10x_exti.h
    system/include/stm32f1-stdperiph/stm32f10x_flash.h
    system/include/stm32f1-stdperiph/stm32f10x_fsmc.h
    system/include/stm32f1-stdperiph/stm32f10x_gpio.h
    system/include/stm32f1-stdperiph/stm32f10x_i2c.h
    system/include/stm32f1-stdperiph/stm32f10x_iwdg.h
    system/include/stm32f1-stdperiph/stm32f10x_pwr.h
    system/include/stm32f1-stdperiph/stm32f10x_rcc.h
    system/include/stm32f1-stdperiph/stm32f10x_rtc.h
    system/include/stm32f1-stdperiph/stm32f10x_sdio.h
    system/include/stm32f1-stdperiph/stm32f10x_spi.h
    system/include/stm32f1-stdperiph/stm32f10x_tim.h
    system/include/stm32f1-stdperiph/stm32f10x_usart.h
    system/include/stm32f1-stdperiph/stm32f10x_wwdg.h
    system/src/cmsis/stack_heap_stm32f10x.c
    system/src/cmsis/system_stm32f10x.c
    system/src/cmsis/vectors_stm32f10x.c
    system/src/cortexm/_initialize_hardware.c
    system/src/cortexm/_reset_hardware.c
    system/src/cortexm/exception_handlers.c
    system/src/diag/Trace.c
    system/src/diag/trace_impl.c
    system/src/newlib/_cxx.cpp
    system/src/newlib/_exit.c
    system/src/newlib/_sbrk.c
    system/src/newlib/_startup.c
    system/src/newlib/_syscalls.c
    system/src/newlib/assert.c
    system/src/stm32f1-stdperiph/misc.c
    system/src/stm32f1-stdperiph/stm32f10x_adc.c
    system/src/stm32f1-stdperiph/stm32f10x_bkp.c
    system/src/stm32f1-stdperiph/stm32f10x_can.c
    system/src/stm32f1-stdperiph/stm32f10x_cec.c
    system/src/stm32f1-stdperiph/stm32f10x_crc.c
    system/src/stm32f1-stdperiph/stm32f10x_dac.c
    system/src/stm32f1-stdperiph/stm32f10x_dbgmcu.c
    system/src/stm32f1-stdperiph/stm32f10x_dma.c
    system/src/stm32f1-stdperiph/stm32f10x_exti.c
    system/src/stm32f1-stdperiph/stm32f10x_flash.c
    system/src/stm32f1-stdperiph/stm32f10x_fsmc.c
    system/src/stm32f1-stdperiph/stm32f10x_gpio.c
    system/src/stm32f1-stdperiph/stm32f10x_i2c.c
    system/src/stm32f1-stdperiph/stm32f10x_iwdg.c
    system/src/stm32f1-stdperiph/stm32f10x_pwr.c
    system/src/stm32f1-stdperiph/stm32f10x_rcc.c
    system/src/stm32f1-stdperiph/stm32f10x_rtc.c
    system/src/stm32f1-stdperiph/stm32f10x_sdio.c
    system/src/stm32f1-stdperiph/stm32f10x_spi.c
    system/src/stm32f1-stdperiph/stm32f10x_tim.c
    system/src/stm32f1-stdperiph/stm32f10x_usart.c
    system/src/stm32f1-stdperiph/stm32f10x_wwdg.c
    include/mylib/can_packet.h
    include/mylib/can_packet_queue.h
    src/mylib/pid.c
    include/mylib/pid.h)


include_directories(include)
include_directories(include/mylib)
include_directories(include/user)
include_directories(system/include)
include_directories(system/include/cmsis)
include_directories(system/include/stm32f1-stdperiph)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS
    HSE_VALUE=8000000
    STM32F10X_MD
    USE_STDPERIPH_DRIVER
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:TRACE>
    $<$<CONFIG:Debug>:OS_USE_TRACE_SEMIHOSTING_DEBUG>
)

set(OPT_LEVEL "s")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -mthumb -O${OPT_LEVEL} -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-m3 -mthumb -O${OPT_LEVEL} -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -std=gnu++11 -fabi-version=0")

set(LINKER_SCRIPTS_DIR ${PROJECT_SOURCE_DIR}/Eclipse/ldscripts)
set(MEM_LINKER_SCRIPT ${LINKER_SCRIPTS_DIR}/mem.ld)
set(SECTIONS_LINKER_SCRIPT ${LINKER_SCRIPTS_DIR}/sections.ld)

set(BUILD_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_PATH}")


set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-T ${MEM_LINKER_SCRIPT} -T ${SECTIONS_LINKER_SCRIPT} -nostartfiles -Xlinker --gc-sections -Wl,-Map,\"${BUILD_OUTPUT_PATH}/${PROJECT_NAME}.map\" --specs=nano.specs")

add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${tools}/bin/${CROSS_COMPILER_PREFIX}objcopy -O ihex \"${BUILD_OUTPUT_PATH}/${PROJECT_NAME}.elf\" \"${BUILD_OUTPUT_PATH}/${PROJECT_NAME}.hex\")
add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${tools}/bin/${CROSS_COMPILER_PREFIX}size --format=berkeley \"${BUILD_OUTPUT_PATH}/${PROJECT_NAME}.elf\")
