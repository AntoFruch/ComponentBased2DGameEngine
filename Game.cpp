#include "Game.h"

#include <string>

#include "GameContext.h"
#include "Input/InputManager.h"
#include "TGUI/AllWidgets.hpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game(const std::string& scenePath) {
  GameContext::instance()->scene_manager()->requestLoading(scenePath);
}

void Game::run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  mWindow.setVerticalSyncEnabled(true);
  while (mWindow.isOpen()) {
    sf::Time elapsedTime = clock.restart();
    timeSinceLastUpdate += elapsedTime;
    while (timeSinceLastUpdate > TimePerFrame) {
      timeSinceLastUpdate -= TimePerFrame;

      processEvents();
      update(TimePerFrame);
    }

    render();
  }
}

void Game::processEvents() {
  GameContext::instance()->input_manager()->beginFrame();
  
  while (const std::optional event = mWindow.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      mWindow.close();
    }
    if (const auto& resized = event->getIf<sf::Event::Resized>()) {
      GameContext::instance()->render_manager()->handleResize(resized->size);
    }
    GameContext::instance()->input_manager()->processEvents(event);
  }
}

void Game::update(const sf::Time elapsedTime) {
  GameContext::instance()->scene_manager()->Update(elapsedTime);
}

void Game::render() {
  GameContext::instance()->render_manager()->renderAll();
  mWindow.display();
}
