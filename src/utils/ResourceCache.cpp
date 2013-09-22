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

#include "ResourceCache.h"
#include "Logger.h"
#include "ShaderLoader.h"

#include <fstream>

namespace PolandBall {

namespace Utils {

std::shared_ptr<Opengl::Texture>& ResourceCache::loadTexture(const std::string& name) {
    if (this->textureCache.find(name) == this->textureCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Image `%s' not in cache, trying to load", name.c_str());

        SDL_Surface* image = IMG_Load(this->buildPath(name).c_str());
        if (image == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "IMG_Load() failed: %s", IMG_GetError());
            return this->textureCache["nullptr"];
        }

        std::shared_ptr<Opengl::Texture> texture(new Opengl::Texture());
        texture->load(image);
        this->textureCache.insert(std::make_pair(name, texture));

        SDL_FreeSurface(image);
    } else {
        Logger::getInstance().log(Logger::LOG_INFO, "Image `%s' picked from cache", name.c_str());
    }

    return this->textureCache.at(name);
}

std::shared_ptr<Opengl::RenderEffect>& ResourceCache::loadEffect(const std::string& name) {
    if (this->effectCache.find(name) == this->effectCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Shader `%s' not in cache, trying to load", name.c_str());

        std::string fullPath(this->buildPath(name).c_str());
        auto shaderSource = this->loadSource(fullPath.c_str());
        if (shaderSource == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Failed to open `%s'", fullPath.c_str());
            return this->effectCache["nullptr"];
        }

        this->insertEffect(name, shaderSource.get());
    } else {
        Logger::getInstance().log(Logger::LOG_INFO, "Shader `%s' picked from cache", name.c_str());
    }

    return this->effectCache.at(name);
}

std::shared_ptr<json_object>& ResourceCache::loadAsset(const std::string& name) {
    std::shared_ptr<json_object> object;

    if (this->assetCache.find(name) == this->assetCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Asset `%s' not in cache, trying to load", name.c_str());

        std::string fullPath(this->buildPath(name).c_str());
        auto entitySource = this->loadSource(fullPath.c_str());
        if (entitySource == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Failed to open `%s'", fullPath.c_str());
            return this->assetCache["nullptr"];
        }

        json_tokener_error parseError;
        object = std::shared_ptr<json_object>(
                json_tokener_parse_verbose(entitySource.get(), &parseError), json_object_put);
        if (object == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Failed to parse `%s': %s",
                    fullPath.c_str(), json_tokener_error_desc(parseError));
            return this->assetCache["nullptr"];
        }

        this->assetCache.insert(std::make_pair(name, object));
    } else {
        Logger::getInstance().log(Logger::LOG_INFO, "Asset `%s' picked from cache", name.c_str());
    }

    return this->assetCache.at(name);
}

std::shared_ptr<TTF_Font>& ResourceCache::loadFont(const std::string& name, unsigned int size) {
    if (this->fontCache[name].find(size) == this->fontCache[name].end()) {
        Logger::getInstance().log(Logger::LOG_INFO,
                "Font `%s' (%dpt) not in cache, trying to load", name.c_str(), size);

        std::shared_ptr<TTF_Font> font(TTF_OpenFont(this->buildPath(name).c_str(), size), TTF_CloseFont);
        if (font == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "TTF_OpenFont() failed: %s", TTF_GetError());
            return this->fontCache[name][-1];
        }

        this->fontCache[name].insert(std::make_pair(size, font));
    } else {
        Logger::getInstance().log(Logger::LOG_INFO,
                "Font `%s' (%dpt) picked from cache", name.c_str(), size);
    }

    return this->fontCache[name][size];
}

void ResourceCache::purge() {
    for (auto& texture: this->textureCache) {
        if (!texture.second.unique() && texture.second != nullptr) {
            Logger::getInstance().log(Logger::LOG_WARNING, "Texture %p has %d references left!",
                    texture.second.get(), texture.second.use_count() - 1);
        }
    }

    for (auto& effect: this->effectCache) {
        if (!effect.second.unique() && effect.second != nullptr) {
            Logger::getInstance().log(Logger::LOG_WARNING, "RenderEffect %p has %d references left!",
                    effect.second.get(), effect.second.use_count() - 1);
        }
    }

    for (auto& asset: this->assetCache) {
        if (!asset.second.unique() && asset.second != nullptr) {
            Logger::getInstance().log(Logger::LOG_WARNING, "Asset %p has %d references left!",
                    asset.second.get(), asset.second.use_count() - 1);
        }
    }

    for (auto& fontName: this->fontCache) {
        for (auto& font: fontName.second) {
            if (!font.second.unique() && font.second != nullptr) {
                Logger::getInstance().log(Logger::LOG_WARNING, "Font %p has %d references left!",
                        font.second.get(), font.second.use_count() - 1);
            }
        }

        fontName.second.clear();
    }

    this->textureCache.clear();
    this->effectCache.clear();
    this->assetCache.clear();
    this->fontCache.clear();
}

void ResourceCache::insertEffect(const std::string& name, const std::string& source) {
    std::shared_ptr<Opengl::RenderEffect> effect(new Opengl::RenderEffect());

    std::stringstream modifiedSource;
    modifiedSource << "#define TYPE_VERTEX\n";
    modifiedSource << source;
    effect->attachShader(modifiedSource.str(), Opengl::RenderEffect::ShaderType::TYPE_VERTEX);

    modifiedSource.str("");
    modifiedSource << "#define TYPE_FRAGMENT\n";
    modifiedSource << source;
    effect->attachShader(modifiedSource.str(), Opengl::RenderEffect::ShaderType::TYPE_FRAGMENT);

    effect->enable();  // Compile
    this->effectCache.insert(std::make_pair(name, effect));
}

std::unique_ptr<char[]> ResourceCache::loadSource(const std::string& name) const {
    std::fstream file(name.c_str(), std::ios::binary | std::ios::in);
    if (!file.good()) {
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    int sourceLength = file.tellg();

    std::unique_ptr<char[]> source(new char[sourceLength + 1]);
    source[sourceLength] = '\0';

    file.seekg(0, std::ios::beg);
    file.read(source.get(), sourceLength);
    file.close();

    return source;
}

}  // namespace Utils

}  // namespace PolandBall
