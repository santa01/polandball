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

#ifndef SPRITE_H
#define SPRITE_H

#include "Transformable.h"
#include "Texture.h"
#include "Primitive.h"

#include <GL/glew.h>
#include <Mat4.h>
#include <string>
#include <memory>

namespace PolandBall {

namespace Opengl {

class Sprite: public Primitive, public Common::Transformable {
public:
    Sprite();

    void replicateX(float factor) {
        this->replication.set(0, 0, factor);
    }

    void replicateY(float factor) {
        this->replication.set(1, 1, factor);
    }

    void replicateZ(float factor) {
        this->replication.set(2, 2, factor);
    }

    float getXReplicaFactor() const {
        return this->replication.get(0, 0);
    }

    float getYReplicaFactor() const {
        return this->replication.get(1, 1);
    }

    float getZReplicaFactor() const {
        return this->replication.get(2, 2);
    }

    void shearX(float slice, int totalSlices) {
        if (slice < totalSlices) {
            this->shear.set(0, 0, 1.0f / totalSlices);
            this->shear.set(0, 3, slice / totalSlices);
        }
    }

    void shearY(float slice, int totalSlices) {
        if (slice < totalSlices) {
            this->shear.set(1, 1, 1.0f / totalSlices);
            this->shear.set(1, 3, slice / totalSlices);
        }
    }

    void shearZ(float slice, int totalSlices) {
        if (slice < totalSlices) {
            this->shear.set(2, 2, 1.0f / totalSlices);
            this->shear.set(2, 3, slice / totalSlices);
        }
    }

    void getXShearFactor(float& slice, int& totalSlices) const {
        totalSlices = 1.0f / this->shear.get(0, 0);
        slice = this->shear.get(0, 3) / this->shear.get(0, 0);
    }

    void getYShearFactor(float& slice, int& totalSlices) const {
        totalSlices = 1.0f / this->shear.get(1, 1);
        slice = this->shear.get(1, 3) / this->shear.get(1, 1);
    }

    void getZShearFactor(float& slice, int& totalSlices) const {
        totalSlices = 1.0f / this->shear.get(2, 2);
        slice = this->shear.get(2, 3) / this->shear.get(2, 2);
    }

    std::shared_ptr<Texture>& getTexture() {
        return this->texture;
    }

    void setTexture(std::shared_ptr<Texture>& texture) {
        this->texture = texture;
    }

private:
    void onRender() {
        if (this->texture == nullptr) {
            return;
        }

        this->texture->bind();
        this->effect->enable();
        this->effect->setUniform("transform", this->replication * this->shear);
    }

    std::shared_ptr<Texture> texture;

    Math::Mat4 replication;
    Math::Mat4 shear;

    static const GLfloat vertices[];
    static const GLuint indices[];
};

}  // namespace Opengl

}  // namespace PolandBall

#endif  // SPRITE_H
