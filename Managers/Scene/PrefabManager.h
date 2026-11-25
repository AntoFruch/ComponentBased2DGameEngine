//
// Created by Antonin Fruchet on 25/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_PREFABMANAGER_H
#define COMPONENT_BASED_ARCH_PREFABMANAGER_H
#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "GameObject.h"

class Parser;

class PrefabManager {
public:
    using PrefabGOList = std::vector<std::unique_ptr<GameObject>>;

private:
    Parser* parser;
    std::unordered_map<std::string, PrefabGOList> registry;

public:
    explicit PrefabManager(Parser* parser);
    [[nodiscard]] PrefabGOList getOrParse(std::string_view path);
};



#endif //COMPONENT_BASED_ARCH_PREFABMANAGER_H
