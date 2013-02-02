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
