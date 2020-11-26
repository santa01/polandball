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

#include <PolandBall.h>
#include <ObjectManager.h>
#include <OpenGL.h>
// #include <Sprite.h>
#include <EntityFactory.h>
#include <SpriteEntity.h>
#include <RenderManager.h>
#include <RenderState.h>
// #include <Vec3.h>
// #include <Vec4.h>
// #include <Mat4.h>
// #include <sstream>

namespace PolandBall {

void PolandBall::onMouseMotion(int x, int y) {
    // Math::Mat4 ndc;
    // ndc.set(0, 0, 2.0f / (this->width / 1.0f));
    // ndc.set(0, 3, -1.0f);
    // ndc.set(1, 1, -2.0f / (this->height / 1.0f));
    // ndc.set(1, 3, 1.0f);

    // Math::Mat4 world(this->scene->getCamera().getProjection());
    // world.invert();

    // Math::Vec4 cursorPosition = (world * ndc) * Math::Vec4(x, y, 0.0f, 1.0f);
    // this->cursor->setOrigin(cursorPosition.extractVec3());
    // this->player->aimAt(cursorPosition.extractVec3());
}

void PolandBall::onMouseButton(Graphene::MouseButton button, bool state) {
    if (button == Graphene::MouseButton::BUTTON_LEFT && state) {
        this->player->shoot();
    }
}

void PolandBall::onSetup() {
    Graphene::glEnable(GL_LINE_SMOOTH);
    Graphene::glLineWidth(0.5f);

    this->setupScene();
    this->setupUI();
}

void PolandBall::onIdle() {
    this->updateScene();
    this->updateUI();
}

void PolandBall::setupScene() {
    auto& scene = this->createScene();

    // In a scene traverse (render) order
    auto background = std::make_shared<Graphene::ObjectGroup>();
    auto terrain = std::make_shared<Graphene::ObjectGroup>();
    auto items = std::make_shared<Graphene::ObjectGroup>();

    auto& sceneRoot = scene->getRoot();
    sceneRoot->addObject(background);
    sceneRoot->addObject(terrain);
    sceneRoot->addObject(items);
    auto& player = scene->getPlayer();  // Attached last

    auto& objectManager = Graphene::GetObjectManager();
    auto camera = objectManager.createCamera(Graphene::ProjectionType::ORTHOGRAPHIC);
    camera->setNearPlane(-5.0f);
    camera->setFarPlane(5.0f);

    //-----------------
    auto backgroundEntity = Game::GetEntityFactory().createBlock("assets/backgrounds/sunny.asset");
    backgroundEntity->scaleX(20.0f);
    backgroundEntity->scaleY(20.0f);
    background->addObject(backgroundEntity);

    //-----------------
    auto bricksEntity = Game::GetEntityFactory().createBlock("assets/blocks/kazakhstan.asset");
    bricksEntity->scaleX(1.5f);
    bricksEntity->scaleX(20.0f);
    bricksEntity->replicateX(20.0f);
    bricksEntity->translate(7.5f, -9.0f, 0.0f);
    terrain->addObject(bricksEntity);

    bricksEntity = Game::GetEntityFactory().createBlock("assets/blocks/kazakhstan.asset");
    bricksEntity->scaleX(1.5f);
    bricksEntity->translate(6.0f, -3.0f, 0.0f);
    terrain->addObject(bricksEntity);

    //-----------------
    auto packPrimaryAmmo = Game::GetEntityFactory().createPack("assets/items/pack_primary_ammo.asset");
    packPrimaryAmmo->translate(-8.0f, 0.0f, 0.0f);
    items->addObject(packPrimaryAmmo);

    auto packSecondaryAmmo = Game::GetEntityFactory().createPack("assets/items/pack_secondary_ammo.asset");
    packSecondaryAmmo->translate(-6.0f, 0.0f, 0.0f);
    items->addObject(packSecondaryAmmo);

    auto packHealth = Game::GetEntityFactory().createPack("assets/items/pack_health.asset");
    packHealth->translate(10.0f, 0.0f, 0.0f);
    items->addObject(packHealth);

    auto packArmor = Game::GetEntityFactory().createPack("assets/items/pack_armor.asset");
    packArmor->translate(12.0f, 0.0f, 0.0f);
    items->addObject(packArmor);

    //-----------------
    auto m4a1 = Game::GetEntityFactory().createWeapon("assets/weapons/m4a1.asset");
    m4a1->translate(-4.0f, 0.0f, 0.0f);
    items->addObject(m4a1);

    auto ak74 = Game::GetEntityFactory().createWeapon("assets/weapons/ak74.asset");
    ak74->translate(-2.0f, 0.0f, 0.0f);
    items->addObject(ak74);

    auto m1911 = Game::GetEntityFactory().createWeapon("assets/weapons/m1911.asset");
    m1911->translate(2.0f, 0.0f, 0.0f);
    items->addObject(m1911);

    auto beretta92 = Game::GetEntityFactory().createWeapon("assets/weapons/beretta92.asset");
    beretta92->translate(4.0f, 0.0f, 0.0f);
    items->addObject(beretta92);

    auto wrench = Game::GetEntityFactory().createWeapon("assets/weapons/wrench.asset");
    wrench->translate(6.0f, 0.0f, 0.0f);
    items->addObject(wrench);

    auto knife = Game::GetEntityFactory().createWeapon("assets/weapons/knife.asset");
    knife->translate(8.0f, 0.0f, 0.0f);
    items->addObject(knife);

    //-----------------
    this->player = Game::GetEntityFactory().createPlayer("assets/players/turkey.asset");
    player->addObject(this->player);
    player->addObject(camera);

    //-----------------
    auto& window = this->getWindow();
    auto& overlay = window->createOverlay(0, 0, window->getWidth(), window->getHeight());
    overlay->setCamera(camera);

    //-----------------
    Graphene::RenderStateCallback callback([](Graphene::RenderState* renderState, const std::shared_ptr<Graphene::Object>& object) {
        auto baseEntity = std::dynamic_pointer_cast<Game::SpriteEntity>(object);
        if (baseEntity != nullptr) {
            auto& shader = renderState->getShader();
            shader->setUniform("uvShear", baseEntity->getShear());
            shader->setUniform("uvReplication", baseEntity->getReplication());
        }
    });

    auto& renderState = Graphene::GetRenderManager().getRenderState(Graphene::RenderOverlay::ID);
    renderState->setCallback(callback);
}

void PolandBall::setupUI() {
//     Math::Mat4 ndc;
//     ndc.set(0, 0, 2.0f / (this->width / 1.0f));
//     ndc.set(0, 3, -1.0f);
//     ndc.set(1, 1, -2.0f / (this->height / 1.0f));
//     ndc.set(1, 3, 1.0f);

//     Math::Mat4 world(this->scene->getCamera().getProjection());
//     world.invert();

//     //-----------------
//     this->emptySlot = Game::GetEntityFactory().createWidget("assets/ui/slot_empty.asset");

//     //-----------------
//     auto& primaryWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_PRIMARY];

//     primaryWeapon.first = Game::GetEntityFactory().createWidget("assets/ui/slot_empty.asset");
//     Math::Vec4 primaryWeaponOrigin = (world * ndc) * Math::Vec4(40.0f, 40.0f, 0.0f, 1.0f);
//     primaryWeapon.first->setOrigin(primaryWeaponOrigin.extractVec3());
//     this->scene->addEntity(primaryWeapon.first);

//     auto primaryWeaponBorder = Game::GetEntityFactory().createWidget("assets/ui/border_weapon.asset");
//     primaryWeaponBorder->setOrigin(primaryWeaponOrigin.extractVec3());
//     this->scene->addEntity(primaryWeaponBorder);

//     primaryWeapon.second = Game::GetEntityFactory().createLabel("dejavu-sans", 14);
//     Math::Vec4 primaryLabelOrigin = (world * ndc) * Math::Vec4(40.0f, 80.0f, 0.0f, 1.0f);
//     primaryWeapon.second->setOrigin(primaryLabelOrigin.extractVec3());
//     this->scene->addEntity(primaryWeapon.second);

//     //-----------------
//     auto& secondaryWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_SECONDARY];

//     secondaryWeapon.first = Game::GetEntityFactory().createWidget("assets/ui/slot_empty.asset");
//     Math::Vec4 secondaryWeaponOrigin = (world * ndc) * Math::Vec4(110.0f, 40.0f, 0.0f, 1.0f);
//     secondaryWeapon.first->setOrigin(secondaryWeaponOrigin.extractVec3());
//     this->scene->addEntity(secondaryWeapon.first);

//     auto secondaryWeaponBorder = Game::GetEntityFactory().createWidget("assets/ui/border_weapon.asset");
//     secondaryWeaponBorder->setOrigin(secondaryWeaponOrigin.extractVec3());
//     this->scene->addEntity(secondaryWeaponBorder);

//     secondaryWeapon.second = Game::GetEntityFactory().createLabel("dejavu-sans", 14);
//     Math::Vec4 secondaryLabelOrigin = (world * ndc) * Math::Vec4(110.0f, 80.0f, 0.0f, 1.0f);
//     secondaryWeapon.second->setOrigin(secondaryLabelOrigin.extractVec3());
//     this->scene->addEntity(secondaryWeapon.second);

//     //-----------------
//     auto& meeleWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_MEELE];

//     meeleWeapon.first = Game::GetEntityFactory().createWidget("assets/ui/slot_empty.asset");
//     Math::Vec4 meeleWeaponOrigin = (world * ndc) * Math::Vec4(180.0f, 40.0f, 0.0f, 1.0f);
//     meeleWeapon.first->setOrigin(meeleWeaponOrigin.extractVec3());
//     this->scene->addEntity(meeleWeapon.first);

//     auto meeleWeaponBorder = Game::GetEntityFactory().createWidget("assets/ui/border_weapon.asset");
//     meeleWeaponBorder->setOrigin(meeleWeaponOrigin.extractVec3());
//     this->scene->addEntity(meeleWeaponBorder);

//     meeleWeapon.second = Game::GetEntityFactory().createLabel("dejavu-sans", 14);
//     Math::Vec4 meeleLabelOrigin = (world * ndc) * Math::Vec4(180.0f, 80.0f, 0.0f, 1.0f);
//     meeleWeapon.second->setOrigin(meeleLabelOrigin.extractVec3());
//     this->scene->addEntity(meeleWeapon.second);

//     //-----------------
//     auto armorShield = Game::GetEntityFactory().createWidget("assets/ui/shield_armor.asset");
//     Math::Vec4 armorShieldOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 40.0f, 40.0f, 0.0f), 1.0f);
//     armorShield->setOrigin(armorShieldOrigin.extractVec3());
//     this->scene->addEntity(armorShield);

//     this->armor = Game::GetEntityFactory().createLabel("dejavu-sans", 14);
//     Math::Vec4 armorLabelOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 40.0f, 80.0f, 0.0f), 1.0f);
//     this->armor->setOrigin(armorLabelOrigin.extractVec3());
//     this->scene->addEntity(this->armor);

//     //-----------------
//     auto healthShield = Game::GetEntityFactory().createWidget("assets/ui/shield_health.asset");
//     Math::Vec4 healthShieldOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 100.0f, 40.0f, 0.0f), 1.0f);
//     healthShield->setOrigin(healthShieldOrigin.extractVec3());
//     this->scene->addEntity(healthShield);

//     this->health = Game::GetEntityFactory().createLabel("dejavu-sans", 14);
//     Math::Vec4 healthLabelOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 100.0f, 80.0f, 0.0f), 1.0f);
//     this->health->setOrigin(healthLabelOrigin.extractVec3());
//     this->scene->addEntity(this->health);

//     //-----------------
//     Game::Camera& camera = this->scene->getCamera();

//     for (auto& weapon: this->weapons) {
//         camera.positionChanged.connect(
//             std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//             weapon.first, std::placeholders::_1));
//         camera.positionChanged.connect(
//             std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//             weapon.second, std::placeholders::_1));
//     }

//     camera.positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         primaryWeaponBorder, std::placeholders::_1));
//     camera.positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         secondaryWeaponBorder, std::placeholders::_1));
//     camera.positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         meeleWeaponBorder, std::placeholders::_1));

//     //-----------------
//     camera.positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         healthShield, std::placeholders::_1));
//     camera.positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         armorShield, std::placeholders::_1));

//     camera.positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         this->health, std::placeholders::_1));
//     camera.positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         this->armor, std::placeholders::_1));

//     //-----------------
//     this->cursor = Game::GetEntityFactory().createWidget("assets/ui/cursor_aim.asset");
//     this->scene->addEntity(this->cursor);

//     //-----------------
//     this->player->positionChanged.connect(
//         std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
//         this->cursor, std::placeholders::_1));
}

void PolandBall::updateScene() {
    const Graphene::KeyboardState& keyboardState = this->getWindow()->getKeyboardState();

    if (keyboardState[Graphene::KeyboardKey::KEY_ESCAPE]) {
        this->exit(0);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_RIGHT]) {
        this->player->setState(Game::PlayerState::STATE_RIGHT_STEP);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_LEFT]) {
        this->player->setState(Game::PlayerState::STATE_LEFT_STEP);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_UP]) {
        this->player->setState(Game::PlayerState::STATE_JUMP);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_ENTER]) {
        this->player->setState(Game::PlayerState::STATE_DROP_WEAPON);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_1]) {
        this->player->activateSlot(Game::WeaponSlot::SLOT_PRIMARY);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_2]) {
        this->player->activateSlot(Game::WeaponSlot::SLOT_SECONDARY);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_3]) {
        this->player->activateSlot(Game::WeaponSlot::SLOT_MEELE);
    }

    const Graphene::MouseState& mouseState = this->getWindow()->getMouseState();

    if (mouseState[Graphene::MouseButton::BUTTON_LEFT]) {
        this->player->shoot();
    }
}

void PolandBall::updateUI() {
//     std::stringstream text;
//     int weaponAmmo = 0;

//     for (int slot = 0; slot < 3; slot++) {
//         auto& weapon = this->player->getWeapon(static_cast<Game::Weapon::WeaponSlot>(slot));
//         auto weaponSprite = std::dynamic_pointer_cast<Opengl::Sprite>(this->weapons[slot].first->getPrimitive());

//         if (weapon != nullptr) {
//             weaponAmmo = weapon->getAmmo();
//             text.str("");

//             if (weaponAmmo >= 999) {
//                 text << "--/--";  // Infinite
//             } else {
//                 text << weaponAmmo;
//             }

//             auto playerWeaponSprite = std::dynamic_pointer_cast<Opengl::Sprite>(weapon->getPrimitive());
//             weaponSprite->setTexture(playerWeaponSprite->getTexture());
//             weaponSprite->shearX(0.0f, 2);
//             this->weapons[slot].first->roll(45.0f);
//             this->weapons[slot].second->setText(text.str());
//         } else {
//             auto emptySprite = std::dynamic_pointer_cast<Opengl::Sprite>(this->emptySlot->getPrimitive());
//             weaponSprite->setTexture(emptySprite->getTexture());
//             this->weapons[slot].second->clear();
//         }
//     }

//     text.str("");
//     text << this->player->getHealth();
//     this->health->setText(text.str());

//     text.str("");
//     text << this->player->getArmor();
//     this->armor->setText(text.str());

//     this->scene->update(this->frameTime, this->frameStep);
//     this->scene->render();
}

}  // namespace PolandBall
