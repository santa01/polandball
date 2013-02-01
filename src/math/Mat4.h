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

#ifndef MAT4_H
#define	MAT4_H

#include "Mat3.h"

namespace PolandBall {

namespace Math {

class Mat4 {
public:
    Mat4();

    Mat4 operator *(const Mat4& mat) const;
    Mat4 operator *(float scalar) const;
    Mat4 operator +(const Mat4& matrix) const;
    Mat4 operator -(const Mat4& matrix) const;
    bool operator ==(const Mat4& matrix) const;
    bool operator !=(const Mat4& matrix) const;

    void transpose();

    float get(int row, int column) const;
    void set(int row, int column, float value);

    const float* data() const  {
        return (float*)&this->matrix;
    }

    Mat3 extractMat3() const;

private:
    float matrix[4][4];
};

}  // namespace Math

}  // namespace PolandBall

#endif  // MAT4_H
