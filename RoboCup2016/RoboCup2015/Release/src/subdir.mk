################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BrainAndMotion.cpp \
../src/DetectedGate.cpp \
../src/Vision.cpp \
../src/findGate.cpp \
../src/main.cpp 

OBJS += \
./src/BrainAndMotion.o \
./src/DetectedGate.o \
./src/Vision.o \
./src/findGate.o \
./src/main.o 

CPP_DEPS += \
./src/BrainAndMotion.d \
./src/DetectedGate.d \
./src/Vision.d \
./src/findGate.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


