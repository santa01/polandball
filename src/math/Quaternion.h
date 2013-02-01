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

#ifndef QUATERNION_H
#define	QUATERNION_H

#include "Vec3.h"
#include "Mat4.h"

namespace PolandBall {

namespace Math {

class Quaternion {
public:
    enum {
        X = 0,
        Y = 1,
        Z = 2,
        W = 3
    };

    Quaternion();
    Quaternion(float x, float y, float z, float w);
    Quaternion(const Vec3& axis, float angle);

    Quaternion operator *(const Quaternion& quaternmion) const;

    Quaternion& normalize();
    float length() const;

    float get(int index) const;
    void set(int index, float value);

    Mat4 extractMat4() const;

private:
    float vector[4];
};

}  // namespace Math

}  // namespace PolandBall

#endif  // QUATERNION_H
