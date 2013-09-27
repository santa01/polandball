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

#ifndef WEAPON_H
#define WEAPON_H

#include "Entity.h"
#include "SpriteEntity.h"
#include "Collider.h"

#include <Vec3.h>
#include <memory>

namespace PolandBall {

namespace Game {

class Weapon: public SpriteEntity {
public:
    enum WeaponSlot {
        SLOT_MEELE,
        SLOT_SECONDARY,
        SLOT_PRIMARY
    };

    enum WeaponState {
        STATE_PICKED,
        STATE_THROWN,
        STATE_AVAILABLE
    };

    Weapon():
            Weapon(SLOT_PRIMARY) {
    }

    Weapon(WeaponSlot slot);

    void setMaxAmmo(int maxAmmo) {
        this->maxAmmo = maxAmmo;
    }

    int getMaxAmmo() const {
        return this->maxAmmo;
    }

    void setGroupingAngle(float groupingAngle) {
        this->groupingAngle = groupingAngle;
    }

    float getGroupingAngle() const {
        return this->groupingAngle;
    }

    void setFiringSpeed(float firingSpeed) {
        this->firingSpeed = firingSpeed;
    }

    float getFiringSpeed() const {
        return this->firingSpeed;
    }

    void setAmmo(int ammo) {
        this->ammo = ammo;
    }

    int getAmmo() const {
        return this->ammo;
    }

    void setTargetSlot(WeaponSlot slot) {
        this->targetSlot = slot;
    }

    WeaponSlot getTargetSlot() const {
        return this->targetSlot;
    }

    void setState(WeaponState state) {
        this->state = state;
    }

    WeaponState getState() const {
        return this->state;
    }

    void fire() {
        this->firing = true;
    }

    void aimAt(const Math::Vec3& target);

private:
    void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {
        if (this->state == WeaponState::STATE_THROWN) {
            if (side == Collider::CollideSide::SIDE_BOTTOM && another->isCollidable()) {
                this->setSpeed(Math::Vec3::ZERO);
                this->state = WeaponState::STATE_AVAILABLE;
            }
        }
    }

    void animate(float frameTime);

    Math::Vec3 target;
    bool firing;

    float groupingAngle;
    float firingSpeed;  // Shots per second
    int maxAmmo;
    int ammo;

    WeaponSlot targetSlot;
    WeaponState state;
    float viewAngle;
    float bounce;
    float relaxTime;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // WEAPON_H
