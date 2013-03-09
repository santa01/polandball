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

#include "Camera.h"
#include "Mat3.h"
#include "Quaternion.h"

#include <cmath>

namespace PolandBall {

void Camera::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        return;
    }
    
    Math::Vec3 axis(vector);
    Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
    q.normalize();
    
    Math::Mat3 rotationMatrix = q.extractMat4().extractMat3();
    Math::Vec3 up(this->rotation.get(1, 0),
                  this->rotation.get(1, 1),
                  this->rotation.get(1, 2));
    Math::Vec3 target(this->rotation.get(2, 0),
                      this->rotation.get(2, 1),
                      this->rotation.get(2, 2));
    up = rotationMatrix * up;
    target = rotationMatrix * target;
    
    Math::Vec3 right = target.cross(up);
    right.normalize();

    this->updateRotationMatrix(right, up, target);

    float xAngleNew, yAngleNew, zAngleNew;
    q.extractEulerAgngles(xAngleNew, yAngleNew, zAngleNew);

    this->xAngle = xAngleNew * 180.f / M_PI;
    this->yAngle = yAngleNew * 180.f / M_PI;
    this->zAngle = zAngleNew * 180.f / M_PI;
}

Math::Vec3 Camera::getRight() const {
    Math::Vec3 up(this->rotation.get(1, 0),
                  this->rotation.get(1, 1),
                  this->rotation.get(1, 2));
    Math::Vec3 target(this->rotation.get(2, 0),
                      this->rotation.get(2, 1),
                      this->rotation.get(2, 2));
    Math::Vec3 right = target.cross(up);
    return right.normalize();
}

void Camera::lookAt(const Math::Vec3& vector) {
    if (vector == Math::Vec3::ZERO) {
        return;
    }
    
    Math::Vec3 target(-vector);
    target.normalize();

    Math::Vec3 right = this->getRight();
    if (target != Math::Vec3::UNIT_Y && target != -Math::Vec3::UNIT_Y) {
        right = target.cross(Math::Vec3::UNIT_Y);
        right.normalize();  
    }
    
    Math::Vec3 up = right.cross(target);
    up.normalize();
    
    this->updateRotationMatrix(right, up, target);
}

void Camera::updateRotationMatrix(const Math::Vec3& right, const Math::Vec3& up, const Math::Vec3& target) {
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

void Camera::initialize() {
    this->aspectRatio = 1.3333f;
    this->nearPlane = 0.1f;
    this->farPlane = 10.0f;
    this->fov = 90.0f;

    this->projection.set(3, 2, -1.0f);
    this->projection.set(3, 3, 0.0f);

    this->updateFieldOfView();
    this->updateClipDistances();

    this->lookAt(Math::Vec3::UNIT_Z);
}

}  // namespace PolandBall
