//
// Created by Antonin Fruchet on 04/06/2026.
//

#include "Camera.h"

#include "GameContext.h"
#include "GameObject.h"

#include "Managers/Render/RenderManager.h"

Camera::~Camera()
{
    if (auto* rm = GameContext::instance()->render_manager())
    {
        rm->unsetMainCamera(this);
    }
}

void Camera::Start()
{
    Component::Start();
    GameContext::instance()->render_manager()->setMainCamera(this);
}

void Camera::Update(const sf::Time& elapsedTime)
{
    Component::Update(elapsedTime);
    m_view.setCenter(gameObject->transform.getWorldPosition());
}

void Camera::updateSize(sf::Vector2u size)
{
    float windowRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
    auto newWidth = windowRatio * GameContext::instance()->render_manager()->referenceHeight;
    m_view.setSize(sf::Vector2f{newWidth, GameContext::instance()->render_manager()->referenceHeight});
}

void Camera::applyView(sf::RenderWindow& window) const
{
    window.setView(m_view);
}
