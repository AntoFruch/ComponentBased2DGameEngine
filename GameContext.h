//
// Created by Antonin Fruchet on 15/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMECONTEXT_H
#define COMPONENT_BASED_ARCH_GAMECONTEXT_H
#include <memory>

#include "Animation/AnimatorTreeManager.h"
#include "Collisions/CollisionsManager.h"
#include "Input/InputManager.h"
#include "Render/ResourceManager.h"
#include "Render/RenderManager.h"
#include "Scene/SceneManager.h"

class GameContext {
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<RenderManager> renderManager;
    std::unique_ptr<CollisionsManager> collisionsManager;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<AnimatorTreeManager> animatorTreeManager;
    std::unique_ptr<SceneManager> sceneManager;

    std::unique_ptr<Parser> parser;

    sf::RenderWindow* window{nullptr};
    tgui::Gui* gui{nullptr};


    friend class Game;
    static GameContext* self;
    GameContext(sf::RenderWindow* win, tgui::Gui* gui);
public:
    GameContext(GameContext&) = delete;
    void operator=(const GameContext&) = delete;

    static GameContext* instance();

    [[nodiscard]] SceneManager* scene_manager() const;
    [[nodiscard]] InputManager* input_manager() const;
    [[nodiscard]] RenderManager* render_manager() const;
    [[nodiscard]] ResourceManager* resource_manager() const;
    [[nodiscard]] AnimatorTreeManager* animator_tree_manager() const;
    [[nodiscard]] CollisionsManager* collisions_manager() const;
    [[nodiscard]] sf::RenderWindow* getWindow() const;

    [[nodiscard]] tgui::Gui* getGui() const;
};



#endif //COMPONENT_BASED_ARCH_GAMECONTEXT_H
