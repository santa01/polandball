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
#include "Scene.h"
#include "EntityFactory.h"

#include <Quaternion.h>
#include <cmath>

namespace PolandBall {

namespace Game {

Weapon::Weapon(WeaponSlot slot):
        target(Math::Vec3::UNIT_X) {
    this->targetSlot = slot;
    this->state = WeaponState::STATE_AVAILABLE;

    this->viewAngle = 0.0f;
    this->bounce = 0.0f;
    this->relaxTime = 0.0f;
    this->firing = false;

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
    if (newTarget.normalize() == this->target.normalize()) {
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

    this->target = target;
}

void Weapon::animate(float frameTime) {
    float targetSignCorrection = (this->target.get(Math::Vec3::X) < 0.0f) ? -1.0f : 1.0f;

    if (this->state == WeaponState::STATE_AVAILABLE) {
        this->sprite->shearY(sinf(this->bounce) / 13.33f - 0.075f * targetSignCorrection, 1);
        this->bounce += frameTime * 6.0f;
    } else {
        this->sprite->shearY(-0.15f * targetSignCorrection, 1);
        this->bounce = 0.0f;
    }

    if (this->state == WeaponState::STATE_PICKED) {
        if (this->firing) {
            if (this->relaxTime == 0.0f && this->ammo > 0) {
                float shotAngle = (rand() / static_cast<float>(RAND_MAX)) * this->groupingAngle;
                float signCorrection = (rand() % 2 == 0) ? 1.0f : -1.0f;

                Math::Vec3 position(this->getPosition());
                Math::Vec3 shotTarget(this->target);
                shotTarget.normalize();
                shotTarget *= 10.0f;

                Math::Quaternion q(Math::Vec3::UNIT_Z, shotAngle * signCorrection * M_PI / 180.0f);
                shotTarget = q.extractMat4().extractMat3() * shotTarget;

                auto parent = std::shared_ptr<Scene>(this->scene);
                parent->addEntity(EntityFactory::getInstance().createTrace(position, position + shotTarget));
                this->ammo--;
            }

            this->relaxTime += frameTime;
        } else if (this->relaxTime > 0.0f) {
            this->relaxTime += frameTime;
        }

        if (this->relaxTime >= 1.0f / this->firingSpeed) {
            this->relaxTime = 0.0f;
            this->firing = false;
        }
    }
}

}  // namespace Game

}  // namespace PolandBall
