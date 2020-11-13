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

#include <Shearable.h>
#include <Logger.h>
#include <stdexcept>

namespace PolandBall {

namespace Game {

void Shearable::shearX(float slice, int totalSlices) {
    if (slice >= totalSlices) {
        throw std::invalid_argument(Graphene::LogFormat("Slice value is invalid"));
    }

    this->shear.set(0, 0, 1.0f / totalSlices);
    this->shear.set(0, 3, slice / totalSlices);
}

void Shearable::shearY(float slice, int totalSlices) {
    if (slice >= totalSlices) {
        throw std::invalid_argument(Graphene::LogFormat("Slice value is invalid"));
    }

    this->shear.set(1, 1, 1.0f / totalSlices);
    this->shear.set(1, 3, slice / totalSlices);
}

void Shearable::shearZ(float slice, int totalSlices) {
    if (slice >= totalSlices) {
        throw std::invalid_argument(Graphene::LogFormat("Slice value is invalid"));
    }

    this->shear.set(2, 2, 1.0f / totalSlices);
    this->shear.set(2, 3, slice / totalSlices);
}

const Math::Mat4& Shearable::getShear() const {
    return this->shear;
}

}  // namespace Game

}  // namespace PolandBall
