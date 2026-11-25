//
// Created by Antonin Fruchet on 25/11/2026.
//

#include "PrefabManager.h"

#include <format>
#include "exceptions/IllegalOperationException.h"
#include "Parsing/Parser.h"

PrefabManager::PrefabManager(Parser* parser) : parser(parser)
{}

PrefabManager::PrefabGOList PrefabManager::getOrParse(std::string_view path)
{
    std::string key{path};
    if (!registry.contains(key))
    {
        PrefabGOList prefab = parser->parsePrefab(path);
        registry.try_emplace(std::string(path), std::move(prefab));
    }

    if (registry.at(key).empty() || !registry.at(key).front())
        throw IllegalOperationException(std::format("Prefab '{}' contains no valid GameObject.", path));

    return registry.at(key).front()->clone();
}