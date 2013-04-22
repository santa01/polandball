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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Texture.h"
#include "RenderEffect.h"
#include "NonCopyable.h"

#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <memory>

namespace PolandBall {

namespace Utils {

class ResourceManager: public NonCopyable {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    std::shared_ptr<Texture>& makeTexture(const std::string& name);
    std::shared_ptr<RenderEffect>& makeEffect(const std::string& name);

private:
    ResourceManager();

    void insertTexture(const std::string& name, SDL_Surface* image) {
        std::shared_ptr<Texture> texture(new Texture());
        texture->load(image);
        this->textureCache.insert(std::make_pair(name, texture));
    }

    void insertEffect(const std::string& name, const std::string& source);

    std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;
    std::unordered_map<std::string, std::shared_ptr<RenderEffect>> effectCache;

    static const std::string defaultShader;
};

}  // namespace Utils

}  // namespace PolandBall

#endif  // RESOURCEMANAGER_H
