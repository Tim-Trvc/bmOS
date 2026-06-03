APP ?= blinky
BOARD ?= stm32f446

CXX = arm-none-eabi-g++
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

COMMON_FLAGS = -mcpu=cortex-m4 -mthumb -ffreestanding -Wall -Wextra -O0 -g
COMMON_FLAGS += -I kernel/inc -I arch/arm-cortex-m/inc -I drivers/stm32/inc

CFLAGS = $(COMMON_FLAGS)
CXXFLAGS = $(COMMON_FLAGS) -std=c++23 -fno-exceptions -fno-rtti -fno-use-cxa-atexit

LDFLAGS = -T ldscripts/$(BOARD).ld -nostdlib

C_SRCS = arch/arm-cortex-m/src/startup.c
CPP_SRCS = apps/$(APP)/main.cpp

C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))
CPP_OBJS = $(patsubst %.cpp, build/%.o, $(CPP_SRCS))
OBJS = $(C_OBJS) $(CPP_OBJS)

all: build/$(APP).bin

build/$(APP).bin: build/$(APP).elf
	$(OBJCOPY) -O binary $< $@

build/$(APP).elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

flash: all
	st-flash --reset write build/$(APP).bin 0x08000000

clean:
	rm -rf build/*
