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

#include <SpriteEntity.h>
// #include <Collider.h>
#include <Weapon.h>
#include <Vec3.h>
#include <memory>
#include <array>

namespace PolandBall {

namespace Game {

enum PlayerState {
    STATE_IDLE = 1 << 0,
    STATE_LEFT_STEP = 1 << 1,
    STATE_RIGHT_STEP = 1 << 2,
    STATE_JUMP = 1 << 3,
    STATE_DROP_WEAPON = 1 << 4
};

class Player: public SpriteEntity {
public:
    Player();

    float getMaxMoveSpeed() const;
    void setMaxMoveSpeed(float maxMoveSpeed);

    float getMaxJumpSpeed() const;
    void setMaxJumpSpeed(float maxJumpSpeed);

    float getMaxJumpTime() const;
    void setMaxJumpTime(float maxJumpTime);

    int getMaxHealth() const;
    void setMaxHealth(int maxHealth);

    int getMaxArmor() const;
    void setMaxArmor(int maxArmor);

    int getHealth() const;
    void setHealth(int health);

    int getArmor() const;
    void setArmor(int armor);

    PlayerState getState() const;
    void setState(PlayerState state);

    Math::Vec3 getTargetDirection() const;
    const std::shared_ptr<Weapon>& getWeapon(WeaponSlot slot);

    void pickWeapon(const std::shared_ptr<Weapon>& weapon);
    void activateSlot(WeaponSlot slot);

    void aimAt(const Math::Vec3& position);
    void shoot();

private:
    // void onCollision(const std::shared_ptr<SpriteEntity>& another, Collider::CollideSide side) override;
    void animate(float frameTime) override;

    void dropWeapon();

    float maxMoveSpeed = 8.0f;
    float maxJumpSpeed = 10.0f;
    float maxJumpTime = 0.5f;
    float minDropTime = 0.5f;
    int maxHealth = 100;
    int maxArmor = 100;

    float jumpTime = 0.0f;
    float dropTime = 0.0f;
    int health = 0;
    int armor = 0;

    PlayerState state = PlayerState::STATE_IDLE;
    PlayerState previousState = PlayerState::STATE_IDLE;
    int activeSlot = -1;

    std::array<std::shared_ptr<Weapon>, 3> weapons;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // PLAYER_H
