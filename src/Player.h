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
#define	PLAYER_H

#include "Entity.h"
#include "Collider.h"

namespace PolandBall {

class Player: public Entity {
public:
    Player():
            Entity(Entity::TYPE_DYNAMIC) {
        this->jumpTime = 0.0f;
    }

    Player(EntityType type):
            Entity(type) {
        this->jumpTime = 0.0f;
    }

    void collideSide(Collider::CollideSide side) {
        switch (side) {
            case Collider::CollideSide::SIDE_TOP:
                this->jumpTime = 100.0f;  // Just god damn big value, bigger than the maximum jump time
                break;

            case Collider::CollideSide::SIDE_BOTTOM:
                this->jumpTime = 0.0f;
                break;

            default:
                break;
        }
    }
    
    float getJumpTime() const {
        return this->jumpTime;
    }

    void updateJumpTime(float delta) {
        this->jumpTime += delta;
    }

private:
    float jumpTime;
};

}  // namespace PolandBall

#endif  // PLAYER_H
