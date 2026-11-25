//
// Created by Antonin Fruchet on 01/06/2026.
//

#include "CharacterController.h"
#include "GameContext.h"
#include "exceptions/IllegalOperationException.h"

void CharacterController::Start()
{
    Component::Start();
    collider = gameObject->getComponent<Collider>();

    if (!collider)
    {
        throw IllegalOperationException("CharacterController requires a Collider on the same GameObject");
    }
}

void CharacterController::move(const sf::Vector2f& delta)
{
    auto newDelta = GameContext::instance()->collisions_manager()->move(*collider, delta);
    gameObject->transform.move(newDelta);
}
