################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/iLLD/TC26B/Tricore/Emem/Std/IfxEmem.c 

OBJS += \
./Libraries/iLLD/TC26B/Tricore/Emem/Std/IfxEmem.o 

COMPILED_SRCS += \
./Libraries/iLLD/TC26B/Tricore/Emem/Std/IfxEmem.src 

C_DEPS += \
./Libraries/iLLD/TC26B/Tricore/Emem/Std/IfxEmem.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/iLLD/TC26B/Tricore/Emem/Std/%.src: ../Libraries/iLLD/TC26B/Tricore/Emem/Std/%.c Libraries/iLLD/TC26B/Tricore/Emem/Std/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fF:/����/LQ_TC264DA_LIB230112-longqiu -6.21/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Libraries/iLLD/TC26B/Tricore/Emem/Std/%.o: ./Libraries/iLLD/TC26B/Tricore/Emem/Std/%.src Libraries/iLLD/TC26B/Tricore/Emem/Std/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


