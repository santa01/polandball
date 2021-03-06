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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Movable.h"
#include "Scalable.h"
#include "Rotatable.h"
#include "NonCopyable.h"
#include "RenderEffect.h"

#include <GL/glew.h>
#include <Vec3.h>
#include <Mat4.h>
#include <memory>
#include <vector>

namespace PolandBall {

namespace Opengl {

class Primitive: public Common::Movable, public Common::Rotatable, public Common::Scalable,
        public Common::NonCopyable {
public:
    Primitive() {
        this->xAngle = 0.0f;
        this->yAngle = 0.0f;
        this->zAngle = 0.0f;

        this->renderMode = GL_TRIANGLES;
        this->vertexCount = 0;

        glGenBuffers(2, this->buffers);
        glGenVertexArrays(1, &this->vao);
    }

    Primitive(float x, float y, float z):
            Primitive() {
        this->setPosition(x, y, z);
    }

    Primitive(const Math::Vec3& position):
            Primitive() {
        this->setPosition(position);
    }

    virtual ~Primitive() {
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(2, this->buffers);
    }

    using Movable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->translation.set(0, 3, position.get(Math::Vec3::X));
        this->translation.set(1, 3, position.get(Math::Vec3::Y));
        this->translation.set(2, 3, position.get(Math::Vec3::Z));
    }

    Math::Vec3 getPosition() const {
        return Math::Vec3(this->translation.get(0, 3),
                          this->translation.get(1, 3),
                          this->translation.get(2, 3));
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

    void scaleX(float factor) {
        this->scaling.set(0, 0, this->scaling.get(0, 0) * factor);
    }

    void scaleY(float factor) {
        this->scaling.set(1, 1, this->scaling.get(1, 1) * factor);
    }

    void scaleZ(float factor) {
        this->scaling.set(2, 2, this->scaling.get(2, 2) * factor);
    }

    float getXFactor() const {
        return this->scaling.get(0, 0);
    }

    float getYFactor() const {
        return this->scaling.get(1, 1);
    }

    float getZFactor() const {
        return this->scaling.get(2, 2);
    }

    std::shared_ptr<RenderEffect>& getEffect() {
        return this->effect;
    }

    void setEffect(std::shared_ptr<RenderEffect>& effect) {
        this->effect = effect;
    }

    void render();

protected:
    enum {
        VERTEX_BUFFER = 0,
        ELEMENT_BUFFER = 1
    };

    typedef struct {
        typedef struct {
            GLint componentsNumber;
            GLenum componentType;
            GLintptr attributeOffset;
        } VertexAttribute;

        std::unique_ptr<GLfloat[]> vertexData;
        GLsizei vertexDataSize;

        std::unique_ptr<GLuint[]> indexData;
        GLsizei indexDataSize;

        std::vector<VertexAttribute> vertexAttributes;
        GLenum renderMode;
    } PrimitiveData;

    virtual void beforeRender() {}
    virtual void afterRender() {}
    void load(const PrimitiveData& data);

    std::shared_ptr<RenderEffect> effect;

    Math::Mat4 translation;
    Math::Mat4 rotation;
    Math::Mat4 scaling;

    float xAngle;
    float yAngle;
    float zAngle;

    GLuint buffers[2];
    GLuint vao;

    GLenum renderMode;
    GLsizei vertexCount;
};

}  // namespace Opengl

}  // namespace PolandBall

#endif  // PRIMITIVE_H
