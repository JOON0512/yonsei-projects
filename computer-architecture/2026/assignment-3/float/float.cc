#include "float.h"

using namespace std;

// Class constructor
_float_::_float_(void) : data(0) { /* Nothing to do */ }

// Class constructor
_float_::_float_(const _Float16 &v) : data(*((uint16_t*)&v)) { /* Nothing to do */ }

// Class constructor (private)
_float_::_float_(const uint16_t d) : data(d) { /* Nothing to do */ }
   
// Class copy constructor
_float_::_float_(const _float_ &f) : data(f.data) { /* Nothing to do */ }

// Class destructor
_float_::~_float_(void) { /* Nothing to do */ }

// cout << operator
ostream& operator<<(ostream &os, const _float_ &f) { os << float(*((_Float16*)&f.data)); return os; }

// float() operator
_float_::operator float() const { return float(*((_Float16*)&data)); }

// Assignment = operator
_float_& _float_::operator=(const _Float16 &v) { data = *((uint16_t*)&v); return *this; }

// Assignment = operator
_float_& _float_::operator=(const _float_ &f) { data = f.data; return *this; }

// Unary - operator
_float_ _float_::operator-(void) { return _float_(uint16_t(data ^ (uint16_t(1) << (exp_bits + frac_bits)))); }





/************************************************************
 * EEE3530 Assignment #3: Floating-point numbers            *
 * Implement the floating-point add, subtract, multiply,    *
 * and divide operators below. The functions should perform *
 * bit-level operations to produce the results.             *
 ************************************************************/

namespace {
    const uint16_t SIGN_MASK = uint16_t(1) << (exp_bits + frac_bits);
    const uint16_t QNAN_BITS = uint16_t(exp_mask | (uint16_t(1) << (frac_bits - 1)));
    const uint64_t HIDDEN_BIT = uint64_t(1) << frac_bits;
    const uint64_t HIDDEN_WITH_GR = HIDDEN_BIT << rnd_bits;

    struct Parts {
        bool sign;
        int stored_exp;
        int frac;
        int exp;          // Unbiased exponent used by arithmetic steps.
        uint64_t sig;     // Integer significand. Normal numbers include the hidden 1.
        bool zero;
        bool subnormal;
        bool normal;
        bool inf;
        bool nan;
    };

    // Extract and classify the sign, exponent, and fraction fields of a half value.
    Parts decode(uint16_t bits) {
        Parts p;
        p.sign = (bits & SIGN_MASK) != 0;
        p.stored_exp = int((bits & exp_mask) >> frac_bits);
        p.frac = int(bits & frac_mask);

        p.zero = false;
        p.subnormal = false;
        p.normal = false;
        p.inf = false;
        p.nan = false;

        if (p.stored_exp == 0) {
            p.exp = 1 - exp_bias;
            if (p.frac == 0) {
                p.zero = true;
                p.sig = 0;
            } else {
                p.subnormal = true;
                p.sig = uint64_t(p.frac);
            }
        } else if (p.stored_exp == exp_max) {
            p.exp = 0;
            p.sig = 0;
            if (p.frac == 0) {
                p.inf = true;
            } else {
                p.nan = true;
            }
        } else {
            p.normal = true;
            p.exp = p.stored_exp - exp_bias;
            p.sig = HIDDEN_BIT | uint64_t(p.frac);
        }

        return p;
    }

    // Assemble commonly used special values from fields.
    uint16_t make_zero(bool sign) {
        return sign ? SIGN_MASK : uint16_t(0);
    }

    uint16_t make_inf(bool sign) {
        return uint16_t((sign ? SIGN_MASK : uint16_t(0)) | exp_mask);
    }

    uint16_t make_nan(void) {
        return QNAN_BITS;
    }

    // Avoid undefined behavior when a shift amount is larger than the integer width.
    uint64_t shift_right(uint64_t value, int amount) {
        if (amount <= 0) {
            return value;
        }
        if (amount >= 64) {
            return 0;
        }
        return value >> amount;
    }

    // The assignment rounding rule uses only guard and round bits.
    uint64_t round_guard_round(uint64_t mant_with_gr) {
        uint64_t mant = mant_with_gr >> rnd_bits;
        uint64_t gr = mant_with_gr & ((uint64_t(1) << rnd_bits) - 1);

        if (gr > 0b10) {
            ++mant;
        } else if (gr == 0b10 && (mant & 1)) {
            ++mant;
        }

        return mant;
    }

    // Normalize, round, and assemble a finite result into half-precision fields.
    uint16_t pack_finite(bool sign, int exp, uint64_t mant_with_gr) {
        const int MIN_NORMAL_EXP = 1 - exp_bias;
        const int MAX_NORMAL_EXP = (exp_max - 1) - exp_bias;

        if (mant_with_gr == 0) {
            return make_zero(sign);
        }

        while (mant_with_gr >= (HIDDEN_WITH_GR << 1)) {
            mant_with_gr >>= 1;
            ++exp;
        }
        while (mant_with_gr < HIDDEN_WITH_GR && exp > MIN_NORMAL_EXP) {
            mant_with_gr <<= 1;
            --exp;
        }

        if (exp > MAX_NORMAL_EXP) {
            return make_inf(sign);
        }

        if (exp < MIN_NORMAL_EXP) {
            mant_with_gr = shift_right(mant_with_gr, MIN_NORMAL_EXP - exp);
            exp = MIN_NORMAL_EXP;
        }

        uint64_t mant = round_guard_round(mant_with_gr);
        if (mant == 0) {
            return make_zero(sign);
        }

        if (mant >= (HIDDEN_BIT << 1)) {
            mant >>= 1;
            ++exp;
            if (exp > MAX_NORMAL_EXP) {
                return make_inf(sign);
            }
        }

        if (exp == MIN_NORMAL_EXP && mant < HIDDEN_BIT) {
            return uint16_t((sign ? SIGN_MASK : uint16_t(0)) | uint16_t(mant & frac_mask));
        }

        uint16_t stored_exp = uint16_t(exp + exp_bias);
        uint16_t stored_frac = uint16_t((mant - HIDDEN_BIT) & frac_mask);
        return uint16_t((sign ? SIGN_MASK : uint16_t(0)) |
                        (stored_exp << frac_bits) |
                        stored_frac);
    }

    uint16_t add_bits(uint16_t a, uint16_t b) {
        Parts x = decode(a);
        Parts y = decode(b);

        if (x.nan || y.nan) {
            return make_nan();
        }
        if (x.inf || y.inf) {
            if (x.inf && y.inf && x.sign != y.sign) {
                return make_nan();
            }
            return x.inf ? make_inf(x.sign) : make_inf(y.sign);
        }

        if (x.zero && y.zero) {
            return make_zero(x.sign & y.sign);
        }
        if (x.zero) {
            return b;
        }
        if (y.zero) {
            return a;
        }

        uint64_t x_mant = x.sig << rnd_bits;
        uint64_t y_mant = y.sig << rnd_bits;
        int result_exp = x.exp;

        if (x.exp > y.exp) {
            y_mant = shift_right(y_mant, x.exp - y.exp);
            result_exp = x.exp;
        } else if (y.exp > x.exp) {
            x_mant = shift_right(x_mant, y.exp - x.exp);
            result_exp = y.exp;
        }

        bool result_sign;
        uint64_t result_mant;
        if (x.sign == y.sign) {
            result_sign = x.sign;
            result_mant = x_mant + y_mant;
        } else if (x_mant >= y_mant) {
            result_sign = x.sign;
            result_mant = x_mant - y_mant;
        } else {
            result_sign = y.sign;
            result_mant = y_mant - x_mant;
        }

        if (result_mant == 0) {
            return make_zero(false);
        }
        return pack_finite(result_sign, result_exp, result_mant);
    }

    uint16_t mul_bits(uint16_t a, uint16_t b) {
        Parts x = decode(a);
        Parts y = decode(b);
        bool result_sign = x.sign ^ y.sign;

        if (x.nan || y.nan) {
            return make_nan();
        }
        if ((x.inf && y.zero) || (x.zero && y.inf)) {
            return make_nan();
        }
        if (x.inf || y.inf) {
            return make_inf(result_sign);
        }
        if (x.zero || y.zero) {
            return make_zero(result_sign);
        }

        uint64_t product = x.sig * y.sig;
        int result_exp = x.exp + y.exp - (frac_bits - rnd_bits);
        return pack_finite(result_sign, result_exp, product);
    }

    uint16_t div_bits(uint16_t a, uint16_t b) {
        Parts x = decode(a);
        Parts y = decode(b);
        bool result_sign = x.sign ^ y.sign;

        if (x.nan || y.nan) {
            return make_nan();
        }
        if ((x.inf && y.inf) || (x.zero && y.zero)) {
            return make_nan();
        }
        if (x.inf) {
            return make_inf(result_sign);
        }
        if (y.inf) {
            return make_zero(result_sign);
        }
        if (y.zero) {
            return make_inf(result_sign);
        }
        if (x.zero) {
            return make_zero(result_sign);
        }

        // Extra internal quotient bits help find the guard/round bits after normalization.
        const int EXTRA_BITS = 16;
        const int SHIFT = frac_bits + rnd_bits + EXTRA_BITS;
        uint64_t quotient = (x.sig << SHIFT) / y.sig;
        int result_exp = x.exp - y.exp - EXTRA_BITS;
        return pack_finite(result_sign, result_exp, quotient);
    }
}

// Add + operator
_float_ _float_::operator+(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point add function.           *
     ********************************************************/

    // An example to extract the sign, exponent, and fraction of x (*this).
    // bool    x_sign = data >> (exp_bits + frac_bits);
    // int16_t x_exp  = (data & exp_mask) >> frac_bits;
    // int16_t x_frac = data & frac_mask;
    
    // An example to extract the sign, exponent, and fraction of y (arg).
    // bool    y_sign = y.data >> (exp_bits + frac_bits);
    // int16_t y_exp  = (y.data & exp_mask) >> frac_bits;
    // int16_t y_frac = y.data & frac_mask;

    // cout                                   << endl;
    // cout << "x_sign = "          << x_sign << endl;
    // cout << "x_exp  = "   << dec << x_exp  << endl;
    // cout << "x_frac = 0x" << hex << x_frac << endl;

    // cout << "y_sign = "          << y_sign << endl;
    // cout << "y_exp  = "   << dec << y_exp  << endl;
    // cout << "y_frac = 0x" << hex << y_frac << endl;
    // cout                                   << endl;

    // Put the calculated sign, exponent, and fraction into r.data.
    _float_ r;
    r.data = add_bits(data, y.data);
    return r;
}

// Subtract - operator
_float_ _float_::operator-(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point subtract function.      *
     ********************************************************/

    // x - y is implemented as x + (-y) by flipping y's sign bit.
    _float_ r;
    r.data = add_bits(data, uint16_t(y.data ^ SIGN_MASK));
    return r;
}

// Multiply * operator
_float_ _float_::operator*(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point multiply function.      *
     ********************************************************/

    // Multiply the integer significands and then pack the normalized result.
    _float_ r;
    r.data = mul_bits(data, y.data);
    return r;
}

// Divide * operator
_float_ _float_::operator/(const _float_ &y) {
    /********************************************************
     * EEE3530 Assignment #3                                *
     * Implement the floating-point divide function.        *
     ********************************************************/

    // Divide the integer significands with extra internal bits for rounding.
    _float_ r;
    r.data = div_bits(data, y.data);
    return r;
}