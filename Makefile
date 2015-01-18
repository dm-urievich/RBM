NAME=rbm
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main



LIB_DIR=-I./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x -I./Libraries/CMSIS/CM3/CoreSupport -I.

# Selecting Core
CORTEX_M=3

# Use newlib-nano. To disable it, specify USE_NANO=
USE_NANO=--specs=nano.specs

# Use seimhosting or not
USE_SEMIHOST=--specs=rdimon.specs
USE_NOHOST=--specs=nosys.specs

TOOLCHAIN=/home/dima/arm_toolchain/gcc-arm-none-eabi-4_8-2014q3

# Compiler & Linker
CC=$(TOOLCHAIN)/bin/arm-none-eabi-gcc
CXX=$(TOOLCHAIN)/bin/arm-none-eabi-g++

# Options for specific architecture
ARCH_FLAGS=-mthumb -mcpu=cortex-m$(CORTEX_M)

# Startup code
STARTUP=startup_stm32f10x_md.s

# -Os -flto -ffunction-sections -fdata-sections to compile for code size
CFLAGS=$(ARCH_FLAGS) $(STARTUP_DEFS) -Os -flto -ffunction-sections -fdata-sections -std=c99
CXXFLAGS=$(CFLAGS)

# Link for code size
GC=-Wl,--gc-sections

# Create map file
MAP=-Wl,-Map=$(NAME).map

LDSCRIPTS=-L. -T stm32f103c8.ld
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP) $(LIB_DIR)

$(NAME).axf: src/main.c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@
	$(TOOLCHAIN)/bin/arm-none-eabi-objcopy -v -O binary $(NAME).axf $(NAME).bin

clean: 
	rm -f $(NAME)*.axf *.map *.o
