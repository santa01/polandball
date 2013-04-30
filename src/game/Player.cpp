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

#include "Player.h"

#include <cfloat>

namespace PolandBall {

namespace Game {

Player::Player():
        target(Math::Vec3::UNIT_X) {
    this->maxMoveSpeed = 8.0f;
    this->maxJumpSpeed = 10.0f;
    this->maxJumpTime = 0.4f;

    this->jumpTime = 0.0f;
    this->viewAngle = 0.0f;
    this->activeSlot = -1;
    this->weaponHandle = -1;
    this->state = STATE_IDLE;

    this->type = Entity::EntityType::TYPE_PLAYER;
    this->shearX(0.0f, 2);
}

void Player::pickWeapon(const std::shared_ptr<Weapon>& weapon) {
    Weapon::WeaponSlot targetSlot = weapon->getTargetSlot();

    if (this->getWeapon(targetSlot) == nullptr) {
        this->weapons[targetSlot] = weapon;
        this->weapons[targetSlot]->setState(Weapon::WeaponState::STATE_PICKED);

        if (targetSlot > this->activeSlot || this->activeSlot == -1) {
            this->activateSlot(targetSlot);
        }
    }
}

void Player::activateSlot(Weapon::WeaponSlot slot) {
    if (this->weapons[slot] == nullptr) {
        return;
    }

    this->weapons[slot]->aimAt(this->target);  // Sync with player
    this->activeSlot = slot;

    if (this->weaponHandle != -1) {
        this->positionChanged.disconnect(this->weaponHandle);
    }

    this->weaponHandle = this->positionChanged.connect(
            std::bind(static_cast<void(Weapon::*)(const Math::Vec3&)>(&Weapon::setPosition),
            this->weapons[slot], std::placeholders::_1));
}

void Player::dropWeapon() {
    if (this->activeSlot == -1) {
        return;
    }

    std::shared_ptr<Weapon> weapon = this->weapons[this->activeSlot];
    this->weapons[this->activeSlot] = nullptr;

    for (this->activeSlot = this->weapons.size() - 1; this->activeSlot > -1; this->activeSlot--) {
        if (this->weapons[this->activeSlot] != nullptr) {
            this->activateSlot(static_cast<Weapon::WeaponSlot>(this->activeSlot));  // TODO: Remove cast
            break;
        }
    }

    if (this->activeSlot < 0) {
        this->positionChanged.disconnect(this->weaponHandle);
        this->weaponHandle = -1;
        this->activeSlot = -1;
    }

    float viewAngle = acosf(this->target.dot(Math::Vec3::UNIT_X)) * 180.0f / M_PI;
    float signCorrection = (cosf(viewAngle * M_PI / 180.0f) < 0.0f) ? -1.0f : 1.0f;
    Math::Vec3 dropAcceleration((Math::Vec3::UNIT_X * signCorrection + Math::Vec3::UNIT_Y) * 7.0f);

    weapon->aimAt(Math::Vec3::UNIT_X * signCorrection);
    weapon->setState(Weapon::WeaponState::STATE_THROWN);
    weapon->setPosition(weapon->getPosition() + Math::Vec3::UNIT_Y * 0.5f);  // Don't collide from bottom

    float hotizontalSpeed = this->getSpeed().get(Math::Vec3::X);
    float verticalSpeed = this->getSpeed().get(Math::Vec3::Y);
    weapon->setSpeed(Math::Vec3(hotizontalSpeed, (verticalSpeed > 0.0f) ? verticalSpeed : 0.0f, 0.0f));
    weapon->accelerateBy(dropAcceleration);
}

void Player::aimAt(const Math::Vec3& target) {
    if (target == Math::Vec3::ZERO) {
        return;
    }

    Math::Vec3 newTarget(target);
    if (newTarget.normalize() == this->target) {
        return;
    }

    float deltaAngle = acosf(this->target.dot(newTarget)) * 180.0f / M_PI;
    if (isnan(deltaAngle)) {
        return;
    }

    Math::Vec3 normal = this->target.cross(newTarget);
    float signCorrection = (normal.get(Math::Vec3::Z) < 0) ? -1.0f : 1.0f;

    float newAngle = this->viewAngle + deltaAngle * signCorrection;
    float shear = (cosf(newAngle * M_PI / 180.0f) < 0.0f) ? 1.0f : 0.0f;
    this->viewAngle = newAngle;

    this->roll(newAngle);
    this->shearX(shear, 2);

    if (this->activeSlot != -1) {
        this->weapons[this->activeSlot]->aimAt(newTarget);
    }

    this->target = newTarget;
}

void Player::onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {
    if (another->getType() == Entity::EntityType::TYPE_WEAPON) {
        std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(another);
        if (weapon->getState() == Weapon::WeaponState::STATE_AVAILABLE) {
            this->pickWeapon(weapon);
        }
        return;  // Don't break the jump
    }

    switch (side) {
        case Collider::CollideSide::SIDE_TOP:
            this->jumpTime = FLT_MAX;  // Just god damn big value, bigger than the maximum jump time
            break;

        case Collider::CollideSide::SIDE_BOTTOM:
            this->jumpTime = 0.0f;
            break;

        default:
            break;
    }
}

void Player::animate(float frameTime) {
    if (this->state & STATE_JUMP) {
        float playerUpSpeed = this->currentSpeed.get(Math::Vec3::Y);

        if (this->jumpTime < this->maxJumpTime && playerUpSpeed >= 0.0f) {
            if (playerUpSpeed < this->maxJumpSpeed) {
                this->accelerateBy(Math::Vec3::UNIT_Y * (this->maxJumpSpeed - playerUpSpeed));
            }
            this->jumpTime += frameTime;
        }

        this->state &= ~STATE_JUMP;
    } else {
        this->jumpTime = FLT_MAX;
    }

    float playerMoveSpeed = this->currentSpeed.get(Math::Vec3::X);
    float moveSpeedDelta = this->maxMoveSpeed * frameTime * 10.0f;
    Math::Vec3 moveAcceleration;

    if (this->state & STATE_LEFT_STEP && !(this->state & STATE_RIGHT_STEP)) {
        // We press LEFT and we already moving LEFT
        if (playerMoveSpeed < 0.0f && fabs(playerMoveSpeed) + moveSpeedDelta > this->maxMoveSpeed) {
            moveAcceleration = -Math::Vec3::UNIT_X * (this->maxMoveSpeed - fabs(playerMoveSpeed));
        } else {
            moveAcceleration = -Math::Vec3::UNIT_X * moveSpeedDelta;
        }

        this->accelerateBy(moveAcceleration);
        this->state &= ~STATE_LEFT_STEP;
    } else if (this->state & STATE_RIGHT_STEP && !(this->state & STATE_LEFT_STEP)) {
        // We press RIGHT and we already moving RIGHT
        if (playerMoveSpeed > 0.0f && fabs(playerMoveSpeed) + moveSpeedDelta > this->maxMoveSpeed) {
            moveAcceleration = Math::Vec3::UNIT_X * (this->maxMoveSpeed - fabs(playerMoveSpeed));
        } else {
            moveAcceleration = Math::Vec3::UNIT_X * moveSpeedDelta;
        }

        this->accelerateBy(moveAcceleration);
        this->state &= ~STATE_RIGHT_STEP;
    } else {
        if (this->currentSpeed.get(Math::Vec3::Y) == 0.0f) {
            moveSpeedDelta /= 2.0f;  // Slow down twice slower than accelerate, looks good
            if ((playerMoveSpeed > 0.0f && playerMoveSpeed - moveSpeedDelta < 0.0f) ||
                    (playerMoveSpeed < 0.0f && playerMoveSpeed + moveSpeedDelta > 0.0f)) {
                moveAcceleration = Math::Vec3::UNIT_X * fabs(playerMoveSpeed);
            } else {
                moveAcceleration = Math::Vec3::UNIT_X * moveSpeedDelta;
            }

            float signCorrection = (playerMoveSpeed < 0.0f) ? 1.0f : -1.0f;
            this->accelerateBy(moveAcceleration * signCorrection);
        }

        this->state &= ~(STATE_RIGHT_STEP | STATE_LEFT_STEP);
    }
}

}  // namespace Game

}  // namespace PolandBall
