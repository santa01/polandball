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
#define	CAMERA_H

#include "Mat4.h"
#include "Vec3.h"
#include "IMovable.h"
#include "IRotatable.h"

namespace PolandBall {

class Camera: public IMovable, public IRotatable {
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

    using IMovable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->translation.set(0, 3, -position.get(Math::Vec3::X));
        this->translation.set(1, 3, -position.get(Math::Vec3::Y));
        this->translation.set(2, 3, -position.get(Math::Vec3::Z));
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
        // TODO: implement
        return 0.0f;
    }

    float getYAngle() const {
        // TODO: implement
        return 0.0f;
    }

    float getZAngle() const {
        // TODO: implement
        return 0.0f;
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

    void setNearPlane(float nearPlane) {
        this->nearPlane = nearPlane;
        this->updateClipDistances();
    }

    float getNearPlane() const {
        return this->nearPlane;
    }

    void setFarPlane(float farPlane) {
        this->farPlane = farPlane;
        this->updateClipDistances();
    }

    float getFarPlane() const {
        return this->farPlane;
    }

    void setFov(float fov) {
        this->fov = fov;
        this->updateFieldOfView();
    }

    float getFov() const {
        return this->fov;
    }

    void setAspectRatio(float aspectRatio) {
        this->aspectRatio = aspectRatio;
        this->updateFieldOfView();
    }

    float getAspectRatio() const {
        return this->aspectRatio;
    }

    const Math::Mat4& getProjectionMatrix() const {
        return this->projection;
    }

    // Slot
    void onPositionUpdate(const Math::Vec3& positionDelta) {
        Math::Vec3 currentPosition = this->getPosition();
        currentPosition.set(Math::Vec3::X, currentPosition.get(Math::Vec3::X) + positionDelta.get(Math::Vec3::X));
        currentPosition.set(Math::Vec3::Y, currentPosition.get(Math::Vec3::Y) + positionDelta.get(Math::Vec3::Y));
        this->setPosition(currentPosition);
    }

private:
    void updateClipDistances() {
        this->projection.set(2, 2, (-this->farPlane - this->nearPlane) /
                                   (this->farPlane - this->nearPlane));
        this->projection.set(2, 3, (-2.0f * this->farPlane * this->nearPlane) /
                                   (this->farPlane - this->nearPlane));
    }

    void updateFieldOfView() {
        this->projection.set(0, 0, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f) *
                                   this->aspectRatio));
        this->projection.set(1, 1, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f)));
    }

    void updateRotationMatrix(const Math::Vec3& right, const Math::Vec3& up, const Math::Vec3& target);
    void initialize();

    Math::Mat4 translation;
    Math::Mat4 rotation;
    Math::Mat4 projection;

    float aspectRatio;
    float nearPlane;
    float farPlane;
    float fov;
};

}  // namespace PolandBall

#endif  // CAMERA_H
