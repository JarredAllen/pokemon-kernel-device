obj-m += pokemon.o
SHELL=/bin/bash

build: pokemon.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

load: pokemon.c
	insmod pokemon.ko
	mknod /dev/pokemon c $$(sed -n "/pokemon/p; /pokemon/q" /proc/devices | grep -o '[0-9]*') 0
	echo "pokemon" >> /etc/modules
	cp pokemon.ko /lib/modules/`uname -r`/kernel/drivers/char

install:
	make build
	sudo make load

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

uninstall:
	sudo rmmod pokemon
	sudo rm /dev/pokemon
	sudo rm /lib/modules/`uname -r`/kernel/drivers/char/pokemon.ko
	sudo sed -i "/^pokemon$$/d" /etc/modules

