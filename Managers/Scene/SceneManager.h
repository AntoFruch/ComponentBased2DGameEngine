//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENEMANAGER_H
#define COMPONENT_BASED_ARCH_SCENEMANAGER_H

#include "pugixml.hpp"
#include "Scene.h"
#include "SceneBuilder.h"
#include "Components/Renderer.h"

class SceneManager {

    std::unique_ptr<Scene> scene;
    std::unique_ptr<SceneBuilder> builder;

    std::string requestedScene = "";

    /**
     * New instantiated GameObjects requested during Update.
     */
    std::vector<std::unique_ptr<GameObject>> requestedGOs;

public:
    explicit SceneManager(std::unique_ptr<SceneBuilder>);
    void Update(const sf::Time& elapsedTime);

    /**
     * @brief Register a request to load a new scene. New scene will be loaded at the end of the current Update() call.
     * @param path
     */
    void requestLoading(std::string_view path);

    GameObject* instantiate(std::string_view prefab);

private:
    void applyRequest();
};



#endif //COMPONENT_BASED_ARCH_SCENEMANAGER_H
