//
// Created by Antonin Fruchet on 25/05/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENEMANAGER_H
#define COMPONENT_BASED_ARCH_SCENEMANAGER_H

#include "PrefabManager.h"
#include "Scene.h"
#include "Components/Renderer.h"

class Parser;

class SceneManager {

    std::unique_ptr<Scene> scene;
    Parser* parser;
    PrefabManager prefabManager;

    std::string requestedScene = "";

    /**
     * New instantiated GameObjects requested during Update.
     */
    std::vector<std::unique_ptr<GameObject>> requestedGOs;

public:
    explicit SceneManager(Parser*);
    void Update(const sf::Time& elapsedTime);

    /**
     * @brief Register a request to load a new scene. New scene will be loaded at the end of the current Update() call.
     * @param path
     */
    void requestLoading(std::string_view path);

    GameObject* instantiate(std::string_view prefab);

private:
    void applyLoadingRequest();
};



#endif //COMPONENT_BASED_ARCH_SCENEMANAGER_H
