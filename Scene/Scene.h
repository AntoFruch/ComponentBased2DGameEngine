//
// Created by Antonin Fruchet on 05/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENE_H
#define COMPONENT_BASED_ARCH_SCENE_H

#include <pugixml.hpp>

#include "GameObject.h"
#include "TGUI/Backend/SFML-Graphics.hpp"

class Scene {
    std::vector<std::unique_ptr<GameObject>> mObjects;

public:
    explicit Scene(std::vector<std::unique_ptr<GameObject>>&);
    void Start();
    void Update(const sf::Time& elapsedTime);

    void instantiate(std::vector<std::unique_ptr<GameObject>>& list);

    std::string dump() const;

};



#endif //COMPONENT_BASED_ARCH_SCENE_H
