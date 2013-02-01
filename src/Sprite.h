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
#include "IMovable.h"
#include "IScalable.h"
#include "IRotatable.h"
#include "Texture.h"
#include "RenderEffect.h"

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>
#include <string>
#include <memory>

namespace PolandBall {

class Sprite: public IMovable, public IRotatable, public IScalable {
public:
    Sprite();
    Sprite(float x, float y, float z);
    Sprite(const Math::Vec3& position);
    ~Sprite();

    using IMovable::setPosition;

    void setPosition(const Math::Vec3& position);
    Math::Vec3 getPosition() const;

    float getXAngle() const;
    float getYAngle() const;
    float getZAngle() const;

    void rotate(const Math::Vec3& vector, float angle);

    void scaleX(float factor);
    void scaleY(float factor);
    void scaleZ(float factor);

    float getXFactor() const;
    float getYFactor() const;
    float getZFactor() const;

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

    Sprite(const Sprite& orig) = delete;
    Sprite& operator =(const Sprite&) = delete;

    std::shared_ptr<Texture> texture;
    std::shared_ptr<RenderEffect> effect;

    Math::Mat4 translation;
    Math::Mat4 rotation;
    Math::Mat4 scaling;

    GLuint buffers[2];
    GLuint vao;

    static const float vertices[];
    static const int indices[];

    void initialize();
};

}

// namespace PolandBall

#endif  // MESH_H
