#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <cstdlib>
#include <new>

// Constructor
template <typename T>
vector_t<T>::vector_t(void) :
    array(0),
    array_size(0),
    num_elements(0) {
    // Nothing to do
}

// Get the number of elements in the array.
template <typename T>
inline size_t vector_t<T>::size(void) const { return num_elements; }

// Get the allocated array size in the unit of elements.
template <typename T>
inline size_t vector_t<T>::capacity(void) const { return array_size; }

// Get a reference of the element at the given index.
template <typename T>
inline T& vector_t<T>::operator[](const size_t m_index) const { return array[m_index]; }

// Get an iterator pointing to the first element of the array.
template <typename T>
inline typename vector_t<T>::iterator vector_t<T>::begin(void) const {
    return iterator_t<T>(array);
}

// Get an iterator pointing to the next of the last element.
template <typename T>
inline typename vector_t<T>::iterator vector_t<T>::end(void) const {
    return iterator_t<T>(array ? array+num_elements : nullptr);
}


/* Assignment 2: C++ Template
   Fill in the following functions of the template class.
 */


// Copy constructor
template <typename T>
vector_t<T>::vector_t(const vector_t<T> &m_vector)
    : array(nullptr), array_size(m_vector.array_size), num_elements(0) {
    if (array_size) {
        array = static_cast<T*>(std::malloc(sizeof(T) * array_size));
        if (!array) throw std::bad_alloc();
        try {
            for (; num_elements < m_vector.num_elements; ++num_elements)
                new (&array[num_elements]) T(m_vector.array[num_elements]);
        } catch (...) {
            clear();
            std::free(array);
            throw;
        }
    }
}

// Destructor
template <typename T>
vector_t<T>::~vector_t(void) {
    for (size_t i = 0; i < num_elements; ++i) array[i].~T();
    std::free(array);
}

// Reserve an array space for the given number of elements.
template <typename T>
void vector_t<T>::reserve(size_t m_array_size) {
    if (m_array_size <= array_size) return; // enough
    T* new_array = static_cast<T*>(std::malloc(sizeof(T) * m_array_size));

    // move original element
    for (size_t i = 0; i < num_elements; ++i) {
        new (&new_array[i]) T(array[i]);
        array[i].~T();
    }

    std::free(array);
    array       = new_array;
    array_size  = m_array_size;
}

// Resize the array to have the given number of elements.
template <typename T>
void vector_t<T>::resize(size_t m_num_elements, T m_data) {
    if (m_num_elements > array_size) {
        size_t new_cap = (array_size == 0) ? 1 : array_size;
        while (new_cap < m_num_elements) new_cap <<= 1;
        reserve(new_cap);
    }
    // create new element / clear original element
    if (m_num_elements > num_elements) {
        for (size_t i = num_elements; i < m_num_elements; ++i)
            new (&array[i]) T(m_data);
    } else if (m_num_elements < num_elements) {
        for (size_t i = m_num_elements; i < num_elements; ++i)
            array[i].~T();
    }
    num_elements = m_num_elements;
}

// Remove all elements in the array.
template <typename T>
void vector_t<T>::clear(void) {
    for (size_t i = 0; i < num_elements; ++i) array[i].~T();
    num_elements = 0;
}

// Add a new element at the end of the array.
template <typename T>
void vector_t<T>::push_back(const T &m_data) {
    if (array_size == 0)
        reserve(1);
    else if (num_elements == array_size)
        reserve(array_size << 1);

    new (&array[num_elements++]) T(m_data);
}

// Remove the last element in the array.
template <typename T>
void vector_t<T>::pop_back(void) {
    if (num_elements == 0) return;
    array[--num_elements].~T();
}

// Assign new contents to the array.
template <typename T>
vector_t<T>& vector_t<T>::operator=(const vector_t<T>& m_vector) {
    if (this == &m_vector) return *this; // self-assignment

    // clear current content
    clear();
    if (m_vector.num_elements > array_size) {
        std::free(array);
        array_size = m_vector.num_elements;
        array      = static_cast<T*>(std::malloc(sizeof(T) * array_size));
    }

    for (size_t i = 0; i < m_vector.num_elements; ++i)
        new (&array[i]) T(m_vector.array[i]);

    num_elements = m_vector.num_elements;
    return *this;
}

// Add a new element at the location pointed by the iterator.
template <typename T>
typename vector_t<T>::iterator vector_t<T>::insert(vector_t<T>::iterator m_it,
                                                   const T &m_data) {
    size_t idx = array ? m_it.ptr - array : 0;      // empty insertion is at 0

    if (array_size == 0) reserve(1);
    else if (num_elements == array_size) reserve(array_size << 1);

    // move one by one from the back to the front
    for (size_t i = num_elements; i > idx; --i) {
        new (&array[i]) T(array[i - 1]);
        array[i - 1].~T();
    }

    new (&array[idx]) T(m_data);
    ++num_elements;
    return iterator_t<T>(array + idx);
}

// Erase an element at the location pointed by the iterator.
template <typename T>
typename vector_t<T>::iterator vector_t<T>::erase(vector_t<T>::iterator m_it) {

    size_t idx = m_it.ptr - array;          // delete idx
    if (idx >= num_elements) return end();  // prevent IB

    array[idx].~T();

    // move one by one from the front to the back
    for (size_t i = idx; i < num_elements - 1; ++i) {
        new (&array[i]) T(array[i + 1]);
        array[i + 1].~T();
    }

    --num_elements;

    return iterator_t<T>(array + idx);
}

#endif
