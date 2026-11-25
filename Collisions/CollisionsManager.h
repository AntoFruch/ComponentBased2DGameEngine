//
// Created by Antonin Fruchet on 01/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
#define COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "Components/Collider.h"

class CollisionsManager {
    std::vector<Collider*> solidColliders;
    std::vector<Collider*> triggersColliders;

public:
    explicit CollisionsManager() = default;
    void registerCollider(Collider*);
    void unRegisterCollider(const Collider*);
    sf::Vector2f move(Collider& collider, const sf::Vector2f& delta);
    std::vector<Collider*> checkTrigger(Collider& collider);

    void debugDraw(sf::RenderWindow& window);
};

#endif //COMPONENT_BASED_ARCH_COLLISIONSMANAGER_H


