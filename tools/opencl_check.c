#include <CL/cl.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

int main(void)
{
    cl_int status = CL_SUCCESS;
    cl_uint num_platforms = 0;
    status = clGetPlatformIDs(0, NULL, &num_platforms);
    assert(status == CL_SUCCESS);

    if (num_platforms == 0)
    {
        fprintf(stderr, "No OpenCL platforms detected");
        return -ENXIO;
    }

    cl_platform_id platforms[num_platforms];
    status = clGetPlatformIDs(num_platforms, platforms, NULL);
    assert(status == CL_SUCCESS);

    for (cl_uint i = 0; i < num_platforms; ++i)
    {
        cl_char str[1024];
        status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(str), &str, NULL);
        assert(status == CL_SUCCESS);
        printf("%d# %s\n", i, str);

        status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(str), &str, NULL);
        assert(status == CL_SUCCESS);
        printf("\tVENDOR=%s\n", str);

        status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(str), &str, NULL);
        assert(status == CL_SUCCESS);
        printf("\tVERSION=%s\n", str);

        cl_uint num_devs;
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devs);
        assert(status == CL_SUCCESS);
        printf("\tNumber of devices: %u\n", num_devs);

        cl_device_id devices[num_devs];
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devs, devices, NULL);
        assert(status == CL_SUCCESS);

        for (cl_uint j = 0; j < num_devs; ++j)
        {
            status = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(str), &str, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t%d# %s\n", i, str);

            status = clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, sizeof(str), &str, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t\tOPENCL_C_VERSION=%s\n", str);

            cl_uint num_cunits;
            status = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num_cunits, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t\tMAX_COMPUTE_UNITS=%u\n", num_cunits);

            cl_ulong mem_size;
            status = clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t\tLOCAL_MEM_SIZE=%lu KB\n", mem_size);

            status = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t\tGLOBAL_MEM_SIZE=%lu MB\n", mem_size / (1024*1024));

            status = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &mem_size, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t\tMAX_MEM_ALLOC_SIZE=%lu MB\n", mem_size / (1024*1204));

            size_t wg_sz;
            status = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &wg_sz, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t\tMAX_WORK_GROUP_SIZE=%lu\n", wg_sz);

            cl_uint num_dims;
            status = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &num_dims, NULL);
            assert(status == CL_SUCCESS);

            size_t dims[num_dims];
            status = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(dims), &dims, NULL);
            assert(status == CL_SUCCESS);
            printf("\t\t\tMAX_WORK_ITEM_SIZE=(");
            for (cl_uint k = 0; k < num_dims; ++k)
                printf("%lu, ", dims[k]);
            puts("\b\b)");
        }
    }


    return 0;
}
