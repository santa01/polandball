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

#include "Label.h"
#include "Texture.h"
#include "RenderEffect.h"

#include <SDL2/SDL_image.h>

namespace PolandBall {

namespace Game {

Label::Label() {
    this->type = Entity::EntityType::TYPE_UI;
    this->widthScaleFactor = 1.0f;
    this->heightScaleFactor = 1.0f;

    GLint viewportParameters[4];
    glGetIntegerv(GL_VIEWPORT, viewportParameters);

    this->ndc.set(0, 0, 2.0f / (viewportParameters[2] / 1.0f));
    this->ndc.set(0, 3, -1.0f);
    this->ndc.set(1, 1, 2.0f / (viewportParameters[3] / 1.0f));
    this->ndc.set(1, 3, -1.0f);

    auto texture = std::shared_ptr<Opengl::Texture>(new Opengl::Texture());
    this->setTexture(texture);

    std::string vertexShader = "                                  \n\
        #version 330                                              \n\
                                                                  \n\
        uniform mat4 mvp;                                         \n\
        uniform mat4 lw;                                          \n\
                                                                  \n\
        layout(location = 0) in vec3 vertexPosition;              \n\
        layout(location = 1) in vec2 vertexUv;                    \n\
        smooth out vec2 fragmentUv;                               \n\
                                                                  \n\
        void main () {                                            \n\
            fragmentUv = vertexUv;                                \n\
            gl_Position = mvp * lw * vec4(vertexPosition, 1.0f);  \n\
        }";

    std::string fragmentShader = "                                \n\
        #version 330                                              \n\
                                                                  \n\
        uniform sampler2D textureSampler;                         \n\
                                                                  \n\
        smooth in vec2 fragmentUv;                                \n\
        out vec4 fragmentColor;                                   \n\
                                                                  \n\
        void main() {                                             \n\
            fragmentColor = texture(textureSampler, fragmentUv);  \n\
        }";

    auto effect = std::shared_ptr<Opengl::RenderEffect>(new Opengl::RenderEffect());
    effect->attachShader(vertexShader, Opengl::RenderEffect::TYPE_VERTEX);
    effect->attachShader(fragmentShader, Opengl::RenderEffect::TYPE_FRAGMENT);
    effect->enable();
    this->setEffect(effect);
}

void Label::renderText() {
    if (!this->text.empty() && this->font != nullptr) {
        int textWidth, textHeight;
        TTF_SizeUTF8(this->font.get(), text.c_str(), &textWidth, &textHeight);

        Math::Mat4 world(this->projection);
        world.invert();

        Math::Vec4 origin((world * this->ndc) * Math::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        Math::Vec4 size(textWidth, textHeight, 0.0f, 1.0f);
        size = (world * this->ndc) * size - origin;

        this->scaleX(1.0f / this->widthScaleFactor);
        this->widthScaleFactor = size.get(Math::Vec4::X);
        this->scaleX(this->widthScaleFactor);

        this->scaleY(1.0f / this->heightScaleFactor);
        this->heightScaleFactor = size.get(Math::Vec4::Y);
        this->scaleY(this->heightScaleFactor);

        SDL_Color color = {0, 0, 0, 0};
        SDL_Surface *textSurface = TTF_RenderUTF8_Blended(this->font.get(), text.c_str(), color);
        this->getTexture()->load(textSurface);
        SDL_FreeSurface(textSurface);
    } else {
        Uint32 data[] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000 };
        SDL_Surface* dummyImage = SDL_CreateRGBSurfaceFrom(&data, 2, 2, 32, 4,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        this->getTexture()->load(dummyImage);
    }
}

}  // namespace Game

}  // namespace Rubik
