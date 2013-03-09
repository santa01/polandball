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

#ifndef MESH_H
#define	MESH_H

#include "Vec3.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "IMovable.h"
#include "IScalable.h"
#include "IReplicable.h"
#include "IRotatable.h"
#include "INonCopyable.h"
#include "Texture.h"
#include "RenderEffect.h"

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>
#include <string>
#include <memory>

namespace PolandBall {

// NOTE: IRotateble only for animation purposes
class Sprite: public IMovable, public IRotatable, public IScalable, public IReplicable, public INonCopyable {
public:
    Sprite() {
        this->initialize();
    }

    Sprite(float x, float y, float z) {
        this->initialize();
        this->setPosition(x, y, z);
    }

    Sprite(const Math::Vec3& position) {
        this->initialize();
        this->setPosition(position);
    }

    ~Sprite() {
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(2, this->buffers);
    }

    using IMovable::setPosition;

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

    void replicateX(float factor) {
        this->replica.set(0, 0, this->replica.get(0, 0) * factor);
        this->scaleX(factor);
    }

    void replicateY(float factor) {
        this->replica.set(1, 1, this->replica.get(1, 1) * factor);
        this->scaleY(factor);
    }

    void replicateZ(float factor) {
        this->replica.set(2, 2, this->replica.get(2, 2) * factor);
        this->scaleZ(factor);
    }

    float getXReplicaFactor() const {
        return this->replica.get(0, 0);
    }

    float getYReplicaFactor() const {
        return this->replica.get(1, 1);
    }

    float getZReplicaFactor() const {
        return this->replica.get(2, 2);
    }

    std::shared_ptr<RenderEffect>& getEffect() {
        return this->effect;
    }

    void setEffect(std::shared_ptr<RenderEffect>& effect) {
        this->effect = effect;
    }

    std::shared_ptr<Texture>& getTexture() {
        return this->texture;
    }

    void setTexture(std::shared_ptr<Texture>& texture) {
        this->texture = texture;
    }

    void render();

private:
    enum {
        VERTEX_BUFFER = 0,
        ELEMENT_BUFFER = 1
    };

    std::shared_ptr<Texture> texture;
    std::shared_ptr<RenderEffect> effect;

    Math::Mat4 translation;
    Math::Mat4 rotation;
    Math::Mat4 scaling;
    Math::Mat4 replica;

    float xAngle;
    float yAngle;
    float zAngle;

    GLuint buffers[2];
    GLuint vao;

    static const float vertices[];
    static const int indices[];

    void initialize();
};

}  // namespace PolandBall

#endif  // MESH_H
