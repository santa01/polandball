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

#ifndef PACK_H
#define	PACK_H

#include "Entity.h"

namespace PolandBall {

namespace Game {

class Pack: public Entity {
public:
    enum PayloadType {
        TYPE_HEALTH,
        TYPE_ARMOR,
        TYPE_PRIMARY_AMMO,
        TYPE_SECONDARY_AMMO
    };

    Pack():
            Pack(TYPE_HEALTH) {
    }

    Pack(PayloadType payload) {
        this->payload = payload;
        this->value = 10;
        this->bounce = 0.0f;
        this->type = TYPE_PACK;
    }

    PayloadType getPayloadType() const {
        return this->payload;
    }

    void setPayloadType(PayloadType payload) {
        this->payload = payload;
    }

    int getValue() const {
        return this->value;
    }

    void setValue(int value) {
        this->value = value;
    }

private:
    void animate(float frameTime) {
        this->shearY(sinf(this->bounce) / 13.33f - 0.075f, 1);
        this->bounce += frameTime * 6.0f;
    }

    PayloadType payload;
    float bounce;
    int value;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // PACK_H
