#include "float.h"

using namespace std;

int main(void) {
    /******************************************
     * DO NOT MODIFY THE CODE BELOW THIS LINE *
     ******************************************/
    check_system();

    cout << "======== [Tests for grading] ========" << endl;

    _float_ x, y;
    unsigned t = 0, u = 0;

    // Add two positive numbers.
    x = _Float16(8.125);
    y = _Float16(3.625);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add two numbers of different signs.
    x = _Float16(-98.11);
    y = _Float16(15.57);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add two negative numbers.
    x = _Float16(-23.83);
    y = _Float16(-8.923);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add zero.
    x = _Float16(0.0);
    y = _Float16(-48.21);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add two large numbers.
    x = _Float16(1.28e+4);
    y = _Float16(5.12e+4);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add two denormalized numbers.
    u = 1;                                  // Set the LSB.
    x = *((_Float16*)&u);
    u <<= 2;                                // Set the third LSB
    y = *((_Float16*)&u);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add max and min.
    u = (0x1e << frac_bits) + frac_mask;    // Max (exponent=30, all fraction bits set)
    x = *((_Float16*)&u);
    u = 1;                                  // Min (exponent=0, LSB of fraction set)
    y = *((_Float16*)&u);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add two values of opposite signs.
    u = (0x1e << frac_bits) + frac_mask;    // Max (exponent=30, all fraction bits set)
    x = *((_Float16*)&u);
    y = -x;
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add infinity.
    x = _Float16(19.25);
    u = 0x1f << frac_bits;                  // Infinity (exponent=31, fraction=0)
    y = *((_Float16*)&u);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Add a normalized number and a denormalized number.
    u = 0x1 << frac_bits;                   // Normalized   (exponent=1, fraction=0)
    x = *((_Float16*)&u);
    u = 0x1 << (frac_bits - 1);             // Denormalized (exponent=0, MSB of fraction set)
    y = *((_Float16*)&u);
    cout << "Test #" << ++t << ": " << float(x) << " + " << float(y) << " = " << float(x + y) << endl;

    // Subtract two positive numbers.
    x = _Float16(8.625);
    y = _Float16(24.125);
    cout << "Test #" << ++t << ": " << float(x) << " - " << float(y) << " = " << float(x - y) << endl;

    // Subtract the same number.
    x = _Float16(-4.185);
    y = x;
    cout << "Test #" << ++t << ": " << float(x) << " - " << float(y) << " = " << float(x - y) << endl;
   
    // Subtract two small numbers.
    x = _Float16(-3.34e-4);
    y = _Float16(-8.144e-3);
    cout << "Test #" << ++t << ": " << float(x) << " - " << float(y) << " = " << float(x - y) << endl;

    // Subtract infinity.
    x = _Float16(-13.28);
    u = 0x1f << frac_bits;                  // Infinity (exponent=31, fraction=0) 
    y = *((_Float16*)&u);
    cout << "Test #" << ++t << ": " << float(x) << " - " << float(y) << " = " << float(x - y) << endl;

    // Subtract NaN;
    x = _Float16(39.44);
    u = 0x3f << (frac_bits - 1);            // NaN (exponent=255, MSB of fraction set)
    y = *((_Float16*)&u);
    cout << "Test #" << ++t << ": " << float(x) << " - " << float(y) << " = " << float(x - y) << endl;

    // Subtract a normalized number from a denormalized number.
    u = 0x1 << (frac_bits - 1);             // Denormalized (exponent=0, MSB of fraction set)
    x = *((_Float16*)&u);
    u = 0x1 << frac_bits;                   // Normalized   (exponent=1, fraction=0)
    y = *((_Float16*)&u);
    cout << "Test #" << ++t << ": " << float(x) << " - " << float(y) << " = " << float(x - y) << endl;

    // Subtract the denormalized numbers of opposite signs.
    u = 0x1 << (frac_bits - 1);             // Denormalized (exponent=0, MSB of fraction set)
    x = *((_Float16*)&u);
    y = -x;
    cout << "Test #" << ++t << ": " << float(x) << " - " << float(y) << " = " << float(x - y) << endl;

    // Multiply two positive numbers.
    x = _Float16(5.25);
    y = _Float16(4.23);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Multiply two negative numbers.
    x = _Float16(-0.45);
    y = _Float16(-3.24);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Multiply two numbers of different signs.
    x = _Float16(13.38);
    y = _Float16(-6.25);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Multiply zero.
    x = _Float16(1288.2);
    y = _Float16(0.0);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Multiply two large numbers.
    x = _Float16(2.8e+2);
    y = _Float16(2.3e+2);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Multiply two small numbers.
    x = _Float16(-9.8e-3);
    y = _Float16(2.8e-2);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Multiply two smaller numbers.
    x = _Float16(6.2e-4);
    y = _Float16(1.7e-3);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Multiply -infinity.
    u = 0x3f << frac_bits;                  // -Infinity (sign=1, exponent=31, fraction=0) 
    x = *((_Float16*)&u);
    y = _Float16(-3.24);
    cout << "Test #" << ++t << ": " << float(x) << " * " << float(y) << " = " << float(x * y) << endl;

    // Divide two positive numbers.
    x = _Float16(6.4e+4);
    y = _Float16(0.8e+2);
    cout << "Test #" << ++t << ": " << float(x) << " / " << float(y) << " = " << float(x / y) << endl;

    // Divide two numbers of different signs.
    x = _Float16(0.0625);
    y = _Float16(-1.25);
    cout << "Test #" << ++t << ": " << float(x) << " / " << float(y) << " = " << float(x / y) << endl;

    // Divide a small number by a large number.
    x = _Float16(6.25e-4);
    y = _Float16(2.5e+3);
    cout << "Test #" << ++t << ": " << float(x) << " / " << float(y) << " = " << float(x / y) << endl;

    // Divide a large number by a small number.
    x = _Float16(3.55e+3);
    y = _Float16(-8.5e-2);
    cout << "Test #" << ++t << ": " << float(x) << " / " << float(y) << " = " << float(x / y) << endl;

    // Divide by zero.
    x = _Float16(-141.9);
    y = _Float16(0.0);
    cout << "Test #" << ++t << ": " << float(x) << " / " << float(y) << " = " << float(x / y) << endl;

    cout << "======== [End of tests] ========" << endl;

    /******************************************
     * DO NOT MODIFY THE CODE ABOVE THIS LINE *
     ******************************************/

    return 0;
}

