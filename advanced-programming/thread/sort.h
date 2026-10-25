#ifndef __SORT_H__
#define __SORT_H__

#include <algorithm>   // std::copy
#include <cstddef>     // std::size_t
#include <cstring>     // std::memcpy
#include <memory>      // std::unique_ptr
#include <thread>      // std::thread
#include <vector>      // std::vector

/* Assignment 3: Multi-threading
   Fill in the sort function to implement a merge-sort algorithm.
 */

/* merge_ranges : merge two already‑sorted half‑ranges
    a   : original array
    buf : temporary buffer (same length as a)
    l   : left  boundary  [l, m)
    m   : middle boundary
    r   : right boundary  [m, r)
*/

template <typename T>
inline void merge_ranges(T* a, T* buf,
                         std::size_t l,
                         std::size_t m,
                         std::size_t r)
{
    std::size_t i = l, j = m, k = l;
    while (i < m && j < r)                   // pick smaller of a[i], a[j]
        buf[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i < m) buf[k++] = a[i++];         // copy leftovers
    while (j < r) buf[k++] = a[j++];

    /* copy merged range back into the original array */
    std::memcpy(a + l, buf + l, (r - l) * sizeof(T));
}

// seq_mergesort : classic recursive merge‑sort (single thread)

template <typename T>
void seq_mergesort(T* a, T* buf,
                   std::size_t l,
                   std::size_t r)            // half‑open interval [l, r)
{
    if (r - l <= 1) return;                 // nothing (or one element) to sort
    std::size_t m = l + ((r - l) >> 1);
    seq_mergesort(a, buf, l, m);
    seq_mergesort(a, buf, m, r);
    merge_ranges(a, buf, l, m, r);
}

// par_mergesort : divide‑and‑conquer merge‑sort with std::thread
// avail_threads – remaining “budget” of threads for this subtree

template <typename T>
void par_mergesort(T* a, T* buf,
                   std::size_t l,
                   std::size_t r,
                   unsigned    avail_threads)
{
    if (r - l <= 1) return;                 // base case
    if (avail_threads <= 1) {               // no extra threads → sequential
        seq_mergesort(a, buf, l, r);
        return;
    }

    std::size_t m = l + ((r - l) >> 1);
    unsigned left_threads  = avail_threads >> 1;         // ⌊n/2⌋
    unsigned right_threads = avail_threads - left_threads; // ⌈n/2⌉

    /* sort left half in a new thread */
    std::thread left_worker([=] {
        par_mergesort(a, buf, l, m, left_threads);
    });

    /* sort right half in current thread */
    par_mergesort(a, buf, m, r, right_threads);

    left_worker.join();                     // wait for the left side
    merge_ranges(a, buf, l, m, r);          // merge the two halves
}

// sort : entry point required by main.cc

template <typename T>
void sort(T* array,
          const std::size_t num_data,
          const unsigned    num_threads)
{
    if (num_data <= 1) return;              // already sorted

    /* allocate one temporary buffer – automatically freed */
    std::unique_ptr<T[]> buffer(new (std::nothrow) T[num_data]);
    if (!buffer) {
        std::cerr << "Error: failed to allocate temporary buffer\n";
        std::exit(1);
    }

    par_mergesort(array,
                  buffer.get(),
                  0,
                  num_data,
                  num_threads);             // launch parallel merge sort
}

#endif
