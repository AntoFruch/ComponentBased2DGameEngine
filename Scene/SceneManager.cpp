//
// Created by Antonin Fruchet on 25/05/2026.
//

#include "SceneManager.h"

#include <iostream>
#include "Parsing/Parser.h"

SceneManager::SceneManager(Parser* parser)
    : parser(parser), prefabManager(parser)
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
    scene = std::move(parser->parseScene(requestedScene, prefabManager));
    std::string dump{scene->dump()};
    std::cout << dump << std::endl;
    scene->Start();
    requestedScene = "";
}

GameObject* SceneManager::instantiate(std::string_view prefab)
{
    if (prefab.empty())
    {
        throw IllegalOperationException("Prefab path must be non-empty");
    }

    for (auto& go : prefabManager.getOrParse(prefab))
    {
        requestedGOs.push_back(std::move(go));
    }

    return requestedGOs.front().get(); // first GO is always the root
}