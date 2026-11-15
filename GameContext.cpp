//
// Created by Antonin Fruchet on 15/11/2026.
//

#include "GameContext.h"

#include "Managers/Render/TextureManager.h"
#include "Managers/Scene/XMLSceneBuilder.h"

GameContext& GameContext::instance()
{
    static GameContext instance;
    return instance;
}

void GameContext::init(sf::RenderWindow* win, tgui::Gui* gui)
{
    instance().window = win;
    instance().gui = gui;

    // FIXME: constructor parameters here can all be moved to a config file,
    // and the config file path can be passed as a parameter to init()
    instance().sceneManager = std::make_unique<SceneManager>(
        std::make_unique<XMLSceneBuilder>()
        );

    instance().renderManager = std::make_unique<RenderManager>(
        win, gui, 480.f
        );

    instance().inputManager = std::make_unique<InputManager>(
        "resources/config/input-config.xml"
        );
    instance().textureManager = std::make_unique<TextureManager>();
}

void GameContext::cleanUp()
{
    instance().sceneManager.reset();
    instance().renderManager.reset();
    instance().inputManager.reset();
    instance().textureManager.reset();
    instance().window = nullptr;
    instance().gui = nullptr;
}
