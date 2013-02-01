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

#ifndef VEC3_H
#define	VEC3_H

namespace PolandBall {

namespace Math {

class Vec3 {
public:
    enum {
        X = 0,
        Y = 1,
        Z = 2
    };

    static const Vec3 UNIT_X;
    static const Vec3 UNIT_Y;
    static const Vec3 UNIT_Z;
    static const Vec3 ZERO;

    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator -(const Vec3& vector) const;
    Vec3 operator +(const Vec3& vector) const;
    Vec3 operator *(float scalar) const;
    Vec3& operator -=(const Vec3& vector);
    Vec3& operator +=(const Vec3& vector);
    Vec3& operator *=(float scalar);
    bool operator ==(const Vec3& vector) const;
    bool operator !=(const Vec3& vector) const;
    Vec3 operator -() const;

    float dot(const Vec3& vector) const;
    Vec3 cross(const Vec3& vector) const;
    Vec3& normalize();
    float length() const;
    float squareLength() const;

    float get(int index) const;
    void set(int index, float value);

    const float* data() const  {
        return (float*)&this->vector;
    }

private:
    float vector[3];
};

}  // namespace Math

}  // namespace PolandBall

#endif  // VEC3_H
