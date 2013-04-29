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
#include "Sprite.h"
#include "RenderEffect.h"
#include "Texture.h"
#include "Vec3.h"
#include "Movable.h"
#include "Scalable.h"
#include "Transformable.h"
#include "ResourceManager.h"
#include "Signals.h"

#include <memory>
#include <utility>

namespace PolandBall {

namespace Game {

// NOTE: IRotateble only for animation purposes
class Entity: public Common::Movable, public Common::Rotatable, public Common::Scalable, public Common::Transformable {
public:
    enum EntityType {
        // Fixed
        TYPE_SOLID = 0,     // Visible, collidable
        TYPE_CLIP = 1,      // Invisible, collidable
        TYPE_PASSABLE = 3,  // Visible, non-collidable
        // Non-fixed
        TYPE_PLAYER = 4,    // Player entity
        TYPE_WEAPON = 5     // Weapon entity
    };

    Entity():
            collider(new Collider()),
            sprite(new Opengl::Sprite()) {
        this->type = TYPE_SOLID;
    }

    Entity(EntityType type):
            collider(new Collider()),
            sprite(new Opengl::Sprite()) {
        this->type = type;
    }

    virtual ~Entity() {}

    using Movable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->sprite->setPosition(position);
        this->collider->setPosition(position);
        this->positionChanged(position);  // Emit signal
    }

    Math::Vec3 getPosition() const {
        return this->sprite->getPosition();  // Positions are syncronized
    }

    float getXAngle() const {
        return this->sprite->getXAngle();
    }

    float getYAngle() const {
        return this->sprite->getYAngle();
    }

    float getZAngle() const {
        return this->sprite->getZAngle();
    }

    void rotate(const Math::Vec3& vector, float angle) {
        this->sprite->rotate(vector, angle);
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
    }

    void replicateY(float factor) {
        this->sprite->replicateY(factor);
    }

    void replicateZ(float factor) {
        this->sprite->replicateZ(factor);
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

    void shearX(int slice, int totalSlices) {
        this->sprite->shearX(slice, totalSlices);
    }

    void shearY(int slice, int totalSlices) {
        this->sprite->shearY(slice, totalSlices);
    }

    void shearZ(int slice, int totalSlices) {
        this->sprite->shearZ(slice, totalSlices);
    }

    void getXShearFactor(int& slice, int& totalSlices) const {
        this->sprite->getXShearFactor(slice, totalSlices);
    }

    void getYShearFactor(int& slice, int& totalSlices) const {
        this->sprite->getYShearFactor(slice, totalSlices);
    }

    void getZShearFactor(int& slice, int& totalSlices) const {
        this->sprite->getZShearFactor(slice, totalSlices);
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

    std::shared_ptr<Opengl::Sprite>& getSprite() {
        return this->sprite;
    }

    void setSprite(const std::shared_ptr<Opengl::Sprite>& sprite) {
        if (sprite != nullptr) {
            this->sprite = sprite;
        }
    }

    std::shared_ptr<Opengl::RenderEffect>& getEffect() {
        return this->sprite->getEffect();
    }

    void setEffect(std::shared_ptr<Opengl::RenderEffect>& effect) {
        this->sprite->setEffect(effect);
    }

    std::shared_ptr<Opengl::Texture>& getTexture() {
        return this->sprite->getTexture();
    }

    void setTexture(std::shared_ptr<Opengl::Texture>& texture) {
        this->sprite->setTexture(texture);
    }

    void render() {
        this->sprite->render();
    }

    Collider::CollideSide collides(const std::shared_ptr<Entity>& another) {
        return this->collider->collides(another->getCollider());
    }

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

    virtual void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {}
    virtual void animate(float frameTime) {}

    Signals::Signal<Math::Vec3> positionChanged;

protected:
    std::unique_ptr<Collider> collider;
    std::shared_ptr<Opengl::Sprite> sprite;

    Math::Vec3 currentSpeed;
    EntityType type;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // ENTITY_H
