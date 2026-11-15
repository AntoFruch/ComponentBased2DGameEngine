//
// Created by Antonin Fruchet on 15/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMECONTEXT_H
#define COMPONENT_BASED_ARCH_GAMECONTEXT_H
#include <memory>

#include "Managers/Input/InputManager.h"
#include "Managers/Render/RenderManager.h"
#include "Managers/Scene/SceneManager.h"


class GameContext {
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<RenderManager> renderManager;
    std::unique_ptr<SceneManager> sceneManager;

    sf::RenderWindow* window{nullptr};
    tgui::Gui* gui{nullptr};

    GameContext() = default;
public:
    GameContext(GameContext&) = delete;
    void operator=(const GameContext&) = delete;

    static GameContext& instance();

    [[nodiscard]] SceneManager* scene_manager() const
    {
        return sceneManager.get();
    }

    [[nodiscard]] InputManager* input_manager() const
    {
        return inputManager.get();
    }

    [[nodiscard]] RenderManager* render_manager() const
    {
        return renderManager.get();
    }

    [[nodiscard]] sf::RenderWindow* getWindow() const
    {
        return window;
    }

    [[nodiscard]] tgui::Gui* getGui() const
    {
        return gui;
    }

private:
    friend class Game;
    static void init(sf::RenderWindow* win, tgui::Gui* gui);
    static void cleanUp();
};



#endif //COMPONENT_BASED_ARCH_GAMECONTEXT_H
