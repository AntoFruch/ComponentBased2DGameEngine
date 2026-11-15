//
// Created by Antonin Fruchet on 04/06/2026.
//

#ifndef COMPONENT_BASED_ARCH_RENDERMANAGER_H
#define COMPONENT_BASED_ARCH_RENDERMANAGER_H
#include <vector>

#include "SFML/Graphics.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

class Camera;
class Renderer;


class RenderManager {
    std::vector<Renderer*> mRenderers;
    Camera* mainCamera{nullptr};
    sf::RenderWindow* window;
    tgui::Gui* gui;

public:
    const float referenceHeight{480.f};

    RenderManager(sf::RenderWindow* win, tgui::Gui* gui, float referenceHeight);

    void registerRenderer(Renderer*);
    void unregisterRenderer(const Renderer*);
    void renderAll();

    void setMainCamera(Camera*);
    void unsetMainCamera(const Camera*);

    void handleResize(const sf::Vector2u& size) const;
};



#endif //COMPONENT_BASED_ARCH_RENDERMANAGER_H
