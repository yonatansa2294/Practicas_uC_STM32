################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f411ceux.s 

OBJS += \
./Startup/startup_stm32f411ceux.o 

S_DEPS += \
./Startup/startup_stm32f411ceux.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/jonatan/FIUBA/uC/Practicas/P5_1_API_UART/Drivers/API/Inc" -I"/home/jonatan/FIUBA/uC/Practicas/P5_1_API_UART/Drivers/API/Src" -I"/home/jonatan/FIUBA/uC/Practicas/P5_1_API_UART/Drivers/BSP/STM32F4xx_BlackPill" -I"/home/jonatan/FIUBA/uC/Practicas/P5_1_API_UART/Drivers/Core/Inc" -I"/home/jonatan/FIUBA/uC/Practicas/P5_1_API_UART/Drivers/CMSIS/Include" -I"/home/jonatan/FIUBA/uC/Practicas/P5_1_API_UART/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/jonatan/FIUBA/uC/Practicas/P5_1_API_UART/Drivers/STM32F4xx_HAL_Driver/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32f411ceux.d ./Startup/startup_stm32f411ceux.o

.PHONY: clean-Startup

