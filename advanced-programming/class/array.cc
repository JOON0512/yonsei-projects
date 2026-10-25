#include <cstdlib>
#include "array.h"

// Class constructor
array_t::array_t() :
    ptr(0),
    num_elements(0),
    array_size(0) {
}


/* Assignment 1: C++ Class
   Write a copy constructor and destructor of the class,
   and complete reserve() and push_back() functions.
  */

// destructor: deallocation - free array
array_t::~array_t(){
    for(size_t i = 0; i < num_elements; i++)
        ptr[i].~data_t();
    free(ptr);
}

// copy constructor: create a copy of a class
array_t::array_t(const array_t &m_array){
    
    array_size = m_array.array_size;
    num_elements = m_array.num_elements;
    // allocate mem for new arr
    ptr = (data_t*) malloc(sizeof(data_t) * array_size);
    if(ptr == nullptr){
        // Handle memory allocation failure (could also throw an exception)
        exit(EXIT_FAILURE);
    }
    // Copy each element using placement new and data_t's copy constructor
    for(size_t i = 0; i < num_elements; i++){
        new (&ptr[i]) data_t(m_array.ptr[i]);
    }

}


// Allocate a memory space for the specified number of elements.
void array_t::reserve(const size_t m_array_size) {
  // Only reallocate if the requested size is larger than the current capacity
  if(m_array_size <= array_size) return;
  
  // Allocate new memory block
  data_t * new_ptr = static_cast<data_t*>(malloc(m_array_size * sizeof(data_t)));
  if(new_ptr == nullptr){
      exit(EXIT_FAILURE);
  }
  
  // Copy existing elements to new memory
  for(size_t i = 0; i < num_elements; i++){
      new (&new_ptr[i]) data_t(ptr[i]); // use copy constructor via placement new
      // Destroy the original element
      ptr[i].~data_t();
  }
  
  // Free old memory and update pointers and capacity
  free(ptr);
  ptr = new_ptr;
  array_size = m_array_size;
}

// Add a new element at the end of the array.
void array_t::push_back(const data_t m_value) {
  // If there is no space left, increase the capacity
  if(num_elements == array_size) {
      size_t new_size = (array_size == 0 ? 1 : 2 * array_size);
      reserve(new_size);
  }
  // Construct the new element at the end of the array
  new (&ptr[num_elements]) data_t(m_value);
  num_elements++;
}
