NAME := CGPIODriver

obj-m += $(NAME).o

VERSION := $(shell uname -r)
LOCAL := $(shell pwd)

all: driver lib testapp


driver:
	make -C /lib/modules/$(VERSION)/build M=$(LOCAL) modules

lib:
	gcc -fPIC -c -o lib$(NAME).o lib$(NAME).c
	gcc -shared -Wl,-soname,lib$(NAME).so.1 -o lib$(NAME).so.1.0.0 lib$(NAME).o -lc
	ln -s lib$(NAME).so.1.0.0 lib$(NAME).so
	ln -s lib$(NAME).so.1.0.0 lib$(NAME).so.1

testapp:
	gcc -o $(NAME)Test $(NAME)Test.c -L. -l$(NAME) -Wl,-rpath=.

appclean:
	rm -rf $(NAME)Test

libclean:
	rm -f lib$(NAME).o
	rm -f lib$(NAME).so*

driverclean:
	make -C /lib/modules/$(VERSION)/build M=$(LOCAL) clean

clean: driverclean libclean appclean
	
	

	