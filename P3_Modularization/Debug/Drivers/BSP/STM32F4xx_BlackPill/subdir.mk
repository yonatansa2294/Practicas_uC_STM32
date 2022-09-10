################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32F4xx_BlackPill/smt32f4xx_blackpill.c 

OBJS += \
./Drivers/BSP/STM32F4xx_BlackPill/smt32f4xx_blackpill.o 

C_DEPS += \
./Drivers/BSP/STM32F4xx_BlackPill/smt32f4xx_blackpill.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F4xx_BlackPill/%.o Drivers/BSP/STM32F4xx_BlackPill/%.su: ../Drivers/BSP/STM32F4xx_BlackPill/%.c Drivers/BSP/STM32F4xx_BlackPill/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Inc -I"/home/jonatan/FIUBA/uC/Practicas/P3_Modularization/Drivers/CMSIS/Include" -I"/home/jonatan/FIUBA/uC/Practicas/P3_Modularization/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/jonatan/FIUBA/uC/Practicas/P3_Modularization/Drivers/BSP/STM32F4xx_BlackPill" -I"/home/jonatan/FIUBA/uC/Practicas/P3_Modularization/Drivers/Core/Inc" -I"/home/jonatan/FIUBA/uC/Practicas/P3_Modularization/Drivers/API/Inc" -I"/home/jonatan/FIUBA/uC/Practicas/P3_Modularization/Drivers/API/Src" -I"/home/jonatan/FIUBA/uC/Practicas/P3_Modularization/Drivers/STM32F4xx_HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32F4xx_BlackPill

clean-Drivers-2f-BSP-2f-STM32F4xx_BlackPill:
	-$(RM) ./Drivers/BSP/STM32F4xx_BlackPill/smt32f4xx_blackpill.d ./Drivers/BSP/STM32F4xx_BlackPill/smt32f4xx_blackpill.o ./Drivers/BSP/STM32F4xx_BlackPill/smt32f4xx_blackpill.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32F4xx_BlackPill

