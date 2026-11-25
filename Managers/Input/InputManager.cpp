//
// Created by Antonin Fruchet on 26/05/2026.
//

#include "InputManager.h"

#include "GameContext.h"
#include "Managers/Scene/SceneManager.h"

InputManager::InputManager(ActionMap actions)
    : actions(std::move(actions))
{}

InputAction* InputManager::findAction(const std::string& action_name)
{
    if (!actions.contains(action_name))
    {
        throw IllegalOperationException(std::format("{} is not a known action", action_name));
    }
    return actions.at(action_name).get();
}

void InputManager::processEvents(std::optional<sf::Event> event)
{
    GameContext::instance()->getGui()->handleEvent(*event);
    // We check the currently pressed keys and we put/remove them from pressed_key
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        pressed_keys.insert(keyPressed->code);
    } else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
        pressed_keys.erase(keyReleased->code);
    } else {
        return;
    }

    for (const auto& [name, action] : actions) {
        action->processEvent(event, pressed_keys);
    }
}

void InputManager::beginFrame()
{
    for (const auto& [name, action] : actions)
    {
        action->resetFrameState();
    }
}