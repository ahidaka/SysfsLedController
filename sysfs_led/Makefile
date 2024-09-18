#
ifeq ($(KDIR),)
KDIR=/lib/modules/`uname -r`/build
endif
# need KDIR external definition like below...
#KDIR = /home/train/imx-yocto-bsp/maaxboard-8ulp/build/tmp/work/maaxboard_8ulp-poky-linux/linux-imx/6.1.22+gitAUTOINC+78ce688d5a-r0/build/

TARGETS = sysfs_led_test.ko sysfs_led_lib.ko

obj-m += sysfs_led_test.o
obj-m += sysfs_led_lib.o

all: $(TARGETS)

sysfs_led_test.ko: sysfs_led.h
	$(MAKE) -C $(KDIR) M=$(PWD) modules

sysfs_led_lib.ko: sysfs_led.h
	$(MAKE) -C $(KDIR) M=$(PWD) modules

#
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *.o $(TARGETS)
