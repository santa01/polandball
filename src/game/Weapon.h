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

#include <SpriteEntity.h>
// #include <Collider.h>
#include <Vec3.h>
#include <memory>

namespace PolandBall {

namespace Game {

enum WeaponSlot { SLOT_MEELE, SLOT_SECONDARY, SLOT_PRIMARY };
enum class WeaponState { AVAILABLE, PICKED, THROWN };

class Weapon: public SpriteEntity {
public:
    Weapon();
    Weapon(WeaponSlot slot);

    void setMaxAmmo(int maxAmmo);
    int getMaxAmmo() const;

    void setAmmo(int ammo);
    int getAmmo() const;

    void setGroupingAngle(float groupingAngle);
    float getGroupingAngle() const;

    void setFiringSpeed(float firingSpeed);
    float getFiringSpeed() const;

    void setTargetSlot(WeaponSlot slot);
    WeaponSlot getTargetSlot() const;

    void setState(WeaponState state);
    WeaponState getState() const;

    Math::Vec3 getTargetDirection() const;

    void aimAt(const Math::Vec3& position);
    void fire();

private:
    // void onCollision(const std::shared_ptr<SpriteEntity>& another, Collider::CollideSide side) override;
    void animate(float frameTime) override;

    int maxAmmo = 100;
    int ammo = 100;
    float groupingAngle = 1.0f;
    float firingSpeed = 3.0f;  // Shots per second

    WeaponSlot targetSlot = WeaponSlot::SLOT_MEELE;
    WeaponState state = WeaponState::AVAILABLE;
    bool firing = false;

    float bounce = 0.0f;
    float relaxTime = 0.0f;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // WEAPON_H
