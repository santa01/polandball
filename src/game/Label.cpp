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
    this->textAspectRatio = 1.0f;

    // Keep data 2x2, 1x1 doesn't seem to work
    Uint32 data[] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000 };
    SDL_Surface* image = SDL_CreateRGBSurfaceFrom(&data, 2, 2, 32, 4,
            0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    auto texture = std::shared_ptr<Opengl::Texture>(new Opengl::Texture());
    texture->load(image);
    SDL_FreeSurface(image);
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
    if (!this->text.empty()) {
        int textWidth, textHeight;
        TTF_SizeUTF8(this->font.get(), text.c_str(), &textWidth, &textHeight);

        this->scaleX(1.0f / this->textAspectRatio);
        this->textAspectRatio = textWidth / (textHeight / 1.0f);
        this->scaleX(this->textAspectRatio);

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
