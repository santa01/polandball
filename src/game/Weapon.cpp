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

#include <Weapon.h>
// #include <Scene.h>
// #include <EntityFactory.h>
#include <Logger.h>
#include <Quaternion.h>
#include <Mat4.h>
#include <Mat3.h>
#include <stdexcept>
#include <cmath>

namespace PolandBall {

namespace Game {

Weapon::Weapon():
        Weapon(WeaponSlot::SLOT_MEELE) {
}

Weapon::Weapon(WeaponSlot slot):
        targetSlot(slot) {
    this->passive = false;
    this->shearY(-0.15f, 1);
}

void Weapon::setMaxAmmo(int maxAmmo) {
    this->maxAmmo = maxAmmo;
}

int Weapon::getMaxAmmo() const {
    return this->maxAmmo;
}

void Weapon::setAmmo(int ammo) {
    this->ammo = ammo;
}

int Weapon::getAmmo() const {
    return this->ammo;
}

void Weapon::setGroupingAngle(float groupingAngle) {
    this->groupingAngle = groupingAngle;
}

float Weapon::getGroupingAngle() const {
    return this->groupingAngle;
}

void Weapon::setFiringSpeed(float firingSpeed) {
    this->firingSpeed = firingSpeed;
}

float Weapon::getFiringSpeed() const {
    return this->firingSpeed;
}

void Weapon::setTargetSlot(WeaponSlot slot) {
    this->targetSlot = slot;
}

WeaponSlot Weapon::getTargetSlot() const {
    return this->targetSlot;
}

void Weapon::setState(WeaponState state) {
    this->state = state;
}

WeaponState Weapon::getState() const {
    return this->state;
}

Math::Vec3 Weapon::getTargetDirection() const {
    return this->getRight();
}

void Weapon::aimAt(const Math::Vec3& position) {
    if (position == Math::Vec3::ZERO) {
        throw std::invalid_argument(Graphene::LogFormat("Target cannot be of zero length"));
    }

    Math::Vec3 oldTarget(this->getTargetDirection());
    Math::Vec3 newTarget(position - this->getPosition());
    newTarget.normalize();

    float pi = static_cast<float>(M_PI);
    float angle = 180.0f * acosf(oldTarget.dot(newTarget)) / pi;

    Math::Vec3 axis = oldTarget.cross(newTarget);
    this->rotate(axis, angle);

    float rotationAngle = this->getRotationAngles().get(Math::Vec3::Z);
    float shear = (cosf(rotationAngle * pi / 180.0f) < 0.0f) ? 1.0f : 0.0f;
    this->shearX(shear, 2);

    Math::Vec3 targetDirection(this->getTargetDirection());
    float targetSignCorrection = (targetDirection.get(Math::Vec3::X) < 0.0f) ? -1.0f : 1.0f;
    this->shearY(-0.15f * targetSignCorrection, 1);
}

void Weapon::fire() {
    this->firing = true;
}

// void Weapon::onCollision(const std::shared_ptr<BaseEntity>& another, Collider::CollideSide side) {
//     if (this->state == WeaponState::THROWN) {
//         if (side == Collider::CollideSide::SIDE_BOTTOM && another->isCollidable()) {
//             this->setSpeed(Math::Vec3::ZERO);
//             this->state = WeaponState::AVAILABLE;
//         }
//     }
// }

void Weapon::animate(float frameTime) {
    Math::Vec3 targetDirection(this->getTargetDirection());
    float targetSignCorrection = (targetDirection.get(Math::Vec3::X) < 0.0f) ? -1.0f : 1.0f;

    if (this->state == WeaponState::AVAILABLE) {
        this->shearY(sinf(this->bounce) / 13.33f - 0.075f * targetSignCorrection, 1);
        this->bounce += frameTime * 6.0f;
    } else {
        this->shearY(-0.15f * targetSignCorrection, 1);
        this->bounce = 0.0f;
    }

    if (this->state == WeaponState::PICKED) {
        if (this->firing) {
            if (this->relaxTime == 0.0f && this->ammo > 0) {
                float shotAngle = (rand() / static_cast<float>(RAND_MAX)) * this->groupingAngle;
                float signCorrection = (rand() % 2 == 0) ? 1.0f : -1.0f;

                Math::Vec3 position(this->getPosition());
                Math::Vec3 shotTarget(targetDirection);
                shotTarget.normalize();
                shotTarget *= 10.0f;

                float pi = static_cast<float>(M_PI);
                Math::Quaternion q(Math::Vec3::UNIT_Z, shotAngle * signCorrection * pi / 180.0f);
                shotTarget = q.extractMat4().extractMat3() * shotTarget;

                // auto parent = std::shared_ptr<Scene>(this->scene);
                // parent->addEntity(EntityFactory::getInstance().createTrace(position, position + shotTarget));
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
