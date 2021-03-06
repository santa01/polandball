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

namespace PolandBall {

namespace Opengl {

const GLfloat Sprite::vertices[] = {
    // coords
     0.5f,  0.5f,  0.0f,  // 0  2<--1
    -0.5f,  0.5f,  0.0f,  // 1  |   ^
    -0.5f, -0.5f,  0.0f,  // 2  v   |
     0.5f, -0.5f,  0.0f,  // 3  3-->4
     // UVs
     1.0f,  0.0f,  // 0
     0.0f,  0.0f,  // 1
     0.0f,  1.0f,  // 2
     1.0f,  1.0f,  // 3
};

const GLuint Sprite::indices[] = {
    0, 1, 2,
    0, 2, 3,
};

Sprite::Sprite() {
    PrimitiveData data;

    data.vertexDataSize = sizeof(Sprite::vertices);
    data.vertexData = std::unique_ptr<GLfloat[]>(new GLfloat[data.vertexDataSize / sizeof(GLfloat)]);
    memcpy(data.vertexData.get(), Sprite::vertices, data.vertexDataSize);

    data.indexDataSize = sizeof(Sprite::indices);
    data.indexData = std::unique_ptr<GLuint[]>(new GLuint[data.indexDataSize / sizeof(GLuint)]);
    memcpy(data.indexData.get(), Sprite::indices, data.indexDataSize);

    PrimitiveData::VertexAttribute coordinate = { 3, GL_FLOAT, 0 };
    PrimitiveData::VertexAttribute uv = { 2, GL_FLOAT, sizeof(float) * 12 };

    data.vertexAttributes.push_back(coordinate);
    data.vertexAttributes.push_back(uv);
    data.renderMode = GL_TRIANGLES;

    this->load(data);
}

}  // namespace Opengl

}  // namespace PolandBall
