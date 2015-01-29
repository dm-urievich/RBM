NAME=rbm
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main



INC_DIR := -I./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x -I./Libraries/CMSIS/CM3/CoreSupport -I. -I./Libraries/STM32F10x_StdPeriph_Driver/inc
BUILD_DIR := ./build
SRC_DIR := ./src ./Libraries/CMSIS/CM3/CoreSupport ./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
STDPERIPHLIB := ./Libraries/libstdperiph.a
STDPERIPHLIB_DIR := ./Libraries/STM32F10x_StdPeriph_Driver/src
STDPERIPHLIB_SRC := $(wildcard $(STDPERIPHLIB_DIR)/*.c)
STDPERIPHLIB_OBJ := $(STDPERIPHLIB_SRC:.c=.o)
#STDPERIPHLIB_OBJ := $(notdir $(STDPERIPHLIB_OBJ))
#STDPERIPHLIB_OBJ := $(addprefix $(BUILD_DIR)/, $(STDPERIPHLIB_OBJ))
search_wildcard := $(addsuffix /*.c,$(SRC_DIR))
SRC := $(wildcard $(search_wildcard))

OBJS := $(SRC:.c=.o)

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
AR=$(TOOLCHAIN)/bin/arm-none-eabi-ar
OBJCOPY=$(TOOLCHAIN)/bin/arm-none-eabi-objcopy

# Options for specific architecture
ARCH_FLAGS=-mthumb -mcpu=cortex-m$(CORTEX_M)

# Startup code
STARTUP=startup_stm32f10x_md.s

# -Os -flto -ffunction-sections -fdata-sections to compile for code size
CFLAGS=$(ARCH_FLAGS) $(STARTUP_DEFS) -DSTM32F10X_MD -Os -flto -ffunction-sections -fdata-sections -std=c99 -g
CXXFLAGS=$(CFLAGS)

# Link for code size
GC=-Wl,--gc-sections

# Create map file
MAP=-Wl,-Map=$(NAME).map

LDSCRIPTS=-L. -T stm32f103c8.ld
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP) $(INC_DIR)

all: $(BUILD_DIR)/$(NAME).axf

$(BUILD_DIR)/$(NAME).axf: $(OBJS) $(STARTUP)
	$(CC) $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS))) $(STARTUP) $(CFLAGS) $(LFLAGS) $(STDPERIPHLIB) -o $@
	$(OBJCOPY) -v -O binary $(BUILD_DIR)/$(NAME).axf $(NAME).bin

clean: 
	rm -f $(NAME)*.axf *.map *.o
	rm -f $(BUILD_DIR)/*

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)	

deploy:
	st-flash write $(NAME).bin 0x08000000

test:
	@echo $(STDPERIPHLIB_OBJ)

%.o: %.c
	$(CC) -o $(BUILD_DIR)/$(notdir $@) -c $< $(CFLAGS) $(INC_DIR)

stdperiphlib: $(STDPERIPHLIB_OBJ)
	$(AR) crs $(STDPERIPHLIB) $(addprefix $(BUILD_DIR)/, $(notdir $(STDPERIPHLIB_OBJ)))


