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

#include "Mat3.h"
#include "Vec3.h"

#include <cmath>
#include <algorithm>

namespace PolandBall {

namespace Math {

Mat3::Mat3() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->matrix[i][j] = 0.0f;
        }
    }

    this->matrix[0][0] = 1.0f;
    this->matrix[1][1] = 1.0f;
    this->matrix[2][2] = 1.0f;
}

Mat3 Mat3::operator *(const Mat3& matrix) const {
    Mat3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, this->matrix[i][0] * matrix.get(0, j) +
                             this->matrix[i][1] * matrix.get(1, j) +
                             this->matrix[i][2] * matrix.get(2, j));
        }
    }
    
    return result;
}

Vec3 Mat3::operator *(const Vec3& vector) const {
    Vec3 result;

    for (int i = 0; i < 3; i++) {
        result.set(i, this->matrix[i][0] * vector.get(Vec3::X) +
                      this->matrix[i][1] * vector.get(Vec3::Y) +
                      this->matrix[i][2] * vector.get(Vec3::Z));
    }
    
    return result;
}

Mat3 Mat3::operator *(float scalar) const {
    Mat3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, this->matrix[i][j] * scalar);
        }
    }
    
    return result;
}

Mat3 Mat3::operator +(const Mat3& matrix) const {
    Mat3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, this->matrix[i][j] + matrix.get(i, j));
        }
    }
    
    return result;
}

Mat3 Mat3::operator -(const Mat3& matrix) const {
    Mat3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.set(i, j, this->matrix[i][j] - matrix.get(i, j));
        }
    }
    
    return result;    
}

bool Mat3::operator ==(const Mat3& matrix) const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (this->matrix[i][j] != matrix.get(i, j)) {
                return false;
            }
        }
    }
    
    return true;
}

bool Mat3::operator !=(const Mat3& matrix) const {
    return !(*this == matrix);
}

void Mat3::transpose() {
    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            std::swap(this->matrix[j][i], this->matrix[i][j]);
        }
    }
}

float Mat3::get(int row, int column) const {
    if (column < 0 || column > 2 || row < 0 || row > 2) {
        return NAN;
    }

    return this->matrix[row][column];
}

void Mat3::set(int row, int column, float value) {
    if (column < 0 || column > 2 || row < 0 || row > 2) {
        return;
    }

    this->matrix[row][column] = value;
}

}  // namespace Math

}  // namespace PolandBall
