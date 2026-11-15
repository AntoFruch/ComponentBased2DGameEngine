//
// Created by Antonin Fruchet on 12/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_XMLSCENEBUILDER_H
#define COMPONENT_BASED_ARCH_XMLSCENEBUILDER_H
#include "SceneBuilder.h"


class XMLSceneBuilder : public SceneBuilder{

public:
    ~XMLSceneBuilder() override = default;
    std::unique_ptr<Scene> buildScene(std::string_view filePath) override;
    std::vector<std::unique_ptr<GameObject>> buildPrefab(std::string_view filePath) override;

private:
    std::vector<std::unique_ptr<GameObject>> buildGameObject(const pugi::xml_node&);
};



#endif //COMPONENT_BASED_ARCH_XMLSCENEBUILDER_H
