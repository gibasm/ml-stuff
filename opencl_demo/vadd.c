#include <CL/cl.h>
#include <inttypes.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

extern char _binary_vadd_spv_start[];
extern char _binary_vadd_spv_end[];

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        puts("Usage: vadd <number of vector elements to add> <platform number>");
        return -1;
    }

    srand(time(NULL));
    int status = CL_SUCCESS;

    uint32_t cnt = atoi(argv[1]);
    cl_uint p_num = atoi(argv[2]);

    float a[cnt], b[cnt], c[cnt];
    memset(c, 0, cnt);

    for (uint32_t i = 0; i < cnt; ++i)
    {
        a[i] = 1.0 + (rand() / (float)(RAND_MAX));
        b[i] = 1.0 + (rand() / (float)(RAND_MAX));
    }

    cl_uint n_plat;
    status = clGetPlatformIDs(0, NULL, &n_plat);
    assert(status == CL_SUCCESS);
    assert(n_plat != 0);
    assert(p_num < n_plat);

    cl_platform_id platforms[n_plat];
    status = clGetPlatformIDs(n_plat, platforms, NULL);
    assert(status     == CL_SUCCESS);

    cl_device_id dev_id;
    status = clGetDeviceIDs(platforms[p_num], CL_DEVICE_TYPE_DEFAULT, 1, &dev_id, NULL);
    assert(status     == CL_SUCCESS);

    cl_context context = clCreateContext(NULL, 1, &dev_id, NULL, NULL, &status);
    assert(status     == CL_SUCCESS);

    cl_command_queue cq = clCreateCommandQueueWithProperties(context, dev_id, NULL, &status);
    assert(status     == CL_SUCCESS);

    size_t p_sz = (size_t)(_binary_vadd_spv_end - _binary_vadd_spv_start);

    // create and build program
    cl_program program = clCreateProgramWithIL(context, (const void*) _binary_vadd_spv_start,
           p_sz, &status);
    assert(status     == CL_SUCCESS);
    status = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    assert(status     == CL_SUCCESS);

    // create kernel
    cl_kernel ko_vadd = clCreateKernel(program, "vadd", &status);
    assert(status     == CL_SUCCESS);

    // create buffers
    cl_mem vec_a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * cnt, NULL, &status);
    assert(status     == CL_SUCCESS);
    cl_mem vec_b = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * cnt, NULL, &status);
    assert(status     == CL_SUCCESS);
    cl_mem vec_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * cnt, NULL, &status);
    assert(status     == CL_SUCCESS);

    // fill the buffers with randomly generated vectors
    status = clEnqueueWriteBuffer(cq, vec_a, CL_TRUE, 0, sizeof(float) * cnt, a, 0, NULL, NULL);
    assert(status     == CL_SUCCESS);
    status = clEnqueueWriteBuffer(cq, vec_b, CL_TRUE, 0, sizeof(float) * cnt, b, 0, NULL, NULL);
    assert(status     == CL_SUCCESS);

    status  = clSetKernelArg(ko_vadd, 0, sizeof(cl_mem), &vec_a);
    status |= clSetKernelArg(ko_vadd, 1, sizeof(cl_mem), &vec_b);
    status |= clSetKernelArg(ko_vadd, 2, sizeof(cl_mem), &vec_c);
    status |= clSetKernelArg(ko_vadd, 3, sizeof(uint32_t), &cnt);
    assert(status     == CL_SUCCESS);

    for(size_t i = 0; i < 100; ++i) {
        size_t global = cnt;
        status = clEnqueueNDRangeKernel(cq, ko_vadd, 1, NULL, &global, NULL, 0, NULL, NULL);
        assert(status     == CL_SUCCESS);

        status = clFinish(cq);
        assert(status     == CL_SUCCESS);

        status = clEnqueueReadBuffer(cq, vec_c, CL_TRUE, 0, sizeof(float) * cnt, c, 0, NULL, NULL);
        assert(status     == CL_SUCCESS);

        status = clFinish(cq);
        assert(status     == CL_SUCCESS);
    }


    printf("A = (");
    for (uint32_t i = 0; i < cnt; ++i)
        printf("%f, ", a[i]);
    puts("\b\b)");

    printf("B = (");
    for (uint32_t i = 0; i < cnt; ++i)
        printf("%f, ", b[i]);
    puts("\b\b)");

    printf("A + B = (");
    for (uint32_t i = 0; i < cnt; ++i)
        printf("%f, ", c[i]);
    puts("\b\b)");

    clReleaseMemObject(vec_a);
    clReleaseMemObject(vec_b);
    clReleaseMemObject(vec_c);
    clReleaseProgram(program);
    clReleaseKernel( ko_vadd);
    clReleaseCommandQueue(cq);
    clReleaseContext(context);

    return 0;
}

