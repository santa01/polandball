/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Vec3.h"

#include <cmath>

namespace PolandBall {

namespace Math {

const Vec3 Vec3::UNIT_X(1.0f, 0.0f, 0.0f);
const Vec3 Vec3::UNIT_Y(0.0f, 1.0f, 0.0f);
const Vec3 Vec3::UNIT_Z(0.0f, 0.0f, 1.0f);
const Vec3 Vec3::ZERO(0.0f, 0.0f, 0.0f);

Vec3::Vec3() {
    for (int i = 0; i < 3; i++)
        this->vector[i] = 0.0f;
}

Vec3::Vec3(float x, float y, float z) {
    this->vector[X] = x;
    this->vector[Y] = y;
    this->vector[Z] = z;
}

Vec3 Vec3::operator -(const Vec3& vector) const {
    Vec3 me(*this);
    return me -= vector;
}

Vec3 Vec3::operator +(const Vec3& vector) const {
    Vec3 me(*this);
    return me += vector;
}

Vec3 Vec3::operator *(float scalar) const {
    Vec3 me(*this);
    return me *= scalar;
}

Vec3& Vec3::operator -=(const Vec3& vector) {
    this->vector[X] -= vector.get(X);
    this->vector[Y] -= vector.get(Y);
    this->vector[Z] -= vector.get(Z);
    return *this;
}

Vec3& Vec3::operator +=(const Vec3& vector) {
    this->vector[X] += vector.get(X);
    this->vector[Y] += vector.get(Y);
    this->vector[Z] += vector.get(Z);
    return *this;
}

Vec3& Vec3::operator *=(float scalar) {
    this->vector[X] *= scalar;
    this->vector[Y] *= scalar;
    this->vector[Z] *= scalar;
    return *this;
}

bool Vec3::operator ==(const Vec3& vector) const {
    return (this->vector[X] == vector.get(X)) &&
           (this->vector[Y] == vector.get(Y)) &&
           (this->vector[Z] == vector.get(Z));
}

bool Vec3::operator !=(const Vec3& vector) const {
    return !(*this == vector);
}

Vec3 Vec3::operator -() const {
    return Vec3(-this->vector[X],
                -this->vector[Y],
                -this->vector[Z]);
}

float Vec3::dot(const Vec3& vector) const {
    return this->vector[X] * vector.get(X) +
           this->vector[Y] * vector.get(Y) +
           this->vector[Z] * vector.get(Z);
}

Vec3 Vec3::cross(const Vec3& vector) const {
    return Vec3(this->vector[Y] * vector.get(Z) - this->vector[Z] * vector.get(Y),
                this->vector[Z] * vector.get(X) - this->vector[X] * vector.get(Z),
                this->vector[X] * vector.get(Y) - this->vector[Y] * vector.get(X));
}

Vec3& Vec3::normalize() {
    float length = this->length();
    this->vector[X] /= length;
    this->vector[Y] /= length;
    this->vector[Z] /= length;
    return *this;
}

float Vec3::length() const {
    return sqrtf(this->squareLength());
}

float Vec3::squareLength() const {
    return this->vector[X] * this->vector[X] +
           this->vector[Y] * this->vector[Y] +
           this->vector[Z] * this->vector[Z];
}

float Vec3::get(int index) const {
    switch (index) {
        case X:
        case Y:
        case Z:
            return this->vector[index];

        default:
            return NAN;            
    }
}

void Vec3::set(int index, float value) {
    switch (index) {
        case X:
        case Y:
        case Z:
            this->vector[index] = value;
            break;          
    }
}

}  // namespace Math

}  // namespace PolandBall
