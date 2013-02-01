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

#include "Quaternion.h"
#include "Mat4.h"
#include "Vec3.h"

#include <cmath>

namespace PolandBall {

namespace Math {

Quaternion::Quaternion() {
    this->vector[X] = 0.0f;
    this->vector[Y] = 0.0f;
    this->vector[Z] = 0.0f;
    this->vector[W] = 1.0f;
}

Quaternion::Quaternion(float x, float y, float z, float w) {
    this->vector[X] = x;
    this->vector[Y] = y;
    this->vector[Z] = z;
    this->vector[W] = w;
}

Quaternion::Quaternion(const Vec3& axis, float angle) {
    float sinAngle = sinf(angle / 2);
    
    this->vector[X] = axis.get(Vec3::X) * sinAngle;
    this->vector[Y] = axis.get(Vec3::Y) * sinAngle;
    this->vector[Z] = axis.get(Vec3::Z) * sinAngle;
    this->vector[W] = cosf(angle / 2);
}

Quaternion Quaternion::operator *(const Quaternion& quaternion) const {
    Quaternion result;

    result.set(W, this->vector[W] * quaternion.get(W) -
                  this->vector[X] * quaternion.get(X) -
                  this->vector[Y] * quaternion.get(Y) -
                  this->vector[Z] * quaternion.get(Z));
    
    result.set(X, this->vector[W] * quaternion.get(X) +
                  this->vector[X] * quaternion.get(W) +
                  this->vector[Y] * quaternion.get(Z) -
                  this->vector[Z] * quaternion.get(Y));
    
    result.set(Y, this->vector[W] * quaternion.get(Y) +
                  this->vector[Y] * quaternion.get(W) +
                  this->vector[Z] * quaternion.get(X) -
                  this->vector[X] * quaternion.get(Z));
    
    result.set(Z, this->vector[W] * quaternion.get(Z) +
                  this->vector[Z] * quaternion.get(W) +
                  this->vector[X] * quaternion.get(Y) -
                  this->vector[Y] * quaternion.get(X));

    result.normalize();
    return result;
}

Quaternion& Quaternion::normalize() {
    float length = this->length();
    this->vector[X] /= length;
    this->vector[Y] /= length;
    this->vector[Z] /= length;
    this->vector[W] /= length;
    return *this;
}

float Quaternion::length() const {
    return sqrtf(vector[X] * vector[X] +
                 vector[Y] * vector[Y] +
                 vector[Z] * vector[Z] +
                 vector[W] * vector[W]);
}

float Quaternion::get(int index) const {
    switch (index) {
        case X:
        case Y:
        case Z:
        case W:
            return this->vector[index];

        default:
            return NAN;            
    }
}

void Quaternion::set(int index, float value) {
    switch (index) {
        case X:
        case Y:
        case Z:
        case W:
            this->vector[index] = value;
            break;           
    }
}

Mat4 Quaternion::extractMat4() const {
    Mat4 result;
    
    result.set(0, 0, 1 - 2 * this->vector[Y] * this->vector[Y] -
                         2 * this->vector[Z] * this->vector[Z]);
    result.set(0, 1, 2 * this->vector[X] * this->vector[Y] -
                     2 * this->vector[Z] * this->vector[W]);
    result.set(0, 2, 2 * this->vector[X] * this->vector[Y] +
                     2 * this->vector[Y] * this->vector[W]);
    
    result.set(1, 0, 2 * this->vector[X] * this->vector[Y] +
                     2 * this->vector[Z] * this->vector[W]);
    result.set(1, 1, 1 - 2 * this->vector[X] * this->vector[X] -
                         2 * this->vector[Z] * this->vector[Z]);
    result.set(1, 2, 2 * this->vector[Y] * this->vector[Z] -
                     2 * this->vector[X] * this->vector[W]);
    
    result.set(2, 0, 2 * this->vector[X] * this->vector[Z] -
                     2 * this->vector[Y] * this->vector[W]);
    result.set(2, 1, 2 * this->vector[Y] * this->vector[Z] +
                     2 * this->vector[X] * this->vector[W]);
    result.set(2, 2, 1 - 2 * this->vector[X] * this->vector[X] -
                         2 * this->vector[Y] * this->vector[Y]);
    
    return result;
}

}  // namespace Math

}  // namespace PolandBall
