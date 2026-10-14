#pragma once

#include <cstdint>
#include <stddef.h>

/////////////////////////////////////////////////////////////
//Miscelanious
///////////////////////////////////////////////////////////////
uint16_t calculateCRC16(const uint8_t* buffer, size_t bufferSize);

////////////////////////////////////////////////////////////
//Math
/////////////////////////////////////////////////////////////
//Complex Number
struct Complex {
    float real;
    float imag;
};

Complex c_add(const Complex& a, const Complex& b);
Complex c_subtract(const Complex& a, const Complex& b);
Complex c_multiply(const Complex& a, const Complex& b);
Complex c_conjugate(const Complex& a);
Complex c_inverse(const Complex& a);
Complex c_divide(const Complex& a, const Complex& b);

//Quaternion
struct Quaternion {
    float w;
    float x;
    float y;
    float z;
};

Quaternion q_add(const Quaternion& a, const Quaternion& b);
Quaternion q_subtract(const Quaternion& a, const Quaternion& b);
Quaternion q_multiply(const Quaternion& a, const Quaternion& b);
Quaternion q_conjugate(const Quaternion& q);
Quaternion q_inverse(const Quaternion& q);
Quaternion q_divide(const Quaternion& a, const Quaternion& b);