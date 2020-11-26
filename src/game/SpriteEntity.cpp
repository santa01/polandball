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

#include <SpriteEntity.h>

namespace PolandBall {

namespace Game {

SpriteEntity::SpriteEntity() {
        // collider(std::make_shared<Collider>()) {
    this->shearX(1.0f, 2);
}

EntityType SpriteEntity::getType() const {
    return this->type;
}

const Math::Vec3& SpriteEntity::getSpeed() const {
    return this->speed;
}

void SpriteEntity::setSpeed(const Math::Vec3& speed) {
    this->speed = speed;
}

void SpriteEntity::accelerateBy(const Math::Vec3& acceleration) {
    this->speed += acceleration;
}

bool SpriteEntity::isPassive() const {
    return this->passive;
}

void SpriteEntity::setPassive(bool passive) {
    this->passive = passive;
}

bool SpriteEntity::isCollidable() const {
    return this->collidable;
}

void SpriteEntity::setCollidable(bool collidable) {
    this->collidable = collidable;
}

void SpriteEntity::destroy() {
    this->destroyed = true;
}

// const std::shared_ptr<Collider>& SpriteEntity::getCollider() const {
//     return this->collider;
// }

// void SpriteEntity::setCollider(const std::shared_ptr<Collider>& collider) {
//     this->collider = collider;
// }

}  // namespace Game

}  // namespace PolandBall
