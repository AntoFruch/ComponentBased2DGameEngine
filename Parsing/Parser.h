//
// Created by Antonin Fruchet on 25/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_PARSER_H
#define COMPONENT_BASED_ARCH_PARSER_H
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "Scene/Scene.h"
#include "Input/InputAction.h"
#include "Animation/AnimatorGraph.h"

class PrefabManager;

class Parser {
public:
    virtual ~Parser() = default;
    virtual std::unique_ptr<Scene> parseScene(const std::string& filePath, PrefabManager& manager)=0;
    virtual std::vector<std::unique_ptr<GameObject>> parsePrefab(std::string_view filePath)=0;
    virtual std::unordered_map<std::string, std::unique_ptr<InputAction>> parseInputAction(std::string_view filePath)=0;
    virtual std::unique_ptr<AnimatorGraph> parseAnimationGraph(std::string_view path)=0;
};



#endif //COMPONENT_BASED_ARCH_PARSER_H
