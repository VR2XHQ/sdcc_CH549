CC := sdcc
output := verimake-ch549
tmpdir := build

# Flashing tool directory
toolsdir := ./tools
_wchisp := wchisp-linux-x64/wchisp
_WCHISPTool := WCH-linux-x64/WCHISPTool_CMD
_ch37x := $(toolsdir)/WCH-linux-x64/ch37x.ko

RM := @rm -f
VPATH = usr:source:example

C_INCLUDES := -Iinc -Isource

# Uncomment the active main program here
#MAIN := demo_oled_spi
MAIN := demo_pwm_led
#MAIN := demo_timer_led
#MAIN := demo_uart_123
#MAIN := USBHostHUB_KM
#MAIN := demo_blink_key
#MAIN := demo_timer_uart

C_SOURCES := \
$(MAIN).c \
CH549_debug.c \
CH549_gpio.c \
CH549_pwm.c \
# Add and move below c library up here if needed for main program \
CH549_timer.c \
CH549_oled.c \
CH549_spi.c \
CH549_adc.c \
CH549_uart123.c \
CH549_usbhost.c\
CH549_compare.c \
CH549_touchkey.c \
CH549_ledcontrol.c \

CFLAGS := $(C_INCLUDES)

# Set output target
all : $(tmpdir)/$(output).ihx $(tmpdir)/$(output).hex $(tmpdir)/$(output).bin

# Get source name, change suffix to .rel, add path to go.
OBJECTS = $(addprefix $(tmpdir)/,$(notdir $(C_SOURCES:.c=.rel)))

# Set searching path, get source directory data, remove the same by sort function.
# Comment below line if use of VPATH to declare searching path.
# vpath %.c $(sort $(dir $(C_SOURCES)))

# $(tmpdir)/%.d: %.c
#	$(CC) -c -M $(CFLAGS) $< > %.d

$(tmpdir)/%.rel: %.c
	@$(CC) --stack-auto -c $(CFLAGS) $< -o $@

$(tmpdir)/$(output).ihx: $(OBJECTS)
	@$(CC) --stack-auto $^ -o $@

$(tmpdir)/$(output).hex: $(tmpdir)/$(output).ihx
	@packihx $< > $@
	@unix2dos $@

$(tmpdir)/$(output).bin: $(tmpdir)/$(output).ihx
	@objcopy -I ihex -O binary $< $@

$(tmpdir):
	mkdir $@

# Remove the previous built files
clean :
	$(RM) $(tmpdir)/*.rel
	$(RM) $(tmpdir)/*.rst
	$(RM) $(tmpdir)/*.sym
	$(RM) $(tmpdir)/*.mem
	$(RM) $(tmpdir)/*.map
	$(RM) $(tmpdir)/*.lst
	$(RM) $(tmpdir)/*.lk
	$(RM) $(tmpdir)/*.ihx
	$(RM) $(tmpdir)/*.asm
	$(RM) $(tmpdir)/*.hex
	$(RM) $(tmpdir)/*.bin

# Add usb module for WCH offical flashing tool
enable_ch37x :
	sudo insmod $(_ch37x)

# Remove usb module if using wchisp tool in RUST
disable_ch37x :
	sudo rmmod ch37x

# Use WCH offical flashing tool
WCH_flash :
	sudo $(toolsdir)/$(_WCHISPTool) -p /dev/ch37x0 -v boot -c $(toolsdir)/WCH-linux-x64/CH549.INI -o program -f $(tmpdir)/$(output).hex

# Use wchisp as flashing tool 
wchisp_flash :
	$(toolsdir)/$(_wchisp) flash $(tmpdir)/$(output).hex

# target option
.PHONY : all clean enable_ch37x disable_ch37x WCH_flash wchisp_flash
# Usage : make 'target option'
# all = make everything
# clean = clean the build directory
# enable_ch37x / disable_ch37x = driver for WCHISPtool_CMD
# WCH_flash = flash firmware with WCH offical isp tool (sudo required)
# wchisp_flash = flash firmware with wchisp tool in RUST
