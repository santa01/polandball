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

#ifndef SHOTTRACE_H
#define SHOTTRACE_H

#include "LineEntity.h"

#include <Vec3.h>
#include <Vec4.h>

namespace PolandBall {

namespace Game {

class ShotTrace: public LineEntity {
public:
    ShotTrace(const Math::Vec3& from, const Math::Vec3& to) {
        this->transparency = 1.0f;
        this->maxLifeTime = 0.1f;
        this->maxFadeTime = 0.2f;

        this->line->setFrom(from);
        this->line->setTo(to);
        this->lifeTime = 0.0f;
        this->fadeTime = 0.0f;

        this->collidable = false;
        this->type = Entity::EntityType::TYPE_GENERIC;
    }

    float getLifeTime() const {
        return this->maxLifeTime;
    }

    void setLifeTime(float maxLifeTime) {
        this->maxLifeTime = maxLifeTime;
    }

    float getFadeTime() const {
        return this->maxFadeTime;
    }

    void setFadeTime(float maxFadeTime) {
        this->maxFadeTime = maxFadeTime;
    }

private:
    void animate(float frameTime);

    float transparency;
    float maxLifeTime;
    float maxFadeTime;

    float lifeTime;
    float fadeTime;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // SHOTTRACE_H
