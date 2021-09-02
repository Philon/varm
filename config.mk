PROJECT_ROOT = $(shell cd `dirname $(CONFIG)` && pwd)

PLATFORM = vexpress-a9
ARCH=arm

OS := $(shell uname)
ifeq ($(OS), Darwin)
	JOBS ?= $(shell sysctl -n machdep.cpu.thread_count)
	MAKE = /opt/homebrew/bin/gmake
	CROSS_COMPILE=/opt/arm-mac-linux-gnueabihf/bin/arm-mac-linux-gnueabihf-
	HOSTCFLAGS="-I/opt/homebrew/opt/openssl/include"
	HOSTLDFLAGS="-L/opt/homebrew/opt/openssl/lib"
endif
ifeq ($(OS), Linux)
	JOBS ?= $(shell grep -c ^process /proc/cpuinfo)
endif
JOBS ?= 1

ifdef O
  ifeq ("$(origin O)", "command line")
    ODIR := $(O)
  endif
else
	ODIR := $(PROJECT_ROOT)/build
endif

export ARCH CROSS_COMPILE