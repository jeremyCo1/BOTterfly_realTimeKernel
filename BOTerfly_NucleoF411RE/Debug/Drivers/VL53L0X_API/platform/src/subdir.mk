################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L0X_API/platform/src/vl53l0x_platform.c 

OBJS += \
./Drivers/VL53L0X_API/platform/src/vl53l0x_platform.o 

C_DEPS += \
./Drivers/VL53L0X_API/platform/src/vl53l0x_platform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L0X_API/platform/src/%.o: ../Drivers/VL53L0X_API/platform/src/%.c Drivers/VL53L0X_API/platform/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/jerem/STM32CubeIDE/workspace_1.7.0/BOTerfly_NucleoF411RE/Drivers/VL53L0X_API/core/inc" -I"C:/Users/jerem/STM32CubeIDE/workspace_1.7.0/BOTerfly_NucleoF411RE/Drivers/VL53L0X_API/platform/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

