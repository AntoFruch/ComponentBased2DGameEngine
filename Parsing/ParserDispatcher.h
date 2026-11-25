//
// Created by Antonin Fruchet on 25/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_PARSERDISPATCHER_H
#define COMPONENT_BASED_ARCH_PARSERDISPATCHER_H
#include "Parser.h"
#include "XMLParser.h"


class ParserDispatcher : public Parser {
    XMLParser xml;
    // add other parsers

public:
    std::unique_ptr<Scene> parseScene(const std::string& filePath, PrefabManager& manager) override;
    std::vector<std::unique_ptr<GameObject>> parsePrefab(std::string_view filePath) override;
    std::unordered_map<std::string, std::unique_ptr<InputAction>> parseInputAction(std::string_view filePath) override;
    std::unique_ptr<AnimatorGraph> parseAnimationGraph(std::string_view path) override;
};



#endif //COMPONENT_BASED_ARCH_PARSERDISPATCHER_H
