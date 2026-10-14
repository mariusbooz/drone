#include "utils.h"


Complex c_add(const Complex& a, const Complex& b) {
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

Complex c_subtract(const Complex& a, const Complex& b) {
    Complex result;
    result.real = a.real - b.real;
    result.imag = a.imag - b.imag;
    return result;
}

Complex c_multiply(const Complex& a, const Complex& b) {
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

Complex c_conjugate(const Complex& a) {
    Complex result;
    result.real = a.real;
    result.imag = -a.imag;
    return result;
}

Complex c_inverse(const Complex& a) {
    Complex result;
    float denominator = a.real * a.real + a.imag * a.imag;
    result.real = a.real / denominator;
    result.imag = -a.imag / denominator;
    return result;
}

Complex c_divide(const Complex& a, const Complex& b) {
    Complex result=c_multiply(a, c_inverse(b));
    return result;
}

Quaternion q_add(const Quaternion& a, const Quaternion& b) {
    Quaternion result;
    result.w = a.w + b.w;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Quaternion q_subtract(const Quaternion& a, const Quaternion& b) {
    Quaternion result;
    result.w = a.w - b.w;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Quaternion q_multiply(const Quaternion& a, const Quaternion& b) {
    Quaternion result;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    return result;
}

Quaternion q_conjugate(const Quaternion& q) {
    Quaternion result;
    result.w = q.w;
    result.x = -q.x;
    result.y = -q.y;
    result.z = -q.z;
    return result;
}

Quaternion q_inverse(const Quaternion& q) {
    Quaternion result;
    float denominator = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
    result.w = q.w / denominator;
    result.x = -q.x / denominator;
    result.y = -q.y / denominator;
    result.z = -q.z / denominator;
    return result;
}

Quaternion q_divide(const Quaternion& a, const Quaternion& b) {
    Quaternion result=q_multiply(a, q_inverse(b));
    return result;
}