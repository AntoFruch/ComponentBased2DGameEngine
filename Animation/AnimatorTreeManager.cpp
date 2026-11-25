//
// Created by Antonin Fruchet on 25/11/2026.
//

#include "AnimatorTreeManager.h"

AnimatorTreeManager::AnimatorTreeManager(Parser* parser) : parser(parser)
{}

AnimatorGraph* AnimatorTreeManager::getOrParse(std::string_view path)
{
    std::string key{path};
    if (!registry.contains(key))
    {
        auto graph = parser->parseAnimationGraph(path);
        registry.try_emplace(std::string(path), std::move(graph));
    }

    return registry.at(key).get();
}
