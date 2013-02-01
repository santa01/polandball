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
#include "Logger.h"
#include "Vec3.h"
#include "Quaternion.h"
#include "RenderEffect.h"
#include "ResourceManager.h"

#include <cmath>
#include <fstream>
#include <string>

namespace PolandBall {

const float Sprite::vertices[] = {
    // coords
     2.0f,  2.0f,  0.0f,  // 0
    -2.0f,  2.0f,  0.0f,  // 1
    -2.0f, -2.0f,  0.0f,  // 2
     2.0f, -2.0f,  0.0f,  // 3
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

Sprite::Sprite():
        texture(),
        effect() {
    this->initialize();
}

Sprite::Sprite(float x, float y, float z) {
    this->initialize();
    this->setPosition(x, y, z);
}

Sprite::Sprite(const Math::Vec3& position) {
    this->initialize();
    this->setPosition(position);
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(2, this->buffers);
}

void Sprite::setPosition(const Math::Vec3& position) {
    this->translation.set(0, 3, position.get(Math::Vec3::X));
    this->translation.set(1, 3, position.get(Math::Vec3::Y));
    this->translation.set(2, 3, position.get(Math::Vec3::Z));
}

Math::Vec3 Sprite::getPosition() const {
    return Math::Vec3(this->translation.get(0, 3),
                this->translation.get(1, 3),
                this->translation.get(2, 3));
}

float Sprite::getXAngle() const {
    // TODO: implement
    return 0.0f;
}

float Sprite::getYAngle() const {
    // TODO: implement
    return 0.0f;
}

float Sprite::getZAngle() const {
    // TODO: implement
    return 0.0f;
}

void Sprite::rotate(const Math::Vec3& vector, float angle) {
    Math::Quaternion q(vector, angle * M_PI / 180.0f);
    q.normalize();
    
    this->rotation = q.extractMat4();
}

void Sprite::scaleX(float factor) {
    this->scaling.set(0, 0, this->scaling.get(0, 0) * factor);
}

void Sprite::scaleY(float factor) {
    this->scaling.set(1, 1, this->scaling.get(1, 1) * factor);
}

void Sprite::scaleZ(float factor) {
    this->scaling.set(2, 2, this->scaling.get(2, 2) * factor);
}

float Sprite::getXFactor() const {
    return this->scaling.get(0, 0);
}

float Sprite::getYFactor() const {
    return this->scaling.get(1, 1);
}

float Sprite::getZFactor() const {
    return this->scaling.get(2, 2);
}

void Sprite::render() {
    if (this->texture != nullptr) {
        this->texture->bind();
    }

    if (this->effect != nullptr) {
        this->effect->enable();
        this->effect->setLW(this->translation * this->rotation * this->scaling);
    }

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}

void Sprite::initialize() {
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

    this->effect = ResourceManager::getInstance().makeEffect("default");
    this->texture = ResourceManager::getInstance().makeTexture("default");
}

}  // namespace PolandBall
