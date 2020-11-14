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

#include <Pack.h>

namespace PolandBall {

namespace Game {

Pack::Pack():
        Pack(PayloadType::HEALTH) {
}

Pack::Pack(PayloadType payload):
        payload(payload) {
    this->passive = false;
}

PayloadType Pack::getPayloadType() const {
    return this->payload;
}

void Pack::setPayloadType(PayloadType payload) {
    this->payload = payload;
}

int Pack::getValue() const {
    return this->value;
}

void Pack::setValue(int value) {
    this->value = value;
}

void Pack::animate(float frameTime) {
    this->shearY(sinf(this->bounce) / 13.33f - 0.075f, 1);
    this->bounce += frameTime * 6.0f;
}

}  // namespace Game

}  // namespace PolandBall
