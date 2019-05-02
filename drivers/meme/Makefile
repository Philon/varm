obj-m += meme.o
meme-objs := main.o meme_proc.o meme_seq_file.o

KDIR = /opt/varm/linux-5.0.7
MODULE = $(notdir $(shell pwd))
CROSS = arm-linux-gnueabihf-
DEFINE = ARCH=arm CROSS_COMPILE=$(CROSS)
CC = $(CROSS)gcc

# CFLAGS += -I$(shell pwd)

all: module test

module:
	$(MAKE) $(DEFINE) -C $(KDIR) M=`pwd` modules

test: $(MODULE)_test.c
	$(CC) $(CFLAGS) $(LFLAGS) -o $(MODULE)_test $^

clean:
	$(MAKE) $(DEFINE) -C $(KDIR) M=`pwd` clean
	rm -f $(MODULE)_test