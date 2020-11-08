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
#define ENTITY_H

#include "Collider.h"
#include "Primitive.h"
#include "RenderEffect.h"
#include "Texture.h"
#include "Movable.h"
#include "Scalable.h"
#include "Transformable.h"

#include <Vec3.h>
#include <Signals.h>
#include <memory>
#include <utility>

namespace PolandBall {

namespace Game {

class Entity: public Common::Movable, public Common::Rotatable, public Common::Scalable {
public:
    enum EntityType {
        TYPE_GENERIC,  // First to render
        TYPE_PLAYER,
        TYPE_WEAPON,
        TYPE_PACK,
        TYPE_WIDGET    // Last to render
    };

    Entity():
            collider(new Collider()) {
        this->type = EntityType::TYPE_GENERIC;
        this->visible = true;
        this->passive = true;
        this->collidable = true;
        this->destroyed = false;
    }

    virtual ~Entity() {}

    using Movable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->primitive->setPosition(this->origin + position);
        this->collider->setPosition(this->origin + position);
        this->positionChanged(position);
    }

    Math::Vec3 getPosition() const {
        return this->primitive->getPosition();
    }

    float getXAngle() const {
        return this->primitive->getXAngle();
    }

    float getYAngle() const {
        return this->primitive->getYAngle();
    }

    float getZAngle() const {
        return this->primitive->getZAngle();
    }

    void rotate(const Math::Vec3& vector, float angle) {
        this->primitive->rotate(vector, angle);
    }

    void scaleX(float factor) {
        this->primitive->scaleX(factor);
        this->collider->scaleX(factor);
    }

    void scaleY(float factor) {
        this->primitive->scaleY(factor);
        this->collider->scaleY(factor);
    }

    void scaleZ(float factor) {
        this->primitive->scaleZ(factor);
        this->collider->scaleZ(factor);
    }

    float getXFactor() const {
        return this->primitive->getXFactor();
    }

    float getYFactor() const {
        return this->primitive->getYFactor();
    }

    float getZFactor() const {
        return this->primitive->getZFactor();
    }

    EntityType getType() const {
        return this->type;
    }

    const Math::Vec3& getOrigin() const {
        return this->origin;
    }

    void setOrigin(float x, float y, float z) {
        this->setOrigin(Math::Vec3(x, y, z));
    }

    void setOrigin(const Math::Vec3& origin) {
        this->origin = origin;
        this->setPosition(this->getPosition());
    }

    const std::unique_ptr<Collider>& getCollider() const {
        return this->collider;
    }

    const std::shared_ptr<Opengl::Primitive>& getPrimitive() const {
        return this->primitive;
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

    bool isVisible() const {
        return this->visible;
    }

    void setVisible(bool visible) {
        this->visible = visible;
    }

    bool isPassive() const {
        return this->passive;
    }

    void setPassive(bool passive) {
        this->passive = passive;
    }

    bool isCollidable() const {
        return this->collidable;
    }

    void setCollidable(bool collidable) {
        this->collidable = collidable;
    }

    void destroy() {
        this->destroyed = true;
    }

    Signals::Signal<Math::Vec3> positionChanged;

protected:
    friend class Scene;

    virtual void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) = 0;
    virtual void animate(float frameTime) = 0;

    std::unique_ptr<Collider> collider;
    std::shared_ptr<Opengl::Primitive> primitive;
    std::weak_ptr<class Scene> scene;

    Math::Vec3 currentSpeed;
    Math::Vec3 origin;

    EntityType type;
    bool visible;
    bool passive;
    bool collidable;
    bool destroyed;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // ENTITY_H
