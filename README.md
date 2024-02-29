# ml-stuff

## Prerequisites

- hardware with OpenCL support (a GPU for example)
- OpenCL headers, appropriate device drivers etc.
- gcc compiler
- clang
- llvm-spirv
- GNU Make

## Directories

To build programs from each subdirectory, use ``make -C <directory name>``


``tools/``

After running make, run ``./tools/opencl_check`` to check supported platofrms and devices.


``opencl_demo/``

This demo generates 2 vectors of random values and adds them together on target device.
Usage: ``./opencl_demo/vadd <number of vector elements> <platform number*>``

\* platform number - number obtained from ``./tools/opencl_check``


