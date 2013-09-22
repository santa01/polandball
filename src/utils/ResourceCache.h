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

#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H

#include "Texture.h"
#include "RenderEffect.h"
#include "NonCopyable.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <json-c/json.h>
#include <unordered_map>
#include <string>
#include <memory>

namespace PolandBall {

namespace Utils {

class ResourceCache: public Common::NonCopyable {
public:
    std::shared_ptr<Opengl::Texture>& loadTexture(const std::string& name);
    std::shared_ptr<Opengl::RenderEffect>& loadEffect(const std::string& name);
    std::shared_ptr<json_object>& loadAsset(const std::string& name);
    std::shared_ptr<TTF_Font>& loadFont(const std::string& name, unsigned int size);

    void purge();

private:
    void insertTexture(const std::string& name, SDL_Surface* image) {
        std::shared_ptr<Opengl::Texture> texture(new Opengl::Texture());
        texture->load(image);
        this->textureCache.insert(std::make_pair(name, texture));
    }

    void insertEffect(const std::string& name, const std::string& source);
    std::unique_ptr<char[]> loadSource(const std::string& name) const;

    std::unordered_map<std::string, std::shared_ptr<Opengl::Texture>> textureCache;
    std::unordered_map<std::string, std::shared_ptr<Opengl::RenderEffect>> effectCache;
    std::unordered_map<std::string, std::shared_ptr<json_object>> assetCache;
    std::unordered_map<std::string, std::unordered_map<int, std::shared_ptr<TTF_Font>>> fontCache;
};

}  // namespace Utils

}  // namespace PolandBall

#endif  // RESOURCECACHE_H
