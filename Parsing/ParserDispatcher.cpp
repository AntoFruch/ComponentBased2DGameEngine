//
// Created by Antonin Fruchet on 25/11/2026.
//

#include "ParserDispatcher.h"

std::string resolveFileType(std::string_view path)
{
    const auto dotPos = path.find_last_of('.');
    if (dotPos == std::string_view::npos || dotPos + 1 >= path.size())
    {
        return {};
    }

    const auto slashPos = path.find_last_of("/\\");
    if (slashPos != std::string_view::npos && dotPos < slashPos)
    {
        return {};
    }

    return std::string(path.substr(dotPos + 1));
}
std::unique_ptr<Scene> ParserDispatcher::parseScene(const std::string& filePath, PrefabManager& manager)
{
    auto extension = resolveFileType(filePath);
    if (extension == "xml")
        return xml.parseScene(filePath, manager);
    /*
    if (extension == "json")
        return json.parseScene(filePath, manager);
    ...
    ...
    */
    return nullptr;
}

std::vector<std::unique_ptr<GameObject>> ParserDispatcher::parsePrefab(std::string_view filePath)
{
    auto extension = resolveFileType(filePath);
    if (extension == "xml")
        return xml.parsePrefab(filePath);
    /*
    if (extension == "json")
        return json.parseScene(filePath, manager);
    ...
    ...
    */
    return std::vector<std::unique_ptr<GameObject>>();
}

std::unordered_map<std::string, std::unique_ptr<InputAction>> ParserDispatcher::parseInputAction(
    std::string_view filePath)
{
    auto extension = resolveFileType(filePath);
    if (extension == "xml")
        return xml.parseInputAction(filePath);
    /*
    if (extension == "json")
        return json.parseScene(filePath, manager);
    ...
    ...
    */
    return std::unordered_map<std::string, std::unique_ptr<InputAction>>{};
}

std::unique_ptr<AnimatorGraph> ParserDispatcher::parseAnimationGraph(std::string_view filePath)
{
    auto extension = resolveFileType(filePath);
    if (extension == "xml")
        return xml.parseAnimationGraph(filePath);
    /*
    if (extension == "json")
        return json.parseScene(filePath, manager);
    ...
    ...
    */
    return nullptr;
}
