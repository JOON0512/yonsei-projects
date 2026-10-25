#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#include "sort.h"
int main() {
    std::mt19937 rng(7);
    for(unsigned size: {0u,1u,2u,17u,513u}) for(unsigned threads: {1u,2u,4u,8u}) {
        std::vector<int> values(size);
        for(auto &v:values) v=int(rng()%101)-50;
        auto expected=values;
        std::sort(expected.begin(),expected.end());
        sort(values.data(),values.size(),threads);
        assert(values==expected);
    }
    std::cout << "PASS: 20 sorting cases\n";
}
