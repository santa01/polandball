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

#include <NonCopyable.h>
#include <Font.h>
#include <json-c/json.h>
#include <unordered_map>
#include <string>
#include <memory>

#define GetResourceCache() ResourceCache::getInstance()

namespace PolandBall {

namespace Utils {

class ResourceCache: public Graphene::NonCopyable {
public:
    static ResourceCache& getInstance();

    const std::shared_ptr<json_object>& loadAsset(const std::string& name);
    const std::shared_ptr<Graphene::Font>& loadFont(const std::string& name, int size);

    void teardown();

private:
    ResourceCache() = default;

    std::unordered_map<std::string, std::shared_ptr<json_object>> assetCache;
    std::unordered_map<std::string, std::shared_ptr<Graphene::Font>> fontCache;
};

}  // namespace Utils

}  // namespace PolandBall

#endif  // RESOURCECACHE_H
