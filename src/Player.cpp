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

namespace PolandBall {

void Player::moveRight(float frameTime) {
    float playerMoveSpeed = this->currentSpeed.get(Math::Vec3::X);
    float moveSpeedDelta = this->maxMoveSpeed * frameTime * 10.0f;
    Math::Vec3 moveAcceleration;

    // We press RIGHT and we already moving RIGHT
    if (playerMoveSpeed > 0.0f && fabs(playerMoveSpeed) + moveSpeedDelta > this->maxMoveSpeed) {
        moveAcceleration = Math::Vec3::UNIT_X * (this->maxMoveSpeed - fabs(playerMoveSpeed));
    } else {
        moveAcceleration = Math::Vec3::UNIT_X * moveSpeedDelta;
    }

    this->accelerateBy(moveAcceleration);
}

void Player::moveLeft(float frameTime) {
    float playerMoveSpeed = this->currentSpeed.get(Math::Vec3::X);
    float moveSpeedDelta = this->maxMoveSpeed * frameTime * 10.0f;
    Math::Vec3 moveAcceleration;

    // We press LEFT and we already moving LEFT
    if (playerMoveSpeed < 0.0f && fabs(playerMoveSpeed) + moveSpeedDelta > this->maxMoveSpeed) {
        moveAcceleration = -Math::Vec3::UNIT_X * (this->maxMoveSpeed - fabs(playerMoveSpeed));
    } else {
        moveAcceleration = -Math::Vec3::UNIT_X * moveSpeedDelta;
    }

    this->accelerateBy(moveAcceleration);
}

void Player::slowDown(float frameTime) {
    float playerMoveSpeed = this->currentSpeed.get(Math::Vec3::X);
    float moveSpeedDelta = this->maxMoveSpeed * frameTime * 5.0f;
    Math::Vec3 moveAcceleration;

    if (this->currentSpeed.get(Math::Vec3::Y) == 0.0f) {
        if ((playerMoveSpeed > 0.0f && playerMoveSpeed - moveSpeedDelta < 0.0f) ||
                (playerMoveSpeed < 0.0f && playerMoveSpeed + moveSpeedDelta > 0.0f)) {
            moveAcceleration = Math::Vec3::UNIT_X * fabs(playerMoveSpeed);
        } else {
            moveAcceleration = Math::Vec3::UNIT_X * moveSpeedDelta;
        }

        if (playerMoveSpeed < 0.0f) {
            this->accelerateBy(moveAcceleration);
        } else if (playerMoveSpeed > 0.0f) {
            this->accelerateBy(-moveAcceleration);
        }
    }
}

void Player::jump(float frameTime) {
    float playerUpSpeed = this->currentSpeed.get(Math::Vec3::Y);

    if (this->jumpWasReleased && this->jumpTime < this->maxJumpTime && playerUpSpeed >= 0.0f) {
        if (playerUpSpeed < this->maxJumpSpeed) {
            Math::Vec3 jumpAcceleration(Math::Vec3::UNIT_Y * (this->maxJumpSpeed - playerUpSpeed));
            this->accelerateBy(jumpAcceleration);
        }

        this->jumpTime += frameTime;
    } else {
        this->jumpWasReleased = false;
    }
}

void Player::aimAt(const Math::Vec3& target) {
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

    float currentAngle = this->sprite->getXAngle();
    Math::Vec3 normal = this->target.cross(newTarget);
    float signCorrection = (normal.get(Math::Vec3::Z) < 0) ? -1.0f : 1.0f;
    float newAngle = currentAngle + deltaAngle * signCorrection;

    this->roll(newAngle);
    if (newAngle > 90.0f || newAngle <= -90.0f) {
        this->shearX(1, 2);
    } else {
        this->shearX(0, 2);
    }

    this->target = newTarget;
}

}  // namespace PolandBall
