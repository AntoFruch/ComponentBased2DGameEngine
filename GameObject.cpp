//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "GameObject.h"
GameObject::GameObject(std::string_view label, const sf::Vector2f& position, const sf::Angle& rotation, const sf::Vector2f& scale, bool active)
    : label(label), active(active), transform(position, rotation, scale, this)
{

}

void GameObject::enable()
{
    for (const auto& c : components )
    {
        c->OnEnable();
    }
}

void GameObject::start()
{
    for (const auto& c : components )
    {
        c->Start();
    }
}

void GameObject::update(const sf::Time& elapsedTime) {

    if (!active) return;
    for (const auto& c : components)
    {
        c->Update(elapsedTime);
    }
}

void GameObject::addComponent(std::unique_ptr<Component> c)
{
    c->setParent(this);
    components.push_back(std::move(c));
}

std::string_view GameObject::getLabel() const
{
    return label;
}
void GameObject::setLabel(std::string_view plabel)
{
    this->label = plabel;
}

void GameObject::setActive(bool pactive)
{
    this->active = pactive;
}

bool GameObject::isActive() const
{
    return active;
}

bool GameObject::isWaitingDestruction() const
{
    return waitingDestruction;
}

void GameObject::destroySelf()
{
    waitingDestruction = true;
}

std::vector<std::unique_ptr<GameObject>> GameObject::clone() const
{
    std::vector<std::unique_ptr<GameObject>> clones;

    auto clone = std::make_unique<GameObject>(
             this->label,
             this->transform.getLocalPosition(),
             this->transform.getLocalRotation(),
             this->transform.getLocalScale(),
             this->active
         );

    for (const auto& component : this->components)
    {
        clone->addComponent(component->clone());
    }

    auto clone_ptr = clone.get();
    clones.push_back(std::move(clone));

    for (const auto& child : this->transform.getChildren())
    {
        auto childClones = child->gameObject->clone();
        for (auto& childClone : childClones)
        {
            clone_ptr->transform.addChild(&childClone->transform);
            clones.push_back(std::move(childClone));
        }
    }

    return clones;
}