cmake_minimum_required(VERSION 3.6)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# Setup project, output and linker file
project(Project LANGUAGES C CXX ASM)

set(CMAKE_BUILD_TYPE "Debug")
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/Libraries/Device/ST/STM32L1xx/Source/Templates/stm32.ld)

#set(COMPILER_CLASSB_FLASH_END_ADDRES 0x08060000) #384k
#set(COMPILER_CLASSB_FLASH_END_ADDRES 0x08040000) #256k
set(COMPILER_CLASSB_FLASH_END_ADDRES 0x08020000) #128k
#set(COMPILER_CLASSB_FLASH_END_ADDRES 0x0800FBF0) #64k
#set(COMPILER_CLASSB_FLASH_END_ADDRES 0x08007BF0) #32k
#set(COMPILER_CLASSB_FLASH_END_ADDRES 0x08003BF0) #16k

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

file(GLOB_RECURSE USER_SOURCES  
        Libraries/Device/ST/STM32L1xx/Source/Templates/gcc_ride7/startup_stm32l1xx_hd.s
        Middlewares/cmsis/CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S
        src/*.cpp
        src/*.c
        Libraries/STM32L1xx_StdPeriph_Driver/src/*.c
        Middlewares/cmsis/CMSIS/RTOS2/RTX/Source/*.c
        Middlewares/cmsis/CMSIS/RTOS2/Source/os_systick.c
        Middlewares/cmsis/CMSIS/RTOS2/RTX/Config/RTX_Config.c
        Middlewares/cmsis-cpp/src/OS.cpp
        Middlewares/cmsis-cpp/src/Thread.cpp
        Middlewares/cmsis-cpp/src/Threads.cpp
        Middlewares/cmsis-cpp/src/ThreadFlag.cpp
        Middlewares/cmsis-cpp/src/OSException.cpp
        Middlewares/cmsis-cpp/src/Semaphore.cpp
        Middlewares/cmsis-cpp/src/Timer.cpp
        Middlewares/cmsis-cpp/src/Mutex.cpp
        Middlewares/cmsis-cpp/src/Memory.cpp
        Middlewares/cmsis-cpp/src/EventFlag.cpp
        Middlewares/cmsis-cpp/src/ConditionVariable.cpp
        Middlewares/cmsis-cpp/src/Chrono.cpp
        #Middlewares/cmsis-cpp/src/MessageQueue.cpp
)
        
# Build the executable based on the source files
add_executable(${EXECUTABLE} ${USER_SOURCES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        -D ARM_MATH_CM3=true
        -D USE_STDPERIPH_DRIVER=1
        -D STM32L1XX_HD
        -D RTE_CMSIS_RTOS2_RTX5
        )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
        inc
        Libraries/STM32L1xx_StdPeriph_Driver/inc
        Libraries/Device/ST/STM32L1xx/Include
        Libraries/Device/ST/STM32L1xx/Source/Templates/gcc_ride7
        Libraries/STM32L1xx_StdPeriph_Driver/inc
        Middlewares/cmsis/CMSIS/RTOS2/RTX/Include
        Middlewares/cmsis/CMSIS/RTOS2/RTX/Include1
        Middlewares/cmsis/CMSIS/RTOS2/RTX/Config
        Middlewares/cmsis/CMSIS/RTOS2/RTX/Source
        Middlewares/cmsis/CMSIS/RTOS2/Include        
        Middlewares/cmsis/Device/ARM/ARMCM3/Include
        Middlewares/cmsis/CMSIS/Core/Include
        Middlewares/cmsis-dsp/Include
        Middlewares/cmsis-cpp/include
        )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m3
        -mthumb
        -O3
        -g3 -MD -MP -MQ 
        -Wpointer-arith 
        -ffunction-sections -fdata-sections         
        #-std=c++14
        )                

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        -T${LINKER_FILE}
        -mcpu=cortex-m3
        -mthumb
        #-mfpu=none
        #-mfloat-abi=none
        -specs=nosys.specs
        -Wl,--gc-sections
        -Xlinker -print-memory-usage
        )

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE}
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
        COMMAND ../Output/srec_cat.exe ${PROJECT_NAME}.s19 -crop 0x08000000 ${COMPILER_CLASSB_FLASH_END_ADDRES} 
        -fill 0xFF 0x08000000 ${COMPILER_CLASSB_FLASH_END_ADDRES} -crc16-l-e ${COMPILER_CLASSB_FLASH_END_ADDRES}  
        -o ../Output/Project.hex -intel -address-length=4)
