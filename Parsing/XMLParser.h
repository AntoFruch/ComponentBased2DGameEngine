//
// Created by Antonin Fruchet on 25/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_XMLPARSER_H
#define COMPONENT_BASED_ARCH_XMLPARSER_H
#include "Parser.h"


class XMLParser : public Parser{
public:
    std::unique_ptr<Scene> parseScene(const std::string& filePath, PrefabManager& manager) override;
    std::vector<std::unique_ptr<GameObject>> parsePrefab(std::string_view filePath) override;
    std::unordered_map<std::string, std::unique_ptr<InputAction>> parseInputAction(std::string_view filePath) override;
};



#endif //COMPONENT_BASED_ARCH_XMLPARSER_H
