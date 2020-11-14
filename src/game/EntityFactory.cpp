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

#include <EntityFactory.h>
#include <ObjectManager.h>
#include <ResourceCache.h>
#include <Material.h>
#include <Logger.h>

namespace PolandBall {

namespace Game {

const std::shared_ptr<Pack> EntityFactory::createPack(const std::string& name) const {
    auto& asset = Utils::GetResourceCache().loadAsset(name);
    auto pack = std::make_shared<Pack>();

    this->loadBase(asset, pack);

    json_object* payloadObject = this->getJsonObject(asset, "payload", json_type_string);
    if (payloadObject != nullptr) {
        std::string payloadValue(json_object_get_string(payloadObject));
        Game::PayloadType payloadType = pack->getPayloadType();

        if (payloadValue == "health") {
            payloadType = Game::PayloadType::HEALTH;
        } else if (payloadValue == "armor") {
            payloadType = Game::PayloadType::ARMOR;
        } else if (payloadValue == "primary_ammo") {
            payloadType = Game::PayloadType::PRIMARY_AMMO;
        } else if (payloadValue == "secondary_ammo") {
            payloadType = Game::PayloadType::SECONDARY_AMMO;
        } else {
            Graphene::LogWarn("Got unknown 'payload' value '%s'", payloadValue.c_str());
        }

        pack->setPayloadType(payloadType);
    }

    json_object* valueObject = this->getJsonObject(asset, "value", json_type_int);
    if (valueObject != nullptr) {
        pack->setValue(json_object_get_int(valueObject));
    }

    return pack;
}

const std::shared_ptr<Player> EntityFactory::createPlayer(const std::string& name) const {
    auto& asset = Utils::GetResourceCache().loadAsset(name);
    auto player = std::make_shared<Player>();

    this->loadBase(asset, player);

    json_object* maxMoveSpeedObject = this->getJsonObject(asset, "max_move_speed", json_type_double);
    if (maxMoveSpeedObject != nullptr) {
        player->setMaxMoveSpeed(static_cast<float>(json_object_get_double(maxMoveSpeedObject)));
    }

    json_object* maxJumpSpeedObject = this->getJsonObject(asset, "max_jump_speed", json_type_double);
    if (maxJumpSpeedObject != nullptr) {
        player->setMaxJumpSpeed(static_cast<float>(json_object_get_double(maxJumpSpeedObject)));
    }

    json_object* maxJumpTimeObject = this->getJsonObject(asset, "max_jump_time", json_type_double);
    if (maxJumpTimeObject != nullptr) {
        player->setMaxJumpTime(static_cast<float>(json_object_get_double(maxJumpTimeObject)));
    }

    json_object* maxHealthObject = this->getJsonObject(asset, "max_health", json_type_int);
    if (maxHealthObject != nullptr) {
        player->setMaxHealth(json_object_get_int(maxHealthObject));
    }

    json_object* maxArmorObject = this->getJsonObject(asset, "max_armor", json_type_int);
    if (maxArmorObject != nullptr) {
        player->setMaxArmor(json_object_get_int(maxArmorObject));
    }

    json_object* healthObject = this->getJsonObject(asset, "health", json_type_int);
    if (healthObject != nullptr) {
        player->setHealth(json_object_get_int(healthObject));
    }

    json_object* armorObject = this->getJsonObject(asset, "armor", json_type_int);
    if (armorObject != nullptr) {
        player->setArmor(json_object_get_int(armorObject));
    }

    return player;
}

const std::shared_ptr<Weapon> EntityFactory::createWeapon(const std::string& name) const {
    auto& asset = Utils::GetResourceCache().loadAsset(name);
    auto weapon = std::make_shared<Weapon>();

    this->loadBase(asset, weapon);

    json_object* slotObject = this->getJsonObject(asset, "target_slot", json_type_string);
    if (slotObject != nullptr) {
        std::string slotValue(json_object_get_string(slotObject));
        Game::WeaponSlot targetSlot = weapon->getTargetSlot();

        if (slotValue == "primary") {
            targetSlot = Game::WeaponSlot::SLOT_PRIMARY;
        } else if (slotValue == "secondary") {
            targetSlot = Game::WeaponSlot::SLOT_SECONDARY;
        } else if (slotValue == "meele") {
            targetSlot = Game::WeaponSlot::SLOT_MEELE;
        } else {
            Graphene::LogWarn("Unknown 'target_slot' value '%s'", slotValue.c_str());
        }

        weapon->setTargetSlot(targetSlot);
    }

    json_object* maxAmmoObject = this->getJsonObject(asset, "max_ammo", json_type_int);
    if (maxAmmoObject != nullptr) {
        weapon->setMaxAmmo(json_object_get_int(maxAmmoObject));
    }

    json_object* ammoObject = this->getJsonObject(asset, "ammo", json_type_int);
    if (ammoObject != nullptr) {
        weapon->setAmmo(json_object_get_int(ammoObject));
    }

    json_object* groupingAngleObject = this->getJsonObject(asset, "grouping_angle", json_type_double);
    if (groupingAngleObject != nullptr) {
        weapon->setGroupingAngle(static_cast<float>(json_object_get_double(groupingAngleObject)));
    }

    json_object* firingSpeedObject = this->getJsonObject(asset, "firing_speed", json_type_double);
    if (firingSpeedObject != nullptr) {
        weapon->setFiringSpeed(static_cast<float>(json_object_get_double(firingSpeedObject)));
    }

    return weapon;
}

// std::shared_ptr<Game::ShotTrace> EntityFactory::createTrace(const Math::Vec3& from, const Math::Vec3& to) const {
//     Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building ShotTrace from (%.3f; %.3f) to (%.3f; %.3f)",
//             from.get(Math::Vec3::X), from.get(Math::Vec3::Y), to.get(Math::Vec3::X), to.get(Math::Vec3::Y));

//     auto effect = this->resourceCache->loadEffect("shaders/trace.shader");

//     std::shared_ptr<Game::ShotTrace> trace(new Game::ShotTrace(from, to));
//     trace->getLine()->setEffect(effect);

//     return trace;
// }

// std::shared_ptr<Game::Label> EntityFactory::createLabel(const std::string& fontName, unsigned int size) const {
//     Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO,
//             "Building Label with '%s' (%dpt) font", fontName.c_str(), size);

//     std::stringstream fontPath;
//     fontPath << "fonts/" << fontName << ".ttf";
//     auto font = this->resourceCache->loadFont(fontPath.str(), size);
//     auto effect = this->resourceCache->loadEffect("shaders/default.shader");

//     std::shared_ptr<Game::Label> label(new Game::Label());
//     label->setFont(font);
//     label->getSprite()->setEffect(effect);

//     return label;
// }

const std::shared_ptr<SpriteEntity> EntityFactory::createBlock(const std::string& name) const {
    auto& asset = Utils::GetResourceCache().loadAsset(name);
    auto entity = std::make_shared<SpriteEntity>();

    this->loadBase(asset, entity);
    return entity;
}

json_object* EntityFactory::getJsonObject(const std::shared_ptr<json_object>& asset, const std::string& name, json_type objectType) const {
    json_object* jsonObject = nullptr;
    if (!json_object_object_get_ex(asset.get(), name.c_str(), &jsonObject)) {
        Graphene::LogWarn("Parameter '%s' is not set", name.c_str());
        return nullptr;
    }

    json_type jsonType = json_object_get_type(jsonObject);
    if (jsonType != objectType) {
        Graphene::LogWarn("Parameter '%s' is '%s' but '%s' is expected", name.c_str(), json_type_to_name(jsonType), json_type_to_name(objectType));
        return nullptr;
    }

    return jsonObject;
}

void EntityFactory::loadBase(const std::shared_ptr<json_object>& asset, const std::shared_ptr<BaseEntity>& entity) const {
    json_object* textureObject = this->getJsonObject(asset, "texture", json_type_string);
    if (textureObject != nullptr) {
        auto texture = Graphene::GetObjectManager().createTexture(json_object_get_string(textureObject));
        auto& mesh = *entity->getMeshes().begin();
        return mesh->getMaterial()->setDiffuseTexture(texture);
    }

    json_object* visibleObject = nullptr;
    if (json_object_object_get_ex(asset.get(), "visible", &visibleObject) && json_object_get_type(visibleObject) == json_type_boolean) {
        entity->setVisible(json_object_get_boolean(visibleObject));
    }

    json_object* passiveObject = nullptr;
    if (json_object_object_get_ex(asset.get(), "passive", &passiveObject) && json_object_get_type(passiveObject) == json_type_boolean) {
        entity->setPassive(json_object_get_boolean(passiveObject));
    }

    json_object* collidableObject = nullptr;
    if (json_object_object_get_ex(asset.get(), "collidable", &collidableObject) && json_object_get_type(collidableObject) == json_type_boolean) {
        entity->setCollidable(json_object_get_boolean(collidableObject));
    }
}

}  // namespace Game

}  // namespace PolandBall
