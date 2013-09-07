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

#ifndef COLLIDER_H
#define COLLIDER_H

#include "Scalable.h"
#include "Movable.h"

#include <Vec3.h>
#include <Vec4.h>
#include <Mat4.h>
#include <array>
#include <memory>

namespace PolandBall {

namespace Game {

// NOTE: Not Rotateble!
class Collider: public Common::Movable, public Common::Scalable {
public:
    enum CollideSide {
        SIDE_NONE,
        SIDE_TOP,
        SIDE_LEFT,
        SIDE_BOTTOM,
        SIDE_RIGHT
    };

    Collider() {
        // NOTE: This is the same as Sprite default geometry
        this->collideBox[0] = Math::Vec3( 0.5f,  0.5f,  0.0f);  // 0  1<--0
        this->collideBox[1] = Math::Vec3(-0.5f,  0.5f,  0.0f);  // 1  |   ^
        this->collideBox[2] = Math::Vec3(-0.5f, -0.5f,  0.0f);  // 2  v   |
        this->collideBox[3] = Math::Vec3( 0.5f, -0.5f,  0.0f);  // 3  2-->3
    }

    using Movable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->translation.set(0, 3, position.get(Math::Vec3::X));
        this->translation.set(1, 3, position.get(Math::Vec3::Y));
        this->translation.set(2, 3, position.get(Math::Vec3::Z));
    }

    Math::Vec3 getPosition() const {
        return Math::Vec3(this->translation.get(0, 3),
                          this->translation.get(1, 3),
                          this->translation.get(2, 3));
    }

    void scaleX(float factor) {
        this->scaling.set(0, 0, factor);
    }

    void scaleY(float factor) {
        this->scaling.set(1, 1, factor);
    }

    void scaleZ(float factor) {
        this->scaling.set(2, 2, factor);
    }

    float getXFactor() const {
        return this->scaling.get(0, 0);
    }

    float getYFactor() const {
        return this->scaling.get(1, 1);
    }

    float getZFactor() const {
        return this->scaling.get(2, 2);
    }

    CollideSide collides(const std::unique_ptr<Collider>& another) const;

private:
    std::array<Math::Vec3, 4> getCollideBox() const {
        std::array<Math::Vec3, 4> realBox;
        Math::Mat4 transformation = this->translation * this->scaling;

        for (int i = 0; i < 4; i++) {
            realBox[i] = (transformation * Math::Vec4(this->collideBox[i], 1.0f)).extractVec3();
        }

        return realBox;
    }

    bool vertexInsideBox(const Math::Vec3& vertex, const std::array<Math::Vec3, 4>& box) const {
        if (vertex.get(Math::Vec3::X) <= box[0].get(Math::Vec3::X) &&
                vertex.get(Math::Vec3::X) >= box[1].get(Math::Vec3::X) &&
                vertex.get(Math::Vec3::Y) <= box[0].get(Math::Vec3::Y) &&
                vertex.get(Math::Vec3::Y) >= box[3].get(Math::Vec3::Y)) {
            return true;
        }

        return false;
    }

    std::array<Math::Vec3, 4> collideBox;

    Math::Mat4 translation;
    Math::Mat4 scaling;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // COLLIDER_H
