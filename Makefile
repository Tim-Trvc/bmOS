APP ?= blinky
BOARD ?= stm32f446

CXX = arm-none-eabi-g++
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CXXFLAGS = -mcpu=cortex-m4 -mthumb
CXXFLAGS += -std=c++23
CXXFLAGS += -ffreestanding
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-use-cxa-atexit
CXXFLAGS += -Wall -Wextra -O0 -g
CXXFLAGS += -I kernel/inc -I arch/arm-coretx-m/inc -I drivers/stm32/inc

LDFLAGS = -T ldscripts/$(BOARD).ld -nostdlib

C_SRCS = arch/arm/cortex-m/srcs/startup.c
CPP_SRCS = apps/$(APP)/main.cpp

C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))
CPP_OBJS = $(patsubst %.cpp, build/%.o, $(CPP_SRCS))
OBJS = $(C_OBJS) $(CPP_OBJS)

all: build/$(APP).bin

build/$(APP).bin: build/$(APP).elf
  $(OBJCOPY) -O binary $< $@

build/($APP).elf: $(OBJS)
  $(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

build/%.o: %.cpp
  @mkdir -p $(dir $@)
  $(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: %.c
  @mkdir -p $(dir $@)
  $(CC) $(CXXFLAGS) -c $< -o %@

flash: all
  st-flash write build/$(APP).bin 0x08000000

clean:
  rm -rf build/*

