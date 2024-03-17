################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Driver/LQ_ADC.c \
../src/Driver/LQ_CCU6.c \
../src/Driver/LQ_DMA.c \
../src/Driver/LQ_EEPROM.c \
../src/Driver/LQ_EMEM.c \
../src/Driver/LQ_FFT.c \
../src/Driver/LQ_GPIO.c \
../src/Driver/LQ_GPSR.c \
../src/Driver/LQ_GPT12_ENC.c \
../src/Driver/LQ_GTM.c \
../src/Driver/LQ_QSPI.c \
../src/Driver/LQ_SOFTI2C.c \
../src/Driver/LQ_SPI.c \
../src/Driver/LQ_STM.c \
../src/Driver/LQ_UART.c 

OBJS += \
./src/Driver/LQ_ADC.o \
./src/Driver/LQ_CCU6.o \
./src/Driver/LQ_DMA.o \
./src/Driver/LQ_EEPROM.o \
./src/Driver/LQ_EMEM.o \
./src/Driver/LQ_FFT.o \
./src/Driver/LQ_GPIO.o \
./src/Driver/LQ_GPSR.o \
./src/Driver/LQ_GPT12_ENC.o \
./src/Driver/LQ_GTM.o \
./src/Driver/LQ_QSPI.o \
./src/Driver/LQ_SOFTI2C.o \
./src/Driver/LQ_SPI.o \
./src/Driver/LQ_STM.o \
./src/Driver/LQ_UART.o 

COMPILED_SRCS += \
./src/Driver/LQ_ADC.src \
./src/Driver/LQ_CCU6.src \
./src/Driver/LQ_DMA.src \
./src/Driver/LQ_EEPROM.src \
./src/Driver/LQ_EMEM.src \
./src/Driver/LQ_FFT.src \
./src/Driver/LQ_GPIO.src \
./src/Driver/LQ_GPSR.src \
./src/Driver/LQ_GPT12_ENC.src \
./src/Driver/LQ_GTM.src \
./src/Driver/LQ_QSPI.src \
./src/Driver/LQ_SOFTI2C.src \
./src/Driver/LQ_SPI.src \
./src/Driver/LQ_STM.src \
./src/Driver/LQ_UART.src 

C_DEPS += \
./src/Driver/LQ_ADC.d \
./src/Driver/LQ_CCU6.d \
./src/Driver/LQ_DMA.d \
./src/Driver/LQ_EEPROM.d \
./src/Driver/LQ_EMEM.d \
./src/Driver/LQ_FFT.d \
./src/Driver/LQ_GPIO.d \
./src/Driver/LQ_GPSR.d \
./src/Driver/LQ_GPT12_ENC.d \
./src/Driver/LQ_GTM.d \
./src/Driver/LQ_QSPI.d \
./src/Driver/LQ_SOFTI2C.d \
./src/Driver/LQ_SPI.d \
./src/Driver/LQ_STM.d \
./src/Driver/LQ_UART.d 


# Each subdirectory must supply rules for building sources it contributes
src/Driver/%.src: ../src/Driver/%.c src/Driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fF:/×ÀÃæ/LQ_TC264DA_LIB230112-longqiu -6.21/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

src/Driver/%.o: ./src/Driver/%.src src/Driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


