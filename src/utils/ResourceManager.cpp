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

#include "ResourceManager.h"
#include "Logger.h"
#include "ShaderLoader.h"

#include <utility>
#include <fstream>
#include <sstream>

namespace PolandBall {

namespace Utils {

const std::string ResourceManager::defaultShader = "                  \n\
    #version 330                                                      \n\
                                                                      \n\
    #ifdef TYPE_VERTEX                                                \n\
        uniform mat4 mvp;                                             \n\
        uniform mat4 lw;                                              \n\
        uniform mat4 replica;                                         \n\
                                                                      \n\
        layout(location = 0) in vec3 vertexPosition;                  \n\
        layout(location = 1) in vec2 vertexUv;                        \n\
                                                                      \n\
        smooth out vec2 fragmentUv;                                   \n\
                                                                      \n\
        void main () {                                                \n\
            fragmentUv = (replica * vec4(vertexUv, 1.0f, 1.0f)).st;   \n\
            gl_Position = mvp * lw * vec4(vertexPosition, 1.0f);      \n\
        }                                                             \n\
    #endif                                                            \n\
                                                                      \n\
    #ifdef TYPE_FRAGMENT                                              \n\
        uniform sampler2D textureSampler;                             \n\
                                                                      \n\
        smooth in vec2 fragmentUv;                                    \n\
                                                                      \n\
        out vec4 fragmentColor;                                       \n\
                                                                      \n\
        void main() {                                                 \n\
            fragmentColor = texture(textureSampler, fragmentUv);      \n\
        }                                                             \n\
    #endif                                                            \n";

std::shared_ptr<Texture>& ResourceManager::makeTexture(const std::string& name) {
    if (this->textureCache.find(name) == this->textureCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Loading image `%s'", name.c_str());

        SDL_Surface* image = IMG_Load(name.c_str());
        if (!image) {
            Logger::getInstance().log(Logger::LOG_ERROR, "IMG_Load(%s) failed: %s", name.c_str(), IMG_GetError());
            return this->textureCache.at("default");
        }

        std::shared_ptr<Texture> texture(new Texture());
        texture->load(image);
        this->textureCache.insert(std::make_pair(name, texture));

        SDL_FreeSurface(image);
    }

    return this->textureCache.at(name);
}

std::shared_ptr<RenderEffect>& ResourceManager::makeEffect(const std::string& name) {
    if (this->effectCache.find(name) == this->effectCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Loading shader `%s'", name.c_str());

        std::fstream file(name.c_str(), std::ios::binary | std::ios::in);
        if (!file.good()) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Cannot open file %s", name.c_str());
            return this->effectCache.at("default");
        }

        file.seekg(0, std::ios::end);
        int sourceLength = file.tellg();

        std::unique_ptr<char[]> shaderSource(new char[sourceLength + 1]);
        shaderSource[sourceLength] = '\0';

        file.seekg(0, std::ios::beg);
        file.read(shaderSource.get(), sourceLength);
        file.close();

        this->insertEffect(name, shaderSource.get());
    }

    return this->effectCache.at(name);
}

void ResourceManager::insertEffect(const std::string& name, const std::string& source) {
    std::shared_ptr<RenderEffect> effect(new RenderEffect());

    std::stringstream modifiedSource;
    modifiedSource << "#define TYPE_VERTEX\n";
    modifiedSource << source;
    effect->attachShader(modifiedSource.str(), RenderEffect::ShaderType::TYPE_VERTEX);

    modifiedSource.str("");
    modifiedSource << "#define TYPE_FRAGMENT\n";
    modifiedSource << source;
    effect->attachShader(modifiedSource.str(), RenderEffect::ShaderType::TYPE_FRAGMENT);

    effect->enable();  // Compile
    this->effectCache.insert(std::make_pair(name, effect));
}

}  // namespace Utils

}  // namespace PolandBall
