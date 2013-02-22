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
#include "Vec3.h"
#include "IMovable.h"
#include "IScalable.h"
#include "IReplicable.h"
#include "ResourceManager.h"
#include "Signals.h"

#include <memory>
#include <utility>

namespace PolandBall {

class Entity: public IMovable, public IScalable, public IReplicable {
public:
    enum EntityType {
        TYPE_STATIC = 0,
        TYPE_DYNAMIC = 1
    };

    Entity():
            collider(new Collider()),
            sprite(new Sprite()) {
        this->type = TYPE_STATIC;
        this->initialize();
    }

    Entity(EntityType type):
            collider(new Collider()),
            sprite(new Sprite()) {
        this->type = type;
        this->initialize();
    }

    virtual ~Entity() {}

    using IMovable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->updatePosition(position - this->getPosition());  // Emit signal
        this->sprite->setPosition(position);
        this->collider->setPosition(position);
    }

    Math::Vec3 getPosition() const {
        return this->sprite->getPosition();  // Positions are syncronized
    }

    void scaleX(float factor) {
        this->sprite->scaleX(factor);
        this->collider->scaleX(factor);
    }

    void scaleY(float factor) {
        this->sprite->scaleY(factor);
        this->collider->scaleY(factor);
    }

    void scaleZ(float factor) {
        this->sprite->scaleZ(factor);
        this->collider->scaleZ(factor);
    }

    float getXFactor() const {
        return this->sprite->getXFactor();  // Factors are syncronized
    }

    float getYFactor() const {
        return this->sprite->getYFactor();  // Factors are syncronized
    }

    float getZFactor() const {
        return this->sprite->getZFactor();  // Factors are syncronized
    }

    void replicateX(float factor) {
        this->sprite->replicateX(factor);
        this->collider->scaleX(factor);
    }

    void replicateY(float factor) {
        this->sprite->replicateY(factor);
        this->collider->scaleY(factor);
    }

    void replicateZ(float factor) {
        this->sprite->replicateZ(factor);
        this->collider->scaleZ(factor);
    }

    float getXReplicaFactor() const {
        return this->sprite->getXReplicaFactor();
    }

    float getYReplicaFactor() const {
        return this->sprite->getYReplicaFactor();
    }

    float getZReplicaFactor() const {
        return this->sprite->getZReplicaFactor();
    }

    std::unique_ptr<Collider>& getCollider() {
        return this->collider;
    }

    // NOTE: As far as we use std::move, unique_ptr
    // passed as parameter will be set to nullptr!
    void setCollider(std::unique_ptr<Collider>& collider) {
        if (collider != nullptr) {
            this->collider = std::move(collider);
        }
    }

    std::shared_ptr<Sprite>& getSprite() {
        return this->sprite;
    }

    void setSprite(const std::shared_ptr<Sprite>& sprite) {
        if (sprite != nullptr) {
            this->sprite = sprite;
        }
    }

    bool isCollidable() const {
        return this->collidable;
    }

    void setCollidable(bool collidable) {
        this->collidable = collidable;
    }

    bool isRenderable() const {
        return this->renderable;
    }

    void setRenderable(bool renderable) {
        this->renderable = renderable;
    }

    // I have no idea why static entity could became dynamic, hence no setter
    EntityType getType() const {
        return this->type;
    }

    const Math::Vec3& getSpeed() const {
        return this->currentSpeed;
    }

    void setSpeed(const Math::Vec3& speed) {
        this->currentSpeed = speed;
    }

    void accelerateBy(const Math::Vec3& acceleration) {
        this->currentSpeed += acceleration;
    }

    virtual void collideSide(Collider::CollideSide side) {}

    // Signal
    Signals::Signal<Math::Vec3> updatePosition;

    // Slot
    void onPositionUpdate(const Math::Vec3& positionDelta) {
        Math::Vec3 currentPosition = this->getPosition();
        currentPosition.set(Math::Vec3::X, currentPosition.get(Math::Vec3::X) + positionDelta.get(Math::Vec3::X));
        currentPosition.set(Math::Vec3::Y, currentPosition.get(Math::Vec3::Y) + positionDelta.get(Math::Vec3::Y));
        this->setPosition(currentPosition);
    }

private:
    void initialize() {
        this->renderable = true;
        this->collidable = true;
    }

    std::unique_ptr<Collider> collider;
    std::shared_ptr<Sprite> sprite;

    bool renderable;
    bool collidable;
    EntityType type;

    Math::Vec3 currentSpeed;
};

} // namespace PolandBall

#endif  // ENTITY_H
