//
// Created by Antonin Fruchet on 12/11/2026.
//

#include "XMLSceneBuilder.h"

#include "Game.h"

std::unique_ptr<Scene> XMLSceneBuilder::buildScene(std::string_view filePath)
{
    std::filesystem::path path = filePath;
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result)
    {
        throw IllegalOperationException(std::format("Could not open scene because {}", result.description()));
    }
    pugi::xml_node sceneObj = doc.child("Scene");
    if (!sceneObj)
    {
        throw IllegalOperationException("Root node of scene must be named \"Scene\"");
    }

    std::vector<std::unique_ptr<GameObject>> gameObjects;

    for (const auto& obj : sceneObj)
    {
        std::string name = obj.name();
        if (name == "GameObject")
        {
            for (auto& p : buildGameObject(obj))
            {
                gameObjects.push_back(std::move(p));
            }
        } else if (name == "Prefab")
        {
            for (auto& p : buildPrefab(obj.attribute("src").as_string()))
            {
                gameObjects.push_back(std::move(p));
            }
        }
    }

    return std::make_unique<Scene>(gameObjects);
}

std::vector<std::unique_ptr<GameObject>> XMLSceneBuilder::buildPrefab(std::string_view filePath)
{
    if (prefabRegistry.contains(filePath.data()))
    {
        std::cout << "Prefab already loaded: " << filePath << "\n cloning it from registry..."<<std::endl;
        return prefabRegistry.at(filePath.data()).front()->clone();
    }

    std::cout << "Prefab not loaded before: " << filePath << "\n parsing file..."<<std::endl;
    std::filesystem::path path = filePath;
    pugi::xml_document doc;
    if (auto result = doc.load_file(path.c_str()); !result)
    {
        throw IllegalOperationException(std::format("Could not prefab scene because {}", result.description()));
    }
    auto obj = doc.child("GameObject");
    prefabRegistry.emplace(filePath, buildGameObject(obj));
    auto objects = prefabRegistry.at(filePath.data()).front()->clone();

    if (obj.attribute("label"))
    {
        objects[0]->setLabel(obj.attribute("label").as_string());
    }
    if (obj.attribute("x") || obj.attribute("y"))
    {
        objects[0]->transform.move({obj.attribute("x").as_float(), obj.attribute("y").as_float()});
    }
    if (obj.attribute("angle"))
    {
        objects[0]->transform.rotate(sf::degrees(obj.attribute("angle").as_float()));
    }
    if (obj.attribute("sx") || obj.attribute("sy"))
    {
        objects[0]->transform.rescale({obj.attribute("sx").as_float(1.f), obj.attribute("sy").as_float(1.f)});
    }
    if (obj.attribute("active"))
    {
        objects[0]->setActive(obj.attribute("active").as_bool());
    }

    return objects;
}

std::vector<std::unique_ptr<GameObject>> XMLSceneBuilder::buildGameObject(const pugi::xml_node& rootNode)
{
    std::vector<std::unique_ptr<GameObject>> objects;

    // Structure pour stocker l'élément XML et son parent dans la pile
    struct StackElement {
        pugi::xml_node node;
        GameObject* parent; // nullptr si c'est un objet racine
    };

    std::stack<StackElement> stack;
    stack.push({rootNode, nullptr});

    while (!stack.empty())
    {
        auto [currentNode, parentPtr] = stack.top();
        stack.pop();

        auto ptr = std::make_unique<GameObject>(
            currentNode.attribute("label").as_string(),
            sf::Vector2f{
                currentNode.attribute("x").as_float(),
                currentNode.attribute("y").as_float()
            },
            sf::degrees(currentNode.attribute("angle").as_float()),
            sf::Vector2f{
                currentNode.attribute("sx") ? currentNode.attribute("sx").as_float() : 1.f,
                currentNode.attribute("sy") ? currentNode.attribute("sy").as_float() : 1.f,
            },
            currentNode.attribute("active") ? currentNode.attribute("active").as_bool() : true
        );

        auto components = currentNode.child("Components");
        for (const auto& component : components.children())
        {
            std::string componentNodeName = component.name();
            if (componentNodeName != "Component")
            {
                throw IllegalOperationException("Components node children must be named \"Component\"");
            }
            const std::string componentName = component.attribute("name").as_string();
            if (componentName.empty())
            {
                throw IllegalOperationException("Component node must have a non-empty \"name\" attribute");
            }
            ptr->addComponent(ComponentFactory::Create(componentName, component));
        }

        if (parentPtr != nullptr)
        {
            parentPtr->transform.addChild(&ptr->transform);
        }

        std::vector<pugi::xml_node> childList;
        for (auto child : currentNode.children("GameObject"))
        {
            stack.push({child, ptr.get()});
        }

        objects.push_back(std::move(ptr));
    }

    return objects;
}
