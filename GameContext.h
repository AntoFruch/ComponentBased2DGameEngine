//
// Created by Antonin Fruchet on 15/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAMECONTEXT_H
#define COMPONENT_BASED_ARCH_GAMECONTEXT_H
#include <memory>

#include "Managers/Animation/AnimatorTreeManager.h"
#include "Managers/Input/InputManager.h"
#include "Managers/Render/RenderManager.h"
#include "Managers/Scene/SceneManager.h"
#include "Managers/Render/TextureManager.h"
#include "Parsing/XMLParser.h"

class GameContext {
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<RenderManager> renderManager;
    std::unique_ptr<SceneManager> sceneManager;
    std::unique_ptr<TextureManager> textureManager;
    std::unique_ptr<AnimatorTreeManager> animatorTreeManager;

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
    [[nodiscard]] TextureManager* texture_manager() const;
    [[nodiscard]] AnimatorTreeManager* animator_tree_manager() const;
    [[nodiscard]] sf::RenderWindow* getWindow() const;

    [[nodiscard]] tgui::Gui* getGui() const;
};



#endif //COMPONENT_BASED_ARCH_GAMECONTEXT_H
