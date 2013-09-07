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

#include <Vec3.h>
#include <cmath>

namespace PolandBall {

namespace Game {

class Weapon: public Entity {
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
            Weapon(SLOT_MEELE) {
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

    void aimAt(const Math::Vec3& target);
    void shoot() {
    }

private:
    void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {
        if (this->state == STATE_THROWN) {
            if (side == Collider::CollideSide::SIDE_BOTTOM &&
                    (another->getType() == Entity::EntityType::TYPE_SOLID ||
                     another->getType() == Entity::EntityType::TYPE_CLIP)) {
                this->setSpeed(Math::Vec3::ZERO);
                this->state = STATE_AVAILABLE;
            }
        }
    }

    void animate(float frameTime) {
        float targetSignCorrection = (this->target.get(Math::Vec3::X) < 0.0f) ? -1.0f : 1.0f;

        if (this->state == STATE_AVAILABLE) {
            this->shearY(sinf(this->bounce) / 13.33f - 0.075f * targetSignCorrection, 1);
            this->bounce += frameTime * 6.0f;
        } else {
            this->shearY(-0.15f * targetSignCorrection, 1);
            this->bounce = 0.0f;
        }
    }

    Math::Vec3 target;

    float groupingAngle;
    float firingSpeed;  // Shots per second
    int maxAmmo;
    int ammo;

    WeaponSlot targetSlot;
    WeaponState state;
    float viewAngle;
    float bounce;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // WEAPON_H
