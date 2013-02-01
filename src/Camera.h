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
    Camera();
    Camera(float x, float y, float z);
    Camera(const Math::Vec3& position);

    using IMovable::setPosition;

    void setPosition(const Math::Vec3& position);
    Math::Vec3 getPosition() const;

    const Math::Mat4& getTranslationMatrix() const {
        return this->translation;
    }

    float getXAngle() const;
    float getYAngle() const;
    float getZAngle() const;

    void rotate(const Math::Vec3& vector, float angle);

    const Math::Mat4& getRotationMatrix() const {
        return this->rotation;
    }

    Math::Vec3 getUp() const;
    Math::Vec3 getTarget() const;
    Math::Vec3 getRight() const;

    void lookAt(float x, float y, float z);
    void lookAt(const Math::Vec3& target);

    void setNearPlane(float nearPlane);
    float getNearPlane() const {
        return this->nearPlane;
    }

    void setFarPlane(float farPlane);
    float getFarPlane() const {
        return this->farPlane;
    }

    void setFov(float fov);
    float getFov() const {
        return this->fov;
    }

    void setAspectRatio(float aspectRatio);
    float getAspectRatio() const {
        return this->aspectRatio;
    }

    const Math::Mat4& getProjectionMatrix() const {
        return this->projection;
    }

private:
    void updateRotationMatrix(const Math::Vec3& right, const Math::Vec3& up, const Math::Vec3& target);
    void updateClipDistances();
    void updateFieldOfView();
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
