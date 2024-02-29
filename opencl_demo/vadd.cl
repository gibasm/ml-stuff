__kernel void vadd(__global float* a, __global float* b,
                   __global float* c, const unsigned int cnt)
{
    int i = get_global_id(0);
    if (i < cnt) {
        c[i] = a[i] + b[i];
    }
}
