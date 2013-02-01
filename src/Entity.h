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

#ifndef ENTITY_H
#define	ENTITY_H

#include "Collider.h"
#include "Sprite.h"

#include <memory>

namespace PolandBall {

class Entity: public IMovable {
public:
    Entity();
    ~Entity();

    using IMovable::setPosition;

    void setPosition(const Math::Vec3& position) {
        //this->collider->setPosition(position);
        this->sprite->setPosition(position);
    }

    Math::Vec3 getPosition() const {
        return this->sprite->getPosition();  // Should be equal to collider position
    }

    std::shared_ptr<Collider>& getCollider() {
        return this->collider;
    }

    void setCollider(const std::shared_ptr<Collider>& collider) {
        this->collider = collider;
    }

    std::shared_ptr<Sprite>& getSprite() {
        return this->sprite;
    }

    void setSprite(const std::shared_ptr<Sprite>& sprite) {
        this->sprite = sprite;
    }

private:
    Entity(const Entity&) = delete;
    Entity& operator =(const Entity&) = delete;

    std::shared_ptr<Collider> collider;
    std::shared_ptr<Sprite> sprite;
};

}

// namespace PolandBall

#endif  // ENTITY_H
