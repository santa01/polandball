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

#include <Player.h>
// #include <Pack.h>
#include <Logger.h>
#include <stdexcept>
#include <cmath>

namespace PolandBall {

namespace Game {

Player::Player() {
    this->passive = false;
    this->health = this->maxHealth;
}

float Player::getMaxMoveSpeed() const {
    return this->maxMoveSpeed;
}

void Player::setMaxMoveSpeed(float maxMoveSpeed) {
    this->maxMoveSpeed = maxMoveSpeed;
}

float Player::getMaxJumpSpeed() const {
    return this->maxJumpSpeed;
}

void Player::setMaxJumpSpeed(float maxJumpSpeed) {
    this->maxJumpSpeed = maxJumpSpeed;
}

float Player::getMaxJumpTime() const {
    return this->maxJumpTime;
}

void Player::setMaxJumpTime(float maxJumpTime) {
    this->maxJumpTime = maxJumpTime;
}

int Player::getMaxHealth() const {
    return this->maxHealth;
}

void Player::setMaxHealth(int maxHealth) {
    this->maxHealth = maxHealth;
}

int Player::getMaxArmor() const {
    return this->maxArmor;
}

void Player::setMaxArmor(int maxArmor) {
    this->maxArmor = maxArmor;
}

int Player::getHealth() const {
    return this->health;
}

void Player::setHealth(int health) {
    this->health = health;
}

int Player::getArmor() const {
    return this->armor;
}

void Player::setArmor(int armor) {
    this->armor = armor;
}

PlayerState Player::getState() const {
    return this->state;
}

void Player::setState(PlayerState state) {
    this->state = static_cast<PlayerState>(this->state | state);
}

Math::Vec3 Player::getTargetDirection() const {
    return this->getRight();
}

const std::shared_ptr<Weapon>& Player::getWeapon(WeaponSlot slot) {
    return this->weapons[slot];
}

void Player::pickWeapon(const std::shared_ptr<Weapon>& weapon) {
    WeaponSlot targetSlot = weapon->getTargetSlot();

    if (this->weapons[targetSlot] == nullptr) {
        this->weapons[targetSlot] = weapon;

        weapon->setState(WeaponState::PICKED);
        weapon->shearX(0.0f, 2);

        if (targetSlot > this->activeSlot || this->activeSlot == -1) {
            this->activateSlot(targetSlot);
        }
    }
}

void Player::activateSlot(WeaponSlot slot) {
    if (this->weapons[slot] == nullptr) {
        return;
    }

    this->weapons[slot]->aimAt(this->getTargetDirection());  // Sync with player
    this->activeSlot = slot;
}

void Player::aimAt(const Math::Vec3& position) {
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

    if (this->activeSlot != -1) {
        this->weapons[this->activeSlot]->aimAt(newTarget);
    }
}

void Player::shoot() {
    if (this->activeSlot != -1) {
        this->weapons[this->activeSlot]->fire();
    }
}

// void Player::onCollision(const std::shared_ptr<BaseEntity>& another, Collider::CollideSide side) {
//     if (another->getType() == Entity::EntityType::TYPE_WEAPON) {
//         auto weapon = std::dynamic_pointer_cast<Weapon>(another);
//         if (weapon->getState() == WeaponState::AVAILABLE) {
//             this->pickWeapon(weapon);
//             return;
//         }
//     }

//     if (another->getType() == Entity::EntityType::TYPE_PACK) {
//         auto pack = std::dynamic_pointer_cast<Pack>(another);
//         WeaponSlot slot = WeaponSlot::SLOT_MEELE;
//         int value = pack->getValue();
//         bool packStaysAlive = true;

//         switch (pack->getPayloadType()) {
//             case Pack::PayloadType::TYPE_ARMOR:
//                 packStaysAlive = (this->armor == this->maxArmor);
//                 this->armor = (this->armor + value > this->maxArmor) ? this->maxArmor : this->armor + value;
//                 break;

//             case Pack::PayloadType::TYPE_HEALTH:
//                 packStaysAlive = (this->health == this->maxHealth);
//                 this->health = (this->health + value > this->maxHealth) ? this->maxHealth : this->health + value;
//                 break;

//             case Pack::PayloadType::TYPE_PRIMARY_AMMO:
//                 slot = WeaponSlot::SLOT_PRIMARY;
//                 break;

//             case Pack::PayloadType::TYPE_SECONDARY_AMMO:
//                 slot = WeaponSlot::SLOT_SECONDARY;
//                 break;
//         }

//         if (slot != WeaponSlot::SLOT_MEELE && this->weapons[slot] != nullptr) {
//             int maxAmmo = this->weapons[slot]->getMaxAmmo();
//             int ammo = this->weapons[slot]->getAmmo();

//             packStaysAlive = (ammo == maxAmmo);
//             this->weapons[slot]->setAmmo((ammo + value > maxAmmo) ? maxAmmo : ammo + value);
//         }

//         if (!packStaysAlive) {
//             pack->destroy();
//         }
//         return;
//     }

//     switch (side) {
//         case Collider::CollideSide::SIDE_TOP:
//             this->jumpTime = FLT_MAX;
//             break;

//         case Collider::CollideSide::SIDE_BOTTOM:
//             if (this->currentSpeed.get(Math::Vec3::Y) < 0.0f) {
//                 this->jumpTime = 0.0f;
//             }
//             break;

//         default:
//             break;
//     }
// }

void Player::animate(float frameTime) {
    float playerMoveSpeed = this->speed.get(Math::Vec3::X);
    float playerJumpSpeed = this->speed.get(Math::Vec3::Y);

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

    int slotsTotal = static_cast<int>(this->weapons.size() - 1);
    for (this->activeSlot = slotsTotal; this->activeSlot > -1; this->activeSlot--) {
        if (this->weapons[this->activeSlot] != nullptr) {
            this->activateSlot(static_cast<WeaponSlot>(this->activeSlot));  // TODO: Remove cast
            break;
        }
    }

    Math::Vec3 targetDirection(this->getTargetDirection());
    float targetSignCorrection = (targetDirection.get(Math::Vec3::X) < 0.0f) ? -1.0f : 1.0f;
    Math::Vec3 dropAcceleration((Math::Vec3::UNIT_X * targetSignCorrection + Math::Vec3::UNIT_Y) * 7.0f);

    weapon->aimAt(Math::Vec3::UNIT_X * targetSignCorrection);
    weapon->setState(WeaponState::THROWN);
    weapon->translate(weapon->getPosition() + Math::Vec3::UNIT_Y * 0.5f);  // Don't collide from bottom

    float hotizontalSpeed = this->getSpeed().get(Math::Vec3::X);
    float verticalSpeed = this->getSpeed().get(Math::Vec3::Y);
    weapon->setSpeed(Math::Vec3(hotizontalSpeed, (verticalSpeed > 0.0f) ? verticalSpeed : 0.0f, 0.0f));
    weapon->accelerateBy(dropAcceleration);
}

}  // namespace Game

}  // namespace PolandBall
