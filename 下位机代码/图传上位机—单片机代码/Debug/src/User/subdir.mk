################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/User/Image_Process.c \
../src/User/LQ_AnoScope.c \
../src/User/LQ_ImageProcess.c \
../src/User/LQ_Inductor.c \
../src/User/LQ_MotorServo.c \
../src/User/LQ_PID.c \
../src/User/LQ_STEER_PID.c \
../src/User/TFT_Key_control.c 

OBJS += \
./src/User/Image_Process.o \
./src/User/LQ_AnoScope.o \
./src/User/LQ_ImageProcess.o \
./src/User/LQ_Inductor.o \
./src/User/LQ_MotorServo.o \
./src/User/LQ_PID.o \
./src/User/LQ_STEER_PID.o \
./src/User/TFT_Key_control.o 

COMPILED_SRCS += \
./src/User/Image_Process.src \
./src/User/LQ_AnoScope.src \
./src/User/LQ_ImageProcess.src \
./src/User/LQ_Inductor.src \
./src/User/LQ_MotorServo.src \
./src/User/LQ_PID.src \
./src/User/LQ_STEER_PID.src \
./src/User/TFT_Key_control.src 

C_DEPS += \
./src/User/Image_Process.d \
./src/User/LQ_AnoScope.d \
./src/User/LQ_ImageProcess.d \
./src/User/LQ_Inductor.d \
./src/User/LQ_MotorServo.d \
./src/User/LQ_PID.d \
./src/User/LQ_STEER_PID.d \
./src/User/TFT_Key_control.d 


# Each subdirectory must supply rules for building sources it contributes
src/User/%.src: ../src/User/%.c src/User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fF:/×ÀÃæ/LQ_TC264DA_LIB230112-longqiu -6.21/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

src/User/%.o: ./src/User/%.src src/User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


