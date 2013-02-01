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

#include "Mat4.h"

#include <cmath>
#include <algorithm>

namespace PolandBall {

namespace Math {

Mat4::Mat4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->matrix[i][j] = 0.0f;
        }
    }

    this->matrix[0][0] = 1.0f;
    this->matrix[1][1] = 1.0f;
    this->matrix[2][2] = 1.0f;
    this->matrix[3][3] = 1.0f;
}

Mat4 Mat4::operator *(const Mat4& matrix) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][0] * matrix.get(0, j) +
                             this->matrix[i][1] * matrix.get(1, j) +
                             this->matrix[i][2] * matrix.get(2, j) +
                             this->matrix[i][3] * matrix.get(3, j));
        }
    }
    
    return result;
}

Mat4 Mat4::operator *(float scalar) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][j] * scalar);
        }
    }
    
    return result;
}

Mat4 Mat4::operator +(const Mat4& matrix) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][j] + matrix.get(i, j));
        }
    }
    
    return result;
}

Mat4 Mat4::operator -(const Mat4& matrix) const {
    Mat4 result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.set(i, j, this->matrix[i][j] - matrix.get(i, j));
        }
    }
    
    return result;    
}

bool Mat4::operator ==(const Mat4& matrix) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (this->matrix[i][j] != matrix.get(i, j)) {
                return false;
            }
        }
    }
    
    return true;
}

bool Mat4::operator !=(const Mat4& matrix) const {
    return !(*this == matrix);
}

void Mat4::transpose() {
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            std::swap(this->matrix[j][i], this->matrix[i][j]);
        }
    }
}

float Mat4::get(int row, int column) const {
    if (column < 0 || column > 3 || row < 0 || row > 3) {
        return NAN;
    }

    return this->matrix[row][column];
}

void Mat4::set(int row, int column, float value) {
    if (column < 0 || column > 3 || row < 0 || row > 3) {
        return;
    }

    this->matrix[row][column] = value;
}

Mat3 Mat4::extractMat3() const {
    Mat3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, this->matrix[i][j]);
        }
    }

    return result;
}

}  // namespace Math

}  // namespace PolandBall
