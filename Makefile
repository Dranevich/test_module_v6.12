MODULE_NAME := test_module
obj-m += $(MODULE_NAME).o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

# default parameters, might be redefined later 
LOG_NAME ?= test_log
PERIOD   ?= 5
LOG_PATH := /var/tmp/$(LOG_NAME)

# .PHONY is used to separate targets from files
.PHONY: all clean load unload

# Targets:
# 1. default (creates test_module.ko)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# 2. cleans the project (i.e. delete all .o, .ko, .mod and other temporary files)
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

# 3. refreshes the module and enrolls it to the kernel with parameters given
load: all
	sudo insmod $(MODULE_NAME).ko log_path=$(LOG_PATH) period_sec=$(PERIOD)

# removes module from the kernel & frees resources
unload:
	sudo rmmod $(MODULE_NAME)


