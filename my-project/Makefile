PROJECT = rau_mcu
BUILD_DIR = bin

SHARED_DIR = ../my-common-code
CFILES = rau_mcu_main.c
CFILES += usart_put.c
CFILES += f3_i2c_master_test.c
CFILES += api.c
AFILES += api-asm.S

# TODO - you will need to edit these two lines!
# DEVICE=stm32f407vgt6
DEVICE=stm32f030c8t6

#OOCD_FILE = board/stm32f4discovery.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

