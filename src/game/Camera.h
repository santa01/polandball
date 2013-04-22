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

#ifndef CAMERA_H
#define CAMERA_H

#include "Mat4.h"
#include "Vec3.h"
#include "Signals.h"
#include "Movable.h"
#include "Rotatable.h"

namespace PolandBall {

class Camera: public Movable, public Rotatable {
public:
    Camera() {
        this->initialize();
    }

    Camera(float x, float y, float z) {
        this->setPosition(x, y, z);
        this->initialize();
    }

    Camera(const Math::Vec3& position) {
        this->setPosition(position);
        this->initialize();
    }

    using Movable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->translation.set(0, 3, -position.get(Math::Vec3::X));
        this->translation.set(1, 3, -position.get(Math::Vec3::Y));
        this->translation.set(2, 3, -position.get(Math::Vec3::Z));
        this->positionChanged(position);  // Emit signal
    }

    Math::Vec3 getPosition() const {
        return Math::Vec3(-this->translation.get(0, 3),
                          -this->translation.get(1, 3),
                          -this->translation.get(2, 3));
    }

    const Math::Mat4& getTranslationMatrix() const {
        return this->translation;
    }

    float getXAngle() const {
        return this->xAngle;
    }

    float getYAngle() const {
        return this->yAngle;
    }

    float getZAngle() const {
        return this->zAngle;
    }

    void rotate(const Math::Vec3& vector, float angle);

    const Math::Mat4& getRotationMatrix() const {
        return this->rotation;
    }

    Math::Vec3 getUp() const {
        return Math::Vec3(this->rotation.get(1, 0),
                          this->rotation.get(1, 1),
                          this->rotation.get(1, 2));
    }

    Math::Vec3 getTarget() const {
        return -Math::Vec3(this->rotation.get(2, 0),
                           this->rotation.get(2, 1),
                           this->rotation.get(2, 2));
    }

    Math::Vec3 getRight() const;

    void lookAt(float x, float y, float z) {
        this->lookAt(Math::Vec3(x, y, z));
    }

    void lookAt(const Math::Vec3& target);

    void setPlaneDistance(float planeDistance) {
        this->planeDistance = planeDistance;
        this->updateProjection();
    }

    float getPlaneDistance() const {
        return this->planeDistance;
    }

    void setAspectRatio(float aspectRatio) {
        this->aspectRatio = aspectRatio;
        this->updateProjection();
    }

    float getAspectRatio() const {
        return this->aspectRatio;
    }

    const Math::Mat4& getProjectionMatrix() const {
        return this->projection;
    }

    Signals::Signal<Math::Vec3> positionChanged;

private:
    void updateProjection() {
        this->projection.set(0, 0, 1.0f / (this->planeDistance * this->aspectRatio));
        this->projection.set(1, 1, 1.0f / this->planeDistance);
        this->projection.set(2, 2, -1.0f / this->planeDistance);
    }

    void updateRotation(const Math::Vec3& right, const Math::Vec3& up, const Math::Vec3& target) {
        this->rotation.set(0, 0, right.get(Math::Vec3::X));
        this->rotation.set(0, 1, right.get(Math::Vec3::Y));
        this->rotation.set(0, 2, right.get(Math::Vec3::Z));

        this->rotation.set(1, 0, up.get(Math::Vec3::X));
        this->rotation.set(1, 1, up.get(Math::Vec3::Y));
        this->rotation.set(1, 2, up.get(Math::Vec3::Z));

        this->rotation.set(2, 0, target.get(Math::Vec3::X));
        this->rotation.set(2, 1, target.get(Math::Vec3::Y));
        this->rotation.set(2, 2, target.get(Math::Vec3::Z));
    }

    void initialize() {
        this->aspectRatio = 1.3333f;
        this->planeDistance = 10.0f;

        this->updateProjection();
        this->lookAt(Math::Vec3::UNIT_Z);
    }

    Math::Mat4 translation;
    Math::Mat4 rotation;
    Math::Mat4 projection;

    float aspectRatio;
    float planeDistance;

    float xAngle;
    float yAngle;
    float zAngle;
};

}  // namespace PolandBall

#endif  // CAMERA_H
