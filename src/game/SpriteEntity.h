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

#ifndef SPRITEENTITY_H
#define SPRITEENTITY_H

#include "Entity.h"
#include "Collider.h"
#include "Sprite.h"

#include <memory>

namespace PolandBall {

namespace Game {

class SpriteEntity: public Entity {
public:
    SpriteEntity():
            sprite(new Opengl::Sprite()) {
        this->primitive = this->sprite;
    }

    virtual ~SpriteEntity() {}

    const std::shared_ptr<Opengl::Sprite>& getSprite() const {
        return this->sprite;
    }

protected:
    virtual void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {}
    virtual void animate(float frameTime) {}

    std::shared_ptr<Opengl::Sprite> sprite;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // SPRITEENTITY_H
