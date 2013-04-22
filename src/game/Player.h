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
#define PLAYER_H

#include "Entity.h"
#include "Collider.h"
#include "Vec3.h"
#include "Weapon.h"

#include <cmath>
#include <memory>
#include <array>

namespace PolandBall {

class Player: public Entity {
public:
    Player();

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

    std::shared_ptr<Weapon>& getWeapon(Weapon::WeaponSlot slot) {
        return this->weapons[slot];
    }

    void pickWeapon(const std::shared_ptr<Weapon>& weapon);
    void activateSlot(Weapon::WeaponSlot slot);

    void moveRight(float frameTime);
    void moveLeft(float frameTime);
    void slowDown(float frameTime);

    void jump(float frameTime);
    void breakJump() {
        this->jumpWasReleased = (this->jumpTime == 0.0f) ? true : false;
    }

    void aimAt(const Math::Vec3& target);
    void shoot() {
        if (this->activeSlot != -1) {
            this->weapons[this->activeSlot]->shoot();
        }
    }

    void dropWeapon();
    void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side);

private:
    std::array<std::shared_ptr<Weapon>, 3> weapons;
    Math::Vec3 target;

    float maxMoveSpeed;
    float maxJumpSpeed;
    float maxJumpTime;

    float jumpTime;
    float viewAngle;
    bool jumpWasReleased;

    int activeSlot;
    int weaponHandle;
};

}  // namespace PolandBall

#endif  // PLAYER_H
