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

#include "Sprite.h"
#include "ResourceManager.h"

namespace PolandBall {

namespace Opengl {

const float Sprite::vertices[] = {
    // coords
     1.0f,  1.0f,  0.0f,  // 0  2<--1
    -1.0f,  1.0f,  0.0f,  // 1  |   ^
    -1.0f, -1.0f,  0.0f,  // 2  v   |
     1.0f, -1.0f,  0.0f,  // 3  3-->4
     // UVs
     1.0f,  0.0f,  // 0
     0.0f,  0.0f,  // 1
     0.0f,  1.0f,  // 2
     1.0f,  1.0f,  // 3
};

const int Sprite::indices[] = {
    0, 1, 2,
    0, 2, 3,
};

Sprite::Sprite() {
    glGenBuffers(2, this->buffers);
    glGenVertexArrays(1, &this->vao);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[Sprite::VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(12 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[Sprite::ELEMENT_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    this->xAngle = 0.0f;
    this->yAngle = 0.0f;
    this->zAngle = 0.0f;
}

void Sprite::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        return;
    }

    Math::Vec3 axis(vector);
    Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
    q.normalize();

    this->rotation = q.extractMat4();

    float xAngleNew, yAngleNew, zAngleNew;
    q.extractEulerAgngles(xAngleNew, yAngleNew, zAngleNew);

    this->xAngle = xAngleNew * 180.f / M_PI;
    this->yAngle = yAngleNew * 180.f / M_PI;
    this->zAngle = zAngleNew * 180.f / M_PI;
}

void Sprite::render() {
    if (this->texture == nullptr || this->effect == nullptr) {
        return;
    }

    this->texture->bind();
    this->effect->enable();
    this->effect->setUniform("lw", this->translation * this->rotation * this->scaling);
    this->effect->setUniform("transform", this->replication * this->shear);

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}

}  // namespace Opengl

}  // namespace PolandBall
