//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_GAME_H
#define COMPONENT_BASED_ARCH_GAME_H
#include "GameContext.h"
#include "Scene/Scene.h"
#include "TGUI/Backend/SFML-Graphics.hpp"


class Game {
public:
    explicit  Game(const std::string& scenePath);
    ~Game() = default;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    void run();

    static constexpr int W_WIDTH{1560};
    static constexpr int W_HEIGHT{900};

private:

    void processEvents();
    void update(sf::Time elapsedTime);
    void render();

    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow{sf::VideoMode({W_WIDTH, W_HEIGHT}), "SFML Application"};
    tgui::Gui gui{mWindow};
    GameContext mCtx{&mWindow, &gui}; //Raii Pattern
};



#endif //COMPONENT_BASED_ARCH_GAME_H
