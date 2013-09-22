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

#ifndef LINEENTITY_H
#define LINEENTITY_H

#include "Entity.h"
#include "Collider.h"
#include "Line.h"

#include <memory>

namespace PolandBall {

namespace Game {

class LineEntity: public Entity {
public:
    LineEntity():
            line(new Opengl::Line()) {
        this->primitive = this->line;
    }

    virtual ~LineEntity() {}

    const std::shared_ptr<Opengl::Line>& getLine() const {
        return this->line;
    }

protected:
    virtual void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {}
    virtual void animate(float frameTime) {}

    std::shared_ptr<Opengl::Line> line;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // LINEENTITY_H
