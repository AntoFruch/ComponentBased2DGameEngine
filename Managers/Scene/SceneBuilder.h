//
// Created by Antonin Fruchet on 12/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_SCENEBUILDER_H
#define COMPONENT_BASED_ARCH_SCENEBUILDER_H
#include <memory>

#include "Scene.h"


class SceneBuilder {
public:
    virtual ~SceneBuilder() = default;
    virtual std::unique_ptr<Scene> buildScene(std::string_view filePath) = 0;
    virtual std::vector<std::unique_ptr<GameObject>> buildPrefab(std::string_view filePath) = 0;
};



#endif //COMPONENT_BASED_ARCH_SCENEBUILDER_H
