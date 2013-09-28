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

#include "Primitive.h"

#include <Quaternion.h>

namespace PolandBall {

namespace Opengl {

void Primitive::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        return;
    }

    Math::Vec3 axis(vector);
    Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
    q.normalize();

    this->rotation = q.extractMat4();

    float xAngleNew, yAngleNew, zAngleNew;
    q.extractEulerAngles(xAngleNew, yAngleNew, zAngleNew);

    this->xAngle = xAngleNew * 180.f / M_PI;
    this->yAngle = yAngleNew * 180.f / M_PI;
    this->zAngle = zAngleNew * 180.f / M_PI;
}

void Primitive::render() {
    if (this->effect == nullptr) {
        return;
    }

    this->effect->enable();
    this->effect->setUniform("lw", this->translation * this->rotation * this->scaling);
    this->beforeRender();

    glBindVertexArray(this->vao);
    glDrawElements(this->renderMode, this->vertexCount, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);

    this->afterRender();
    this->effect->disable();
}

void Primitive::load(const PrimitiveData& data) {
    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, data.vertexDataSize, data.vertexData.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[ELEMENT_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indexDataSize, data.indexData.get(), GL_STATIC_DRAW);

    for (unsigned int i = 0; i < data.vertexAttributes.size(); i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, data.vertexAttributes[i].componentsNumber,
                data.vertexAttributes[i].componentType, GL_FALSE, 0,
                (GLvoid*)data.vertexAttributes[i].attributeOffset);
    }

    glBindVertexArray(0);

    this->renderMode = data.renderMode;
    this->vertexCount = data.indexDataSize / sizeof(GLuint);
}

}  // namespace Opengl

}  // namespace PolandBall
