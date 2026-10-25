#ifndef __DFT_H__
#define __DFT_H__

#include <complex>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <mpi.h>
#include "data.h"

// Perform 2-D discrete Fourier transform (DFT).
template <typename T>
void dft2d(T *data, const unsigned width, const unsigned height,
           const int num_ranks, const int rank_id) {

    // --- 0) Preconditions: width == height and power of two check ---
    if (width == 0 || width != height || (width & (width - 1)) != 0) {
        if (rank_id == 0) {
            std::cerr << "Error: width and height must be equal and power of two.\n";
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // --- 1) Compute local row distribution for load balancing ---
    unsigned base_rows = height / num_ranks;
    unsigned rem       = height % num_ranks;
    unsigned my_rows   = base_rows + (static_cast<unsigned>(rank_id) < rem ? 1 : 0);
    unsigned row_start = rank_id * base_rows + std::min<unsigned>(static_cast<unsigned>(rank_id), rem);

    // --- 2) Precompute bit-reversal indices and twiddle factors for 1-D FFT ---
    //    Compute number of bits such that 2^bits == width
    unsigned bits = 0;
    while ((1u << bits) < width) {
        ++bits;
    }

    //    Build bit-reversal lookup table of size 'width'
    std::vector<unsigned> rev(width);
    for (unsigned i = 0; i < width; ++i) {
        unsigned x = i;
        unsigned y = 0;
        for (unsigned j = 0; j < bits; ++j) {
            y = (y << 1) | (x & 1u);
            x >>= 1;
        }
        rev[i] = y;
    }

    //    Precompute twiddle factors: W[k] = exp(-2πi * k / width) for k in [0..width/2-1]
    std::vector<T> W(width / 2);
    const double PI = 3.14159265358979323846;
    for (unsigned k = 0; k < width / 2; ++k) {
        double angle = -2.0 * PI * static_cast<double>(k) / static_cast<double>(width);
        W[k] = T(std::cos(angle), std::sin(angle));
    }

    // --- 3) Define 1-D in-place FFT as a lambda function ---
    auto fft1d = [&](T *row_ptr) {
        //    Allocate a temporary array for in-place FFT computations
        std::vector<T> x(width);
        //    Step 1: Copy input into bit-reversed order
        for (unsigned i = 0; i < width; ++i) {
            x[i] = row_ptr[rev[i]];
        }
        //    Step 2: Iterative Cooley-Tukey butterfly stages
        for (unsigned len = 2; len <= width; len <<= 1) {
            unsigned half = len >> 1;                // half-length of current sub-DFT
            unsigned stride = width / len;           // twiddle factor stride
            for (unsigned start = 0; start < width; start += len) {
                for (unsigned j = 0; j < half; ++j) {
                    unsigned idx1 = start + j;
                    unsigned idx2 = idx1 + half;
                    T u = x[idx1];
                    T v = W[j * stride] * x[idx2];
                    x[idx1] = u + v;
                    x[idx2] = u - v;
                }
            }
        }
        //    Step 3: Copy transformed data back to the row
        std::memcpy(row_ptr, x.data(), width * sizeof(T));
    };

    // --- 4) Define in-place transpose for square matrix ---
    auto transpose = [&](T *mat) {
        for (unsigned r = 0; r < height; ++r) {
            for (unsigned c = r + 1; c < width; ++c) {
                std::swap(mat[r * width + c], mat[c * width + r]);
            }
        }
    };

    // --- 5) Define custom All-gather-like row synchronization ---
    auto custom_allgather = [&](T *matrix) {
        // Prepare non-blocking receive requests for each rank's rows
        std::vector<MPI_Request> reqs(num_ranks);
        for (int src = 0; src < num_ranks; ++src) {
            if (src == rank_id) { reqs[src] = MPI_REQUEST_NULL; continue; }
            unsigned src_rows = base_rows + (static_cast<unsigned>(src) < rem ? 1 : 0);
            unsigned src_start = src * base_rows + std::min<unsigned>(static_cast<unsigned>(src), rem);
            // Receive into matrix + (src_start * width), count = src_rows * width * sizeof(T) bytes
            MPI_Irecv(
                reinterpret_cast<char*>(matrix + src_start * width),
                static_cast<int>(src_rows * width * sizeof(T)),
                MPI_BYTE,
                src,
                0,
                MPI_COMM_WORLD,
                &reqs[src]
            );
        }
        // Send this rank's transformed rows to all ranks (including itself)
        for (int dst = 0; dst < num_ranks; ++dst) {
            if (dst == rank_id) continue;
            MPI_Send(
                reinterpret_cast<char*>(matrix + row_start * width),
                static_cast<int>(my_rows * width * sizeof(T)),
                MPI_BYTE,
                dst,
                0,
                MPI_COMM_WORLD
            );
        }
        // Wait for all non-blocking receives to complete
        MPI_Waitall(num_ranks, reqs.data(), MPI_STATUSES_IGNORE);
    };

    // --- 6) First pass: Perform row-wise 1-D FFT on local rows ---
    for (unsigned r = 0; r < my_rows; ++r) {
        fft1d(data + (row_start + r) * width);
    }
    //    Synchronize all rows across ranks
    custom_allgather(data);

    // --- 7) Transpose matrix to prepare for column-wise FFT ---
    transpose(data);

    //    Recompute row_start and my_rows for the transposed layout;
    //    since the matrix is square, distribution is identical
    //    (row_start, my_rows, base_rows, rem remain valid for 'height' dimension)

    // --- 8) Second pass: Perform row-wise 1-D FFT on transposed rows (original columns) ---
    for (unsigned r = 0; r < my_rows; ++r) {
        fft1d(data + (row_start + r) * width);
    }
    //    Synchronize transformed columns (still in transposed layout)
    custom_allgather(data);

    // --- 9) Transpose matrix back to original orientation ---
    transpose(data);

    // All ranks now have the complete 2-D DFT result in 'data'
}

#endif
