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

#include "Collider.h"

#include <cmath>

namespace PolandBall {

int Collider::collides(const std::unique_ptr<Collider>& another) const {
    // Broad phase
    auto ourBox = this->getCollideBox();
    auto anotherBox = another->getCollideBox();
    int result = 0;

    bool intersect = false;
    for (int i = 0; i < 4; i++) {
        if (anotherBox[i].get(Math::Vec3::X) > ourBox[1].get(Math::Vec3::X) &&
                anotherBox[i].get(Math::Vec3::X) < ourBox[0].get(Math::Vec3::X) &&
                anotherBox[i].get(Math::Vec3::Y) < ourBox[0].get(Math::Vec3::Y) &&
                anotherBox[i].get(Math::Vec3::Y) > ourBox[3].get(Math::Vec3::Y)) {
            intersect = true;
            break;
        }
    }

    if (!intersect) {
        return result;
    }

    // Narrow phase
    Math::Vec3 toZeroBottomFirst = anotherBox[0] - ourBox[3];
    Math::Vec3 toZeroBottomSecond = anotherBox[0] - ourBox[2];
    
    
    
    return result | COLLIDE_TOP;
}

}  // namespace PolandBall
