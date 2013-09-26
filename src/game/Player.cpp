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
#include "Pack.h"

#include <cfloat>

namespace PolandBall {

namespace Game {

Player::Player():
        target(Math::Vec3::UNIT_X) {
    this->maxMoveSpeed = 8.0f;
    this->maxJumpSpeed = 10.0f;
    this->maxJumpTime = 0.5f;
    this->minDropTime = 0.5f;
    this->maxHealth = 100;
    this->maxArmor = 100;

    this->jumpTime = 0.0f;
    this->dropTime = 0.0f;
    this->viewAngle = 0.0f;
    this->health = this->maxHealth;
    this->armor = 0;

    this->activeSlot = -1;
    this->weaponHandle = -1;
    this->state = PlayerState::STATE_IDLE;
    this->previousState = this->state;

    this->passive = false;
    this->type = Entity::EntityType::TYPE_PLAYER;
    this->sprite->shearX(0.0f, 2);
}

void Player::pickWeapon(const std::shared_ptr<Weapon>& weapon) {
    Weapon::WeaponSlot targetSlot = weapon->getTargetSlot();

    if (this->getWeapon(targetSlot) == nullptr) {
        this->weapons[targetSlot] = weapon;
        this->weapons[targetSlot]->setState(Weapon::WeaponState::STATE_PICKED);

        auto weaponSprite = std::dynamic_pointer_cast<Opengl::Sprite>(this->weapons[targetSlot]->getPrimitive());
        weaponSprite->shearX(0.0f, 2);

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
    this->positionChanged(this->getPosition());
}

void Player::aimAt(const Math::Vec3& target) {
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
    float signCorrection = (normal.get(Math::Vec3::Z) < 0) ? -1.0f : 1.0f;

    float newAngle = this->viewAngle + deltaAngle * signCorrection;
    float shear = (cosf(newAngle * M_PI / 180.0f) < 0.0f) ? 1.0f : 0.0f;
    this->viewAngle = newAngle;

    this->roll(newAngle);
    this->sprite->shearX(shear, 2);

    if (this->activeSlot != -1) {
        this->weapons[this->activeSlot]->aimAt(newTarget);
    }

    this->target = target;
}

void Player::onCollision(const std::shared_ptr<Entity>& another, Collider::CollideSide side) {
    if (another->getType() == Entity::EntityType::TYPE_WEAPON) {
        auto weapon = std::dynamic_pointer_cast<Weapon>(another);
        if (weapon->getState() == Weapon::WeaponState::STATE_AVAILABLE) {
            this->pickWeapon(weapon);
            return;
        }
    }

    if (another->getType() == Entity::EntityType::TYPE_PACK) {
        auto pack = std::dynamic_pointer_cast<Pack>(another);
        Weapon::WeaponSlot slot = Weapon::WeaponSlot::SLOT_MEELE;
        int value = pack->getValue();
        bool packStaysAlive = true;

        switch (pack->getPayloadType()) {
            case Pack::PayloadType::TYPE_ARMOR:
                packStaysAlive = (this->armor == this->maxArmor);
                this->armor = (this->armor + value > this->maxArmor) ? this->maxArmor : this->armor + value;
                break;

            case Pack::PayloadType::TYPE_HEALTH:
                packStaysAlive = (this->health == this->maxHealth);
                this->health = (this->health + value > this->maxHealth) ? this->maxHealth : this->health + value;
                break;

            case Pack::PayloadType::TYPE_PRIMARY_AMMO:
                slot = Weapon::WeaponSlot::SLOT_PRIMARY;
                break;

            case Pack::PayloadType::TYPE_SECONDARY_AMMO:
                slot = Weapon::WeaponSlot::SLOT_SECONDARY;
                break;
        }

        if (slot != Weapon::WeaponSlot::SLOT_MEELE && this->weapons[slot] != nullptr) {
            int maxAmmo = this->weapons[slot]->getMaxAmmo();
            int ammo = this->weapons[slot]->getAmmo();

            packStaysAlive = (ammo == maxAmmo);
            this->weapons[slot]->setAmmo((ammo + value > maxAmmo) ? maxAmmo : ammo + value);
        }

        if (!packStaysAlive) {
            pack->destroy();
        }
        return;
    }

    switch (side) {
        case Collider::CollideSide::SIDE_TOP:
            this->jumpTime = FLT_MAX;
            break;

        case Collider::CollideSide::SIDE_BOTTOM:
            if (this->currentSpeed.get(Math::Vec3::Y) < 0.0f) {
                this->jumpTime = 0.0f;
            }
            break;

        default:
            break;
    }
}

void Player::animate(float frameTime) {
    float playerMoveSpeed = this->currentSpeed.get(Math::Vec3::X);
    float playerJumpSpeed = this->currentSpeed.get(Math::Vec3::Y);

    if ((this->state & PlayerState::STATE_JUMP) &&
            !(this->jumpTime == 0.0f && (this->previousState & PlayerState::STATE_JUMP))) {
        if (this->jumpTime < this->maxJumpTime && playerJumpSpeed >= 0.0f) {
            if (playerJumpSpeed < this->maxJumpSpeed) {
                this->accelerateBy(Math::Vec3::UNIT_Y * (this->maxJumpSpeed - playerJumpSpeed));
            }
            this->jumpTime += frameTime;
        }
    } else {
        this->jumpTime = FLT_MAX;
    }

    if ((this->state & PlayerState::STATE_DROP_WEAPON)) {
        if (!(this->dropTime != 0.0f && (this->previousState & PlayerState::STATE_DROP_WEAPON))) {
            this->dropWeapon();
        }

        this->dropTime += frameTime;
        if (this->dropTime >= this->minDropTime) {
            this->dropTime = 0.0f;
        }
    } else {
        this->dropTime = 0.0f;
    }

    float moveSpeedDelta = this->maxMoveSpeed * frameTime * 10.0f;
    Math::Vec3 moveAcceleration = Math::Vec3::UNIT_X * moveSpeedDelta;
    float signCorrection = 0.0f;

    if (this->state & PlayerState::STATE_LEFT_STEP && !(this->state & PlayerState::STATE_RIGHT_STEP)) {
        if (playerMoveSpeed < 0.0f && moveSpeedDelta - playerMoveSpeed > this->maxMoveSpeed) {
            moveAcceleration = Math::Vec3::UNIT_X * (this->maxMoveSpeed + playerMoveSpeed);
        }
        signCorrection = -1.0f;
    } else if (this->state & PlayerState::STATE_RIGHT_STEP && !(this->state & PlayerState::STATE_LEFT_STEP)) {
        if (playerMoveSpeed > 0.0f && moveSpeedDelta + playerMoveSpeed > this->maxMoveSpeed) {
            moveAcceleration = Math::Vec3::UNIT_X * (this->maxMoveSpeed - playerMoveSpeed);
        }
        signCorrection = 1.0f;
    } else if (playerJumpSpeed == 0.0f && playerMoveSpeed != 0.0f) {
        if ((playerMoveSpeed > 0.0f && playerMoveSpeed - (moveSpeedDelta * 0.5f) < 0.0f) ||
                (playerMoveSpeed < 0.0f && playerMoveSpeed + (moveSpeedDelta * 0.5f) > 0.0f)) {
            moveAcceleration = Math::Vec3::UNIT_X * fabs(playerMoveSpeed);
        } else {
            moveAcceleration = moveAcceleration * 0.5f;
        }
        signCorrection = (playerMoveSpeed < 0.0f) ? 1.0f : -1.0f;
    }

    this->accelerateBy(moveAcceleration * signCorrection);
    this->previousState = this->state;
    this->state = PlayerState::STATE_IDLE;
}

void Player::dropWeapon() {
    if (this->activeSlot == -1) {
        return;
    }

    auto weapon = this->weapons[this->activeSlot];
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

    float targetSignCorrection = (this->target.get(Math::Vec3::X) < 0.0f) ? -1.0f : 1.0f;
    Math::Vec3 dropAcceleration((Math::Vec3::UNIT_X * targetSignCorrection + Math::Vec3::UNIT_Y) * 7.0f);

    weapon->aimAt(Math::Vec3::UNIT_X * targetSignCorrection);
    weapon->setState(Weapon::WeaponState::STATE_THROWN);
    weapon->setPosition(weapon->getPosition() + Math::Vec3::UNIT_Y * 0.5f);  // Don't collide from bottom

    float hotizontalSpeed = this->getSpeed().get(Math::Vec3::X);
    float verticalSpeed = this->getSpeed().get(Math::Vec3::Y);
    weapon->setSpeed(Math::Vec3(hotizontalSpeed, (verticalSpeed > 0.0f) ? verticalSpeed : 0.0f, 0.0f));
    weapon->accelerateBy(dropAcceleration);
}

}  // namespace Game

}  // namespace PolandBall
