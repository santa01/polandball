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

#include <memory>
#include <array>

namespace PolandBall {

namespace Game {

class Player: public Entity {
public:
    enum PlayerState {
        STATE_IDLE = 1 << 0,
        STATE_LEFT_STEP = 1 << 1,
        STATE_RIGHT_STEP = 1 << 2,
        STATE_JUMP = 1 << 3
    };

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

    int getMaxHealth() const {
        return this->maxHealth;
    }

    void setMaxHealth(int maxHealth) {
        this->maxHealth = maxHealth;
    }

    int getMaxArmor() const {
        return this->maxArmor;
    }

    void setMaxArmor(int maxArmor) {
        this->maxArmor = maxArmor;
    }

    int getState() const {
        return this->state;
    }

    void setState(PlayerState state) {
        this->state |= state;
    }

    int getHealth() const {
        return this->health;
    }

    void setHealth(int health) {
        this->health = health;
    }

    int getArmor() const {
        return this->armor;
    }

    void setArmor(int armor) {
        this->armor = armor;
    }

    std::shared_ptr<Weapon>& getWeapon(Weapon::WeaponSlot slot) {
        return this->weapons[slot];
    }

    void pickWeapon(const std::shared_ptr<Weapon>& weapon);
    void activateSlot(Weapon::WeaponSlot slot);
    void dropWeapon();

    void aimAt(const Math::Vec3& target);
    void shoot() {
        if (this->activeSlot != -1) {
            this->weapons[this->activeSlot]->shoot();
        }
    }

    void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side);
    void animate(float frameTime);

private:
    std::array<std::shared_ptr<Weapon>, 3> weapons;
    Math::Vec3 target;

    float maxMoveSpeed;
    float maxJumpSpeed;
    float maxJumpTime;
    int maxHealth;
    int maxArmor;

    float jumpTime;
    float viewAngle;
    int health;
    int armor;

    /* NOTE: Weapon::activeSlot should be accessible by Scene. Still exposing
     * Weapon::activeSlot through the Weapon inteface by any know method requires
     * special attention to be payed for activeSlot value of -1. Scene is marked
     * as friend for now. */
    friend class Scene;
    int activeSlot;
    int weaponHandle;
    int state;
    int previousState;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // PLAYER_H
