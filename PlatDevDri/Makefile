obj-m := PlatDev.o PlatDriver.o 

all:
	 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	 make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

