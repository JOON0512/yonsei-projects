#include <cassert>
#include <cstddef>
#include <string>
#include "vector.h"

int main() {
    vector_t<std::string> original;
    vector_t<std::string> empty_copy(original);
    assert(empty_copy.size() == 0);
    original.insert(original.begin(), "first");
    original.push_back("second");
    original.reserve(16);
    vector_t<std::string> copy(original);
    assert(copy.size() == 2 && copy[0] == "first" && copy[1] == "second");
    copy[0] = "independent";
    original.clear();
    assert(copy[0] == "independent" && copy[1] == "second");
    vector_t<std::string> reserved_empty(original);
    assert(reserved_empty.size() == 0 && reserved_empty.capacity() == 16);
    copy.erase(copy.begin());
    assert(copy.size() == 1 && copy[0] == "second");
}
