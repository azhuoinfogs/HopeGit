obj-m = get_random.o
KDIR = $(shell uname -r)
PWD = $(shell pwd)

all:
	make -C /lib/modules/$(KDIR)/build M=$(PWD) modules
.PHONE:clean
clean:	
	make -C /lib/modules/$(KDIR)/build M=$(PWD) clean
	
