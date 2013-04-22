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

Collider::CollideSide Collider::collides(const std::unique_ptr<Collider>& another) const {
    if (this == another.get()) {
        return SIDE_NONE;
    }

    auto ourBox = this->getCollideBox();
    auto anotherBox = another->getCollideBox();
    float width = 0, height = 0;

    // Their top is inside us
    if ((anotherBox[0].get(Math::Vec3::Y) <= ourBox[0].get(Math::Vec3::Y)) &&
            (anotherBox[0].get(Math::Vec3::Y) >= ourBox[3].get(Math::Vec3::Y)) &&
            !((anotherBox[3].get(Math::Vec3::Y) <= ourBox[0].get(Math::Vec3::Y)) &&
            (anotherBox[3].get(Math::Vec3::Y) >= ourBox[3].get(Math::Vec3::Y)))) {

        bool topRightInside = this->vertexInsideBox(anotherBox[0], ourBox);
        bool topLeftInside = this->vertexInsideBox(anotherBox[1], ourBox);

        if (topLeftInside && topRightInside) {
            return SIDE_BOTTOM;
        }

        if (topLeftInside) {
            width = ourBox[3].get(Math::Vec3::X) - anotherBox[1].get(Math::Vec3::X);
            height = anotherBox[1].get(Math::Vec3::Y) - ourBox[3].get(Math::Vec3::Y);
            return (width > height) ? SIDE_BOTTOM : SIDE_RIGHT;
        }

        if (topRightInside) {
            width = anotherBox[0].get(Math::Vec3::X) - ourBox[2].get(Math::Vec3::X);
            height = anotherBox[0].get(Math::Vec3::Y) - ourBox[2].get(Math::Vec3::Y);
            return (width > height) ? SIDE_BOTTOM : SIDE_LEFT;
        }

        if (!topLeftInside && !topRightInside &&
                anotherBox[1].get(Math::Vec3::X) <= ourBox[1].get(Math::Vec3::X) &&
                anotherBox[0].get(Math::Vec3::X) >= ourBox[0].get(Math::Vec3::X)) {
            return SIDE_BOTTOM;
        }
    // Their bottom is inside us
    } else if ((anotherBox[3].get(Math::Vec3::Y) <= ourBox[0].get(Math::Vec3::Y)) &&
            (anotherBox[3].get(Math::Vec3::Y) >= ourBox[3].get(Math::Vec3::Y)) &&
            !((anotherBox[0].get(Math::Vec3::Y) <= ourBox[0].get(Math::Vec3::Y)) &&
            (anotherBox[0].get(Math::Vec3::Y) >= ourBox[3].get(Math::Vec3::Y)))) {

        bool bottomRightInside = this->vertexInsideBox(anotherBox[3], ourBox);
        bool bottomLeftInside = this->vertexInsideBox(anotherBox[2], ourBox);

        // Both inside (narrorer) or both outside (wider)
        if (bottomLeftInside && bottomRightInside) {
            return SIDE_TOP;
        }

        if (bottomLeftInside) {
            width = ourBox[0].get(Math::Vec3::X) - anotherBox[2].get(Math::Vec3::X);
            height = ourBox[0].get(Math::Vec3::Y) - anotherBox[2].get(Math::Vec3::Y);
            return (width > height) ? SIDE_TOP : SIDE_RIGHT;
        }

        if (bottomRightInside) {
            width = anotherBox[3].get(Math::Vec3::X) - ourBox[1].get(Math::Vec3::X);
            height = ourBox[1].get(Math::Vec3::Y) - anotherBox[3].get(Math::Vec3::Y);
            return (width > height) ? SIDE_TOP : SIDE_LEFT;
        }

        if (!bottomLeftInside && !bottomRightInside &&
                anotherBox[2].get(Math::Vec3::X) <= ourBox[2].get(Math::Vec3::X) &&
                anotherBox[3].get(Math::Vec3::X) >= ourBox[3].get(Math::Vec3::X)) {
            return SIDE_TOP;
        }
    // Their right is inside us
    } else if ((anotherBox[0].get(Math::Vec3::X) <= ourBox[0].get(Math::Vec3::X)) &&
            (anotherBox[0].get(Math::Vec3::X) >= ourBox[1].get(Math::Vec3::X)) &&
            !((anotherBox[1].get(Math::Vec3::X) <= ourBox[0].get(Math::Vec3::X)) &&
            (anotherBox[1].get(Math::Vec3::X) >= ourBox[1].get(Math::Vec3::X)))) {
        // Partial intersection is already covered in previous clauses
        bool rightTopInside = this->vertexInsideBox(anotherBox[0], ourBox);
        bool rightBottomInside = this->vertexInsideBox(anotherBox[3], ourBox);

        if (rightBottomInside && rightTopInside) {
            return SIDE_LEFT;
        }

        if (!rightBottomInside && !rightTopInside &&
                anotherBox[0].get(Math::Vec3::Y) >= ourBox[1].get(Math::Vec3::Y) &&
                anotherBox[3].get(Math::Vec3::Y) <= ourBox[2].get(Math::Vec3::Y)) {
            return SIDE_LEFT;
        }
    // Their left is inside us
    } else if ((anotherBox[1].get(Math::Vec3::X) <= ourBox[0].get(Math::Vec3::X)) &&
            (anotherBox[1].get(Math::Vec3::X) >= ourBox[1].get(Math::Vec3::X)) &&
            !((anotherBox[0].get(Math::Vec3::X) <= ourBox[0].get(Math::Vec3::X)) &&
            (anotherBox[0].get(Math::Vec3::X) >= ourBox[1].get(Math::Vec3::X)))) {
        // Partial intersection is already covered in previous clauses
        bool leftTopInside = this->vertexInsideBox(anotherBox[1], ourBox);
        bool leftBottomInside = this->vertexInsideBox(anotherBox[2], ourBox);

        // Both inside (narrorer) or both outside (wider)
        if (leftBottomInside && leftTopInside) {
            return SIDE_RIGHT;
        }

        if (!leftBottomInside && !leftTopInside &&
                anotherBox[1].get(Math::Vec3::Y) >= ourBox[0].get(Math::Vec3::Y) &&
                anotherBox[2].get(Math::Vec3::Y) <= ourBox[0].get(Math::Vec3::Y)) {
            return SIDE_RIGHT;
        }
    }

    return SIDE_NONE;
}

}  // namespace PolandBall
