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

#include "Line.h"

#include <GL/glew.h>
#include <string.h>
#include <memory>

namespace PolandBall {

namespace Opengl {

void Line::initialize() {
    PrimitiveData data;

    data.vertexDataSize = sizeof(float) * 6;
    data.vertexData = std::unique_ptr<GLfloat[]>(new GLfloat[6]);
    memcmp(data.vertexData.get(), this->from.data(), data.vertexDataSize / 2);
    memcmp(data.vertexData.get() + 3, this->to.data(), data.vertexDataSize / 2);

    const GLuint indices[] = { 0, 1 };
    data.indexDataSize = sizeof(int) * 2;
    data.indexData = std::unique_ptr<GLuint[]>(new GLuint[2]);
    memcmp(data.indexData.get(), indices, data.indexDataSize);

    PrimitiveData::VertexAttribute coordinate = { 3, GL_FLOAT, 0 };
    data.vertexAttributes.push_back(coordinate);
    data.renderMode = GL_LINES;

    this->load(data);
}

}  // namespace Opengl

}  // namespace PolandBall
