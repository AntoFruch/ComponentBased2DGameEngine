//
// Created by Antonin Fruchet on 15/11/2026.
//

#include "GameContext.h"

#include "Managers/Render/TextureManager.h"
#include "Parsing/ParserDispatcher.h"

GameContext* GameContext::self = nullptr;

GameContext::GameContext(sf::RenderWindow* win, tgui::Gui* gui)
{
    self = this;
    self->window = win;
    self->gui = gui;

    self->parser = std::make_unique<ParserDispatcher>();

    // FIXME: constructor parameters here can all be moved to a config file,
    // and the config file path can be passed as a parameter
    self->sceneManager = std::make_unique<SceneManager>(
        parser.get());

    self->renderManager = std::make_unique<RenderManager>(
        win, gui, 480.f
        );

    self->inputManager = std::make_unique<InputManager>(
        "resources/config/input-config.xml"
        );
    self->textureManager = std::make_unique<TextureManager>();
}

GameContext* GameContext::instance()
{
    return self;
}

SceneManager* GameContext::scene_manager() const
{
    return sceneManager.get();
}

InputManager* GameContext::input_manager() const
{
    return inputManager.get();
}

RenderManager* GameContext::render_manager() const
{
    return renderManager.get();
}

TextureManager* GameContext::texture_manager() const
{
    return textureManager.get();
}

sf::RenderWindow* GameContext::getWindow() const
{
    return window;
}
