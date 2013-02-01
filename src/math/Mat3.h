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

#ifndef MAT3_H
#define	MAT3_H

#include "Vec3.h"

namespace PolandBall {

namespace Math {

class Mat3 {
public:
    Mat3();

    Mat3 operator *(const Mat3& matrix) const;
    Vec3 operator *(const Vec3& vector) const;
    Mat3 operator *(float scalar) const;
    Mat3 operator +(const Mat3& matrix) const;
    Mat3 operator -(const Mat3& matrix) const;
    bool operator ==(const Mat3& matrix) const;
    bool operator !=(const Mat3& matrix) const;

    void transpose();

    float get(int row, int column) const;
    void set(int row, int column, float value);

    const float* data() const  {
        return (float*)&this->matrix;
    }

private:
    float matrix[3][3];
};

}  // namespace Math

}  // namespace PolandBall

#endif  // MAT3_H
