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
#include "Vec3.h"

namespace PolandBall {

class Weapon: public Entity {
public:
    enum WeaponSlot {
        SLOT_MEELE = 0,
        SLOT_PRIMARY = 1,
        SLOT_SECONDARY = 2
    };

    enum WeaponState {
        STATE_PICKED = 0,
        STATE_THROWN = 1,
        STATE_AVAILABLE = 2
    };

    Weapon():
            target(Math::Vec3::UNIT_X) {
        this->targetSlot = SLOT_MEELE;
        this->initialize();
    }

    Weapon(WeaponSlot slot):
            target(Math::Vec3::UNIT_X) {
        this->targetSlot = slot;
        this->initialize();
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

    void onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {
        if (this->state == STATE_THROWN) {
            if (another->getType() == Entity::EntityType::TYPE_SOLID &&
                    side == Collider::CollideSide::SIDE_BOTTOM) {
                this->setSpeed(Math::Vec3::ZERO);
                this->state = STATE_AVAILABLE;
            }
        }
    }

private:
    void initialize() {
        this->type = Entity::EntityType::TYPE_WEAPON;
        this->state = STATE_AVAILABLE;
    }

    Math::Vec3 target;

    WeaponSlot targetSlot;
    WeaponState state;
};

}  // namespace PolandBall

#endif  // WEAPON_H
