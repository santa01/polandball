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

#include <BaseEntity.h>

namespace PolandBall {

namespace Game {

BaseEntity::BaseEntity() {
        // collider(std::make_shared<Collider>()) {
}

EntityType BaseEntity::getType() const {
    return this->type;
}

const Math::Vec3& BaseEntity::getSpeed() const {
    return this->speed;
}

void BaseEntity::setSpeed(const Math::Vec3& speed) {
    this->speed = speed;
}

void BaseEntity::accelerateBy(const Math::Vec3& acceleration) {
    this->speed += acceleration;
}

bool BaseEntity::isPassive() const {
    return this->passive;
}

void BaseEntity::setPassive(bool passive) {
    this->passive = passive;
}

bool BaseEntity::isCollidable() const {
    return this->collidable;
}

void BaseEntity::setCollidable(bool collidable) {
    this->collidable = collidable;
}

void BaseEntity::destroy() {
    this->destroyed = true;
}

// const std::shared_ptr<Collider>& BaseEntity::getCollider() const {
//     return this->collider;
// }

// void BaseEntity::setCollider(const std::shared_ptr<Collider>& collider) {
//     this->collider = collider;
// }

}  // namespace Game

}  // namespace PolandBall
