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

#ifndef LINE_H
#define LINE_H

#include "Primitive.h"

#include <Vec3.h>
#include <Vec4.h>

namespace PolandBall {

namespace Opengl {

class Line: public Primitive {
public:
    Line():
            from(Math::Vec3::ZERO),
            to(Math::Vec3::UNIT_X) {
        this->initialize();
    }

    const Math::Vec4& getColor() const {
        return this->color;
    }

    void setColor(const Math::Vec4& color) {
        this->color = color;
    }

    const Math::Vec3& getFrom() const {
        return this->from;
    }

    void setFrom(const Math::Vec3& from) {
        this->from = from;
        this->initialize();
    }

    const Math::Vec3& getTo() const {
        return this->to;
    }

    void setTo(const Math::Vec3& to) {
        this->to = to;
        this->initialize();
    }

private:
    void initialize();

    void beforeRender() {
        this->effect->enable();
        this->effect->setUniform("color", this->color);
    }

    Math::Vec4 color;
    Math::Vec3 from;
    Math::Vec3 to;
};

}  // namespace Opengl

}  // namespace PolandBall

#endif  // LINE_H
