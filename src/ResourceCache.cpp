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

#include <ResourceCache.h>
#include <EngineConfig.h>
#include <Config.h>
#include <Logger.h>
#include <fstream>
#include <stdexcept>

namespace PolandBall {

ResourceCache& ResourceCache::getInstance() {
    static ResourceCache instance;
    return instance;
}

const std::shared_ptr<json_object>& ResourceCache::loadAsset(const std::string& name) {
    auto assetIt = this->assetCache.find(name);
    if (assetIt != this->assetCache.end()) {
        Graphene::LogDebug("Reuse cached '%s' asset", name.c_str());
        return assetIt->second;
    }

    Graphene::LogDebug("Load asset from '%s'", name.c_str());

    std::ifstream file(Graphene::GetEngineConfig().getDataDirectory() + '/' + name, std::ios::binary);
    if (!file) {
        throw std::runtime_error(Graphene::LogFormat("Failed to open '%s'", name.c_str()));
    }

    file.seekg(0, std::ios::end);
    std::ifstream::pos_type sourceLength = file.tellg();
    file.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> source(new char[sourceLength]);
    file.read(source.get(), sourceLength);

    std::string jsonSource(source.get(), sourceLength);

    json_tokener_error parseError;
    auto jsonObject = std::shared_ptr<json_object>(json_tokener_parse_verbose(jsonSource.c_str(), &parseError), json_object_put);
    if (jsonObject == nullptr) {
        throw std::runtime_error(Graphene::LogFormat("Failed to parse '%s': %s", name.c_str(), json_tokener_error_desc(parseError)));
    }

    return this->assetCache.emplace(name, jsonObject).first->second;
}

const std::shared_ptr<Graphene::Font>& ResourceCache::loadFont(const std::string& name, int size) {
    auto fontIt = this->fontCache.find(name);
    if (fontIt != this->fontCache.end()) {
        Graphene::LogDebug("Reuse cached '%s' font", name.c_str());
        return fontIt->second;
    }

    Graphene::LogDebug("Load font from '%s'", name.c_str());
    auto font = std::make_shared<Graphene::Font>(name, size);

    return this->fontCache.emplace(name, font).first->second;
}

void ResourceCache::teardown() {
    for (auto& asset: this->assetCache) {
        long externalReferences = asset.second.use_count() - 1;
        if (externalReferences > 0) {
            Graphene::LogWarn("Asset '%s' has %d external references", asset.first.c_str(), externalReferences);
        }
    }

    for (auto& font: this->fontCache) {
        long externalReferences = font.second.use_count() - 1;
        if (externalReferences > 0) {
            Graphene::LogWarn("Font '%s' has %d external references", font.first.c_str(), externalReferences);
        }
    }

    this->assetCache.clear();
    this->fontCache.clear();
}

}  // namespace PolandBall
