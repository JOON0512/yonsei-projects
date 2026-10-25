#include "mandelbrot.h"
#include "complex.h"
#include <cuda_runtime.h>
#include <cstdio>

// simple CUDA error check
#define CUDA_CHECK(err) \
    if ((err) != cudaSuccess) { \
        fprintf(stderr, "CUDA Error: %s (line %d)\n", cudaGetErrorString(err), __LINE__); \
        exit(EXIT_FAILURE); \
    }

// kernel: compute Mandelbrot iteration count per pixel
__global__
void mandelbrotKernel(unsigned *out) {
    // calculate thread x, y
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= resolution || y >= resolution) return;

    // map pixel to complex plane
    float real = minX + x * (maxX - minX) / (resolution - 1);
    float imag = minY + y * (maxY - minY) / (resolution - 1);

    Complex<float> c(real, imag);
    Complex<float> z(0.0f, 0.0f);

    unsigned iter = 0;
    // iterate until escape or max count
    while (z.magnitude2() <= escape && iter < max_iterations) {
        z = z * z + c;
        ++iter;
    }

    // store result in output array
    out[x + y * resolution] = iter;
}

// host function: alloc → launch → copy → free
void calc_mandelbrot(void) {
    size_t bytes = resolution * resolution * sizeof(unsigned);
    unsigned *d_mandelbrot = nullptr;

    // 1) allocate device memory
    CUDA_CHECK(cudaMalloc(&d_mandelbrot, bytes));

    // 2) set up block and grid sizes
    dim3 block(16, 16);
    dim3 grid(
        (resolution + block.x - 1) / block.x,
        (resolution + block.y - 1) / block.y
    );

    // 3) launch the kernel
    mandelbrotKernel<<<grid, block>>>(d_mandelbrot);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    // 4) copy results back to host
    CUDA_CHECK(cudaMemcpy(
        mandelbrot,           // global array in main.cc
        d_mandelbrot,
        bytes,
        cudaMemcpyDeviceToHost
    ));

    // 5) free device memory
    CUDA_CHECK(cudaFree(d_mandelbrot));
}
