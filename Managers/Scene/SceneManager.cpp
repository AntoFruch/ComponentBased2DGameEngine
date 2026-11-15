//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "SceneManager.h"

#include <iostream>

SceneManager::SceneManager(std::unique_ptr<SceneBuilder> builder)
    : builder(std::move(builder))
{
}

void SceneManager::Update(const sf::Time& elapsedTime)
{
    if (scene) scene->Update(elapsedTime);
    if (!requestedScene.empty()) applyLoadingRequest();
    if (!requestedGOs.empty()) scene->instantiate(requestedGOs);
}

void SceneManager::requestLoading(std::string_view path)
{
    if (!requestedScene.empty()) return;
    requestedScene = path;
}

void SceneManager::applyLoadingRequest()
{
    scene = std::move(builder->buildScene(requestedScene));
    std::string dump{scene->dump()};
    std::cout << dump << std::endl;
    scene->Start();
    requestedScene = "";
}

GameObject* SceneManager::instantiate(std::string_view prefab)
{
    if (std::filesystem::path path{prefab}; path.empty())
    {
        throw IllegalOperationException("Prefab path must be non-empty");
    }

    for (auto& go : builder->buildPrefab(prefab))
    {
        requestedGOs.push_back(std::move(go));
    }

    return requestedGOs.front().get();
}