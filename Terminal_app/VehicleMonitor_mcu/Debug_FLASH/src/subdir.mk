################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/clock_init.c \
../src/lpuart1.c \
../src/main.c \
../src/peripheral_init.c 

OBJS += \
./src/clock_init.o \
./src/lpuart1.o \
./src/main.o \
./src/peripheral_init.o 

C_DEPS += \
./src/clock_init.d \
./src/lpuart1.d \
./src/main.d \
./src/peripheral_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/clock_init.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


