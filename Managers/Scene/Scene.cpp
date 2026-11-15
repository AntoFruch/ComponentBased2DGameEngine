//
// Created by Antonin Fruchet on 05/06/2026.
//

#include "Scene.h"
#include "SceneManager.h"
#include <format>
#include <iostream>

#include "exceptions/IllegalOperationException.h"

Scene::Scene(std::vector<std::unique_ptr<GameObject>>& list)
{
    for (auto& p : list)
    {
        mObjects.push_back(std::move(p));
    }
}

void Scene::Start()
{
    for (const auto& go : mObjects)
    {
        go->enable();
    }

    for (const auto& go : mObjects)
    {
        go->start();
    }
}

void Scene::Update(const sf::Time& elapsedTime)
{
    for (auto const& go : mObjects)
    {
        go->update(elapsedTime);
    }

    std::erase_if(mObjects, [](const std::unique_ptr<GameObject>& go){
            return go->isWaitingDestruction();
        });
}

void Scene::instantiate(std::vector<std::unique_ptr<GameObject>>& list)
{
    for (auto& go : list)
    {
        mObjects.push_back(std::move(go));
        go->start();
    }
    list.clear();
}

std::string Scene::dump() const
{
    std::ostringstream oss;
    oss << "Scene :\n";

    auto dumpGameObject = [&oss](const auto& self, const GameObject* go, const std::string& indent) -> void {
        if (!go) return;

        oss << indent << "- " << go->getLabel() << "\n";

        for (const Transform* child : go->transform.getChildren())
        {
            self(self, child->gameObject, indent + "  ");
        }
    };

    for (const auto& go : mObjects)
    {
        if (go->transform.getParent() == nullptr)
        {
            dumpGameObject(dumpGameObject, go.get(), "  ");
        }
    }
    return oss.str();
}

