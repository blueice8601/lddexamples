ifeq ($(KERNELRELEASE),)
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *.ko *~ core .depend .*.cmd *.mod.c .tmp_versions modules.order Module.symvers

.PHONY: modules modules_install clean

else
	obj-m := input_driver.o
endif
