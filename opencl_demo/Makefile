LLVM_SPIRV=llvm-spirv
LLVM_CC=clang
LLVM_CFLAGS=-c -target spir64 -O0 -emit-llvm

CC=gcc
CFLAGS=-Wall -Wextra -pedantic --std=c2x -D CL_TARGET_OPENCL_VERSION=300
LDFLAGS=-lOpenCL
LD=ld

.DEFAULT_GOAL: all

all: vadd

.PHONY+=vadd.bc
vadd.bc:
	$(LLVM_CC) $(LLVM_CFLAGS) -o vadd.bc vadd.cl

.PHONY+=vadd.spv
vadd.spv: vadd.bc
	$(LLVM_SPIRV) vadd.bc

.PHONY+=vadd.o
vadd.o:
	$(CC) $(CFLAGS) -c vadd.c

.PHONY+=vadd.spv.o
vadd.spv.o: vadd.spv
	$(LD) -r -b binary -o vadd.spv.o vadd.spv

.PHONY+=vadd
vadd: vadd.o vadd.spv.o
	$(CC) -o vadd vadd.o vadd.spv.o $(LDFLAGS)

clean:
	rm *.bc *.o *.spv || true
	make
