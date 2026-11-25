//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "Renderer.h"

#include <iostream>

#include "GameObject.h"
#include "Collider.h"
#include "GameContext.h"
#include "pugixml.hpp"
#include "Managers/Render/RenderManager.h"
#include "Managers/Render/ResourceManager.h"

Renderer::Renderer(
    const std::string& texture_path,
    const sf::Vector2f& anchor,
    const sf::Vector2u& spriteSize,
    int layer,
    float sortOffsetY)
    : layer(layer), sortOffsetY(sortOffsetY), spriteSize(spriteSize), texture(GameContext::instance()->resource_manager()->getTexture(texture_path))
{
    const auto size = static_cast<sf::Vector2f>(spriteSize);
    this->mShape.setSize(size);
    this->mShape.setOrigin(anchor + size / 2.f);

    this->mShape.setTexture(&texture);
}

Renderer::~Renderer()
{
    if (auto* rm = GameContext::instance()->render_manager())
    {
        rm->unregisterRenderer(this);
    }
}

void Renderer::Start()
{
    GameContext::instance()->render_manager()->registerRenderer(this);
    cutRect.size = static_cast<sf::Vector2i>(spriteSize);
    setCutRectPos(0, 0);
    mShape.setTextureRect(cutRect);
}

void Renderer::Update(const sf::Time& elapsedTime)
{
    mShape.setPosition(gameObject->transform.getWorldPosition());
    mShape.setRotation(gameObject->transform.getWorldRotation());
    mShape.setScale(gameObject->transform.getWorldScale());
}

void Renderer::render(sf::RenderWindow& window) const
{
    window.draw(mShape);
}

void Renderer::setCutRectPos(unsigned int x, unsigned int y)
{
    cutRect.position = sf::Vector2i{static_cast<int>(x*spriteSize.x), static_cast<int>(y*spriteSize.y)};
    mShape.setTextureRect(cutRect);
}

const sf::Vector2u& Renderer::getSpriteSize() const
{
    return spriteSize;
}

int Renderer::getLayer() const
{
    return layer;
}

float Renderer::getSortY() const
{
    return gameObject->transform.getWorldPosition().y + sortOffsetY;
}
