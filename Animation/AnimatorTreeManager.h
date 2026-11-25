//
// Created by Antonin Fruchet on 25/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_ANIMATORTREEMANAGER_H
#define COMPONENT_BASED_ARCH_ANIMATORTREEMANAGER_H
#include <unordered_map>

#include "AnimatorGraph.h"
#include "Parsing/Parser.h"


class AnimatorTreeManager {
    std::unordered_map<std::string , std::unique_ptr<AnimatorGraph>> registry;

    Parser* parser;
public:
    explicit AnimatorTreeManager(Parser* parser);
    AnimatorGraph* getOrParse(std::string_view path);
};



#endif //COMPONENT_BASED_ARCH_ANIMATORTREEMANAGER_H
