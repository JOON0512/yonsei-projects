#ifndef __FLOAT_H__
#define __FLOAT_H__

#define exp_bits  5                         // # of exponent bits
#define frac_bits 10                        // # of fraction bits
#define rnd_bits  2                         // # of rounding bits

#define exp_bias  15                        // Exponent bias
#define exp_max   31                        // Max of exponent
#define exp_mask  0x7c00                    // Exponent mask
#define frac_mask 0x03ff                    // Fraction mask

#define isNaN(x) (((x & exp_mask) == exp_mask) &&  (x & frac_mask))
#define isInf(x) (((x & exp_mask) == exp_mask) && !(x & frac_mask))

#include <cassert>
#include <cstdint>
#include <iostream>

class _float_ {
public:
    _float_(void);                          // Class constructor
    _float_(const _Float16 &v);             // Class constructor
    _float_(const _float_ &f);              // Class copy constructor
    ~_float_(void);                         // Class destructor

    friend std::ostream& operator<<(std::ostream &os, const _float_ &f);
    operator float() const;
    _float_& operator=(const _Float16 &v);  // Assignment = operator
    _float_& operator=(const _float_ &f);   // Assignment = operator
    _float_ operator-(void);                // Unary - operator
    _float_ operator+(const _float_ &f);    // Add + operator
    _float_ operator-(const _float_ &f);    // Subtract - operator
    _float_ operator*(const _float_ &f);    // Multiply * operator
    _float_ operator/(const _float_ &f);    // Divide / operator

private:
    _float_(const uint16_t d);              // Class constructor
    uint16_t data;                          // 16-bit data
};

// Check system
void check_system();

#endif
