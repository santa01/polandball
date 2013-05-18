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
#include <bits/shared_ptr_base.h>

namespace PolandBall {

namespace Utils {

const std::string ResourceManager::defaultShader = "                  \n\
    #version 330                                                      \n\
                                                                      \n\
    #ifdef TYPE_VERTEX                                                \n\
        uniform mat4 mvp;                                             \n\
        uniform mat4 lw;                                              \n\
        uniform mat4 transform;                                       \n\
                                                                      \n\
        layout(location = 0) in vec3 vertexPosition;                  \n\
        layout(location = 1) in vec2 vertexUv;                        \n\
                                                                      \n\
        smooth out vec2 fragmentUv;                                   \n\
                                                                      \n\
        void main () {                                                \n\
            fragmentUv = (transform * vec4(vertexUv, 0.0f, 1.0f)).st; \n\
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

ResourceManager::ResourceManager() {
        // Keep data 2x2, 1x1 doesn't seem to work on a cleared color buffer
        Uint32 green[] = { 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00 };
        SDL_Surface* image = SDL_CreateRGBSurfaceFrom(&green, 2, 2, 32, 4,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        this->insertTexture("default", image);
        SDL_FreeSurface(image);

        // Handy for "empty" entites
        Uint32 transparent[] = { 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000 };
        image = SDL_CreateRGBSurfaceFrom(&transparent, 2, 2, 32, 4,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        this->insertTexture("transparent", image);
        SDL_FreeSurface(image);

        this->insertEffect("default", ResourceManager::defaultShader);
    }

std::shared_ptr<Opengl::Texture>& ResourceManager::makeTexture(const std::string& name) {
    if (this->textureCache.find(name) == this->textureCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Loading image `%s'", name.c_str());

        SDL_Surface* image = IMG_Load(name.c_str());
        if (image == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "IMG_Load(%s) failed: %s", name.c_str(), IMG_GetError());
            return this->textureCache.at("default");
        }

        std::shared_ptr<Opengl::Texture> texture(new Opengl::Texture());
        texture->load(image);
        this->textureCache.insert(std::make_pair(name, texture));

        SDL_FreeSurface(image);
    }

    return this->textureCache.at(name);
}

std::shared_ptr<Opengl::RenderEffect>& ResourceManager::makeEffect(const std::string& name) {
    if (this->effectCache.find(name) == this->effectCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Loading shader `%s'", name.c_str());

        std::unique_ptr<char[]> shaderSource = this->loadSource(name);
        if (shaderSource == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Cannot open `%s'", name.c_str());
            return this->effectCache.at("default");
        }

        this->insertEffect(name, shaderSource.get());
    }

    return this->effectCache.at(name);
}

std::shared_ptr<Game::Entity> ResourceManager::makeEntity(const std::string& name) {
    std::shared_ptr<json_object> object;
    json_object* object_type;

    if (this->entityCache.find(name) == this->entityCache.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Loading entity `%s'", name.c_str());

        std::unique_ptr<char[]> entitySource = this->loadSource(name);
        if (entitySource == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Cannot open `%s'", name.c_str());
            return std::shared_ptr<Game::Entity>(new Game::Entity());
        }

        json_tokener_error parseError;
        object = std::shared_ptr<json_object>(json_tokener_parse_verbose(entitySource.get(), &parseError), json_object_put);
        if (object == nullptr) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Cannot parse `%s': %s",
                    name.c_str(), json_tokener_error_desc(parseError));
            return std::shared_ptr<Game::Entity>(new Game::Entity());
        }

        object_type = json_object_object_get(object.get(), "type");
        if (object_type == nullptr || json_object_get_type(object_type) != json_type_string) {
            Logger::getInstance().log(Logger::LOG_ERROR, "Failed to read `type' parameter");
            return std::shared_ptr<Game::Entity>(new Game::Entity());
        }

        this->entityCache.insert(std::make_pair(name, object));
    } else {
        object = this->entityCache.at(name);
        object_type = json_object_object_get(object.get(), "type");
    }

    std::shared_ptr<Game::Entity> entity;
    std::string entityType(json_object_get_string(object_type));

    if (entityType == "pack") {
        entity = this->loadPack(object);
    } else if (entityType == "player") {
        entity = this->loadPlayer(object);
    } else if (entityType == "weapon") {
        entity = this->loadWeapon(object);
    } else if (entityType == "solid") {
        entity = std::shared_ptr<Game::Entity>(new Game::Entity(Game::Entity::EntityType::TYPE_SOLID));
    } else if (entityType == "clip") {
        entity = std::shared_ptr<Game::Entity>(new Game::Entity(Game::Entity::EntityType::TYPE_CLIP));
    } else if (entityType == "passable") {
        entity = std::shared_ptr<Game::Entity>(new Game::Entity(Game::Entity::EntityType::TYPE_PASSABLE));
    } else {
        Logger::getInstance().log(Logger::LOG_WARNING, "Got unknown `type' value `%s'", entityType.c_str());
        entity = std::shared_ptr<Game::Entity>(new Game::Entity());
    }

    if (entity->getType() != Game::Entity::EntityType::TYPE_CLIP) {
        json_object* texture = json_object_object_get(object.get(), "texture");
        if (texture == nullptr || json_object_get_type(texture) != json_type_string) {
            Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `texture' parameter");
        } else {
            entity->setTexture(this->makeTexture(json_object_get_string(texture)));
        }

        json_object* effect = json_object_object_get(object.get(), "effect");
        if (effect == nullptr || json_object_get_type(effect) != json_type_string) {
            Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `effect' parameter");
        } else {
            entity->setEffect(this->makeEffect(json_object_get_string(effect)));
        }
    }

    return entity;
}

void ResourceManager::purgeCaches() {
    for (auto& texture: this->textureCache) {
        if (!texture.second.unique()) {
            Logger::getInstance().log(Logger::LOG_WARNING, "Texture %p has %d references left!",
                    texture.second.get(), texture.second.use_count() - 1);
        }
    }

    for (auto& effect: this->effectCache) {
        if (!effect.second.unique()) {
            Logger::getInstance().log(Logger::LOG_WARNING, "RenderEffect %p has %d references left!",
                    effect.second.get(), effect.second.use_count() - 1);
        }
    }

    this->textureCache.clear();
    this->effectCache.clear();
    this->entityCache.clear();
}

void ResourceManager::insertEffect(const std::string& name, const std::string& source) {
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

std::unique_ptr<char[]> ResourceManager::loadSource(const std::string& name) const {
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

std::shared_ptr<Game::Pack> ResourceManager::loadPack(const std::shared_ptr<json_object>& object) const {
    std::shared_ptr<Game::Pack> pack(new Game::Pack());

    json_object* payload = json_object_object_get(object.get(), "payload");
    if (payload == nullptr || json_object_get_type(payload) != json_type_string) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `payload' parameter");
    } else {
        std::string payloadValue(json_object_get_string(payload));
        Game::Pack::PayloadType payloadType = pack->getPayloadType();

        if (payloadValue == "health") {
            payloadType = Game::Pack::PayloadType::TYPE_HEALTH;
        } else if (payloadValue == "armor") {
            payloadType = Game::Pack::PayloadType::TYPE_ARMOR;
        } else if (payloadValue == "primary_ammo") {
            payloadType = Game::Pack::PayloadType::TYPE_PRIMARY_AMMO;
        } else if (payloadValue == "secondary_ammo") {
            payloadType = Game::Pack::PayloadType::TYPE_SECONDARY_AMMO;
        } else {
            Logger::getInstance().log(Logger::LOG_WARNING, "Got unknown `payload' value `%s'", payloadValue.c_str());
        }

        pack->setPayloadType(payloadType);
    }

    json_object* value = json_object_object_get(object.get(), "value");
    if (value == nullptr || json_object_get_type(value) != json_type_int) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `value' parameter");
    } else {
        pack->setValue(json_object_get_int(value));
    }

    return pack;
}

std::shared_ptr<Game::Player> ResourceManager::loadPlayer(const std::shared_ptr<json_object>& object) const {
    std::shared_ptr<Game::Player> player(new Game::Player());

    json_object* maxMoveSpeed = json_object_object_get(object.get(), "max_move_speed");
    if (maxMoveSpeed == nullptr || json_object_get_type(maxMoveSpeed) != json_type_double) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `max_move_speed' parameter");
    } else {
        player->setMaxMoveSpeed(json_object_get_double(maxMoveSpeed));
    }

    json_object* maxJumpSpeed = json_object_object_get(object.get(), "max_jump_speed");
    if (maxJumpSpeed == nullptr || json_object_get_type(maxJumpSpeed) != json_type_double) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `max_jump_speed' parameter");
    } else {
        player->setMaxJumpSpeed(json_object_get_double(maxJumpSpeed));
    }

    json_object* maxJumpTime = json_object_object_get(object.get(), "max_jump_time");
    if (maxJumpTime == nullptr || json_object_get_type(maxJumpTime) != json_type_double) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `max_jump_time' parameter");
    } else {
        player->setMaxJumpTime(json_object_get_double(maxJumpTime));
    }

    json_object* maxHealth = json_object_object_get(object.get(), "max_health");
    if (maxHealth == nullptr || json_object_get_type(maxHealth) != json_type_int) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `max_health' parameter");
    } else {
        player->setMaxHealth(json_object_get_int(maxHealth));
    }

    json_object* maxArmor = json_object_object_get(object.get(), "max_armor");
    if (maxArmor == nullptr || json_object_get_type(maxArmor) != json_type_int) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `max_armor' parameter");
    } else {
        player->setMaxArmor(json_object_get_int(maxArmor));
    }

    json_object* health = json_object_object_get(object.get(), "health");
    if (health == nullptr || json_object_get_type(health) != json_type_int) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `health' parameter");
    } else {
        player->setHealth(json_object_get_int(health));
    }

    json_object* armor = json_object_object_get(object.get(), "armor");
    if (armor == nullptr || json_object_get_type(armor) != json_type_int) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `armor' parameter");
    } else {
        player->setArmor(json_object_get_int(armor));
    }

    return player;
}

std::shared_ptr<Game::Weapon> ResourceManager::loadWeapon(const std::shared_ptr<json_object>& object) const {
    std::shared_ptr<Game::Weapon> weapon(new Game::Weapon());

    json_object* slot = json_object_object_get(object.get(), "target_slot");
    if (slot == nullptr || json_object_get_type(slot) != json_type_string) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `target_slot' parameter");
    } else {
        std::string slotValue(json_object_get_string(slot));
        Game::Weapon::WeaponSlot targetSlot = weapon->getTargetSlot();

        if (slotValue == "primary") {
            targetSlot = Game::Weapon::WeaponSlot::SLOT_PRIMARY;
        } else if (slotValue == "secondary") {
            targetSlot = Game::Weapon::WeaponSlot::SLOT_SECONDARY;
        } else if (slotValue == "meele") {
            targetSlot = Game::Weapon::WeaponSlot::SLOT_MEELE;
        } else {
            Logger::getInstance().log(Logger::LOG_WARNING, "Got unknown `target_slot' value `%s'", slotValue.c_str());
        }

        weapon->setTargetSlot(targetSlot);
    }

    json_object* maxAmmo = json_object_object_get(object.get(), "max_ammo");
    if (maxAmmo == nullptr || json_object_get_type(maxAmmo) != json_type_int) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `max_ammo' parameter");
    } else {
        weapon->setMaxAmmo(json_object_get_int(maxAmmo));
    }

    json_object* ammo = json_object_object_get(object.get(), "ammo");
    if (ammo == nullptr || json_object_get_type(ammo) != json_type_int) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `ammo' parameter");
    } else {
        weapon->setAmmo(json_object_get_int(ammo));
    }

    json_object* groupingAngle = json_object_object_get(object.get(), "grouping_angle");
    if (groupingAngle == nullptr || json_object_get_type(groupingAngle) != json_type_double) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `grouping_angle' parameter");
    } else {
        weapon->setGroupingAngle(json_object_get_double(groupingAngle));
    }

    json_object* firingSpeed = json_object_object_get(object.get(), "firing_speed");
    if (firingSpeed == nullptr || json_object_get_type(firingSpeed) != json_type_double) {
        Logger::getInstance().log(Logger::LOG_WARNING, "Failed to read `firing_speed' parameter");
    } else {
        weapon->setFiringSpeed(json_object_get_double(firingSpeed));
    }

    return weapon;
}

}  // namespace Utils

}  // namespace PolandBall
