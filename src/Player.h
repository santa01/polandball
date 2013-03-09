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

#ifndef PLAYER_H
#define	PLAYER_H

#include "Entity.h"
#include "Collider.h"
#include "Vec3.h"

#include <cmath>
#include <memory>

namespace PolandBall {

class Player: public Entity {
public:
    Player():
            Entity(Entity::TYPE_DYNAMIC),
            target(Math::Vec3::UNIT_X) {
        this->maxMoveSpeed = 8.0f;
        this->maxJumpSpeed = 15.0f;
        this->maxJumpTime = 0.15f;

        this->jumpTime = 0.0f;
        this->jumpWasReleased = true;
    }

    void collideWith(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {
        switch (side) {
            case Collider::CollideSide::SIDE_TOP:
                this->jumpTime = 100.0f;  // Just god damn big value, bigger than the maximum jump time
                break;

            case Collider::CollideSide::SIDE_BOTTOM:
                this->jumpTime = 0.0f;
                break;

            default:
                break;
        }
    }

    const Math::Vec3& getTarget() const {
        return this->target;
    }

    float getMaxMoveSpeed() const {
        return this->maxMoveSpeed;
    }

    void setMaxMoveSpeed(float maxMoveSpeed) {
        this->maxMoveSpeed = maxMoveSpeed;
    }

    float getMaxJumpSpeed() const {
        return this->maxJumpSpeed;
    }

    void setMaxJumpSpeed(float maxJumpSpeed) {
        this->maxJumpSpeed = maxJumpSpeed;
    }

    float getMaxJumpTime() const {
        return this->maxJumpTime;
    }

    void setMaxJumpTime(float maxJumpTime) {
        this->maxJumpTime = maxJumpTime;
    }

    void moveRight(float frameTime);
    void moveLeft(float frameTime);
    void slowDown(float frameTime);

    void jump(float frameTime);
    void breakJump() {
        this->jumpWasReleased = (this->jumpTime == 0.0f) ? true : false;
    }

    void aimAt(const Math::Vec3& target);

private:
    Math::Vec3 target;  // Window space!

    float maxMoveSpeed;
    float maxJumpSpeed;
    float maxJumpTime;

    float jumpTime;
    bool jumpWasReleased;
};

}  // namespace PolandBall

#endif  // PLAYER_H
