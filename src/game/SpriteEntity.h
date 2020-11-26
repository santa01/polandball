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

#include <Entity.h>
#include <Shearable.h>
#include <Replicable.h>
// #include <Collider.h>
#include <Vec3.h>
#include <memory>
#include <utility>

namespace PolandBall {

namespace Game {

enum class EntityType { GENERIC, PLAYER, WEAPON, PACK };

class SpriteEntity: public Graphene::Entity, public Shearable, public Replicable {
public:
    SpriteEntity();
    virtual ~SpriteEntity() = default;

    EntityType getType() const;

    const Math::Vec3& getSpeed() const;
    void setSpeed(const Math::Vec3& speed);
    void accelerateBy(const Math::Vec3& acceleration);

    bool isPassive() const;
    void setPassive(bool passive);

    bool isCollidable() const;
    void setCollidable(bool collidable);

    void destroy();

    // const std::shared_ptr<Collider>& getCollider() const;
    // void setCollider(const std::shared_ptr<Collider>& collider);

protected:
    // virtual void onCollision(const std::shared_ptr<SpriteEntity>& another, Collider::CollideSide side) { };
    virtual void animate(float frameTime) { };

    EntityType type = EntityType::GENERIC;
    Math::Vec3 speed;

    bool passive = true;
    bool collidable = true;
    bool destroyed = false;

    // std::shared_ptr<Collider> collider;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // SPRITEENTITY_H
