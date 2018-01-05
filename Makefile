obj-m = get_random.o
KDIR = $(shell uname -r)
PWD = $(shell pwd)

clock:calculate_clock.o
all:
	make -C /lib/modules/$(KDIR)/build M=$(PWD) modules
clock:
	gcc calculate_clock.c -o clock
.PHONE:clean
clean:	
	make -C /lib/modules/$(KDIR)/build M=$(PWD) clean
	
