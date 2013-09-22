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

#include "Weapon.h"

namespace PolandBall {

namespace Game {

Weapon::Weapon(WeaponSlot slot):
        target(Math::Vec3::UNIT_X) {
    this->targetSlot = slot;
    this->state = STATE_AVAILABLE;

    this->viewAngle = 0.0f;
    this->bounce = 0.0f;
    this->groupingAngle = 1.0f;
    this->firingSpeed = 3.0f;
    this->maxAmmo = 100;
    this->ammo = this->maxAmmo;

    this->passive = false;
    this->type = Entity::EntityType::TYPE_WEAPON;
    sprite->shearX(0.0f, 2);
    sprite->shearY(-0.15f, 1);
}

void Weapon::aimAt(const Math::Vec3& target) {
    if (target == Math::Vec3::ZERO) {
        return;
    }

    Math::Vec3 newTarget(target);
    newTarget.normalize();

    if (newTarget == this->target) {
        return;
    }

    float deltaAngle = acosf(this->target.dot(newTarget)) * 180.0f / M_PI;
    if (isnan(deltaAngle)) {
        return;
    }

    Math::Vec3 normal = this->target.cross(newTarget);
    float signCorrection = (normal.get(Math::Vec3::Z) < 0.0f) ? -1.0f : 1.0f;
    float targetSignCorrection = (newTarget.get(Math::Vec3::X) < 0.0f) ? -1.0f : 1.0f;

    float newAngle = this->viewAngle + deltaAngle * signCorrection;
    float shear = (cosf(newAngle * M_PI / 180.0f) < 0.0f) ? 1.0f : 0.0f;
    this->viewAngle = newAngle;

    this->roll(newAngle);
    this->sprite->shearX(shear, 2);
    this->sprite->shearY(-0.15f * targetSignCorrection, 1);

    this->target = newTarget;
}

void Weapon::shoot() {
}

}  // namespace Game

}  // namespace PolandBall
