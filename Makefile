NAME := CGPIODriver

obj-m += $(NAME).o

VERSION := $(shell uname -r)
LOCAL := $(shell pwd)

CXX := gcc
CFLAGS := -fPIC -c -o
LDFLAGS := -shared -Wl,-soname,lib$(NAME).so.1
LIBS := -lc

all: driver lib testapp

driver:
	make -C /lib/modules/$(VERSION)/build M=$(LOCAL) modules

lib:
	$(CXX) $(CFLAGS) lib$(NAME).o lib$(NAME).c
	$(CXX) $(LDFLAGS) -o lib$(NAME).so.1.0.0 lib$(NAME).o $(LIBS)
	ln -sf lib$(NAME).so.1.0.0 lib$(NAME).so
	ln -sf lib$(NAME).so.1.0.0 lib$(NAME).so.1

testapp:
	$(CXX) -o $(NAME)Test $(NAME)Test.c -L. -l$(NAME) -Wl,-rpath=.

clean:
	rm -f $(NAME).ko $(NAME).o lib$(NAME).o lib$(NAME).so* $(NAME)Test
