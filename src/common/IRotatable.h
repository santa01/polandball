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

#ifndef ROTATABLE_H
#define ROTATABLE_H

#include "Vec3.h"

namespace PolandBall {

class IRotatable {
public:
    virtual ~IRotatable() {}

    void roll(float angle) {
        this->rotate(Math::Vec3(0.0f, 0.0f, 1.0f), angle);
    }

    void yaw(float angle) {
        this->rotate(Math::Vec3(0.0f, 1.0f, 0.0f), angle);
    }

    void pitch(float angle) {
        this->rotate(Math::Vec3(1.0f, 0.0f, 0.0f), angle);
    }

    virtual float getXAngle() const = 0;
    virtual float getYAngle() const = 0;
    virtual float getZAngle() const = 0;

    virtual void rotate(const Math::Vec3& vector, float angle) = 0;
};

}  // namespace PolandBall

#endif  // ROTATABLE_H
