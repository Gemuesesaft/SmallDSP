################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/IC_Drivers/Src/AK4619.c 

OBJS += \
./Drivers/IC_Drivers/Src/AK4619.o 

C_DEPS += \
./Drivers/IC_Drivers/Src/AK4619.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/IC_Drivers/Src/%.o Drivers/IC_Drivers/Src/%.su Drivers/IC_Drivers/Src/%.cyclo: ../Drivers/IC_Drivers/Src/%.c Drivers/IC_Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"/Users/till/Documents/Projects/Personal/SmallDSP/Firmware/SmallDSPMaxi_MCUFirmware/Drivers/IC_Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-IC_Drivers-2f-Src

clean-Drivers-2f-IC_Drivers-2f-Src:
	-$(RM) ./Drivers/IC_Drivers/Src/AK4619.cyclo ./Drivers/IC_Drivers/Src/AK4619.d ./Drivers/IC_Drivers/Src/AK4619.o ./Drivers/IC_Drivers/Src/AK4619.su

.PHONY: clean-Drivers-2f-IC_Drivers-2f-Src

