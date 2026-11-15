//
// Created by Antonin Fruchet on 04/06/2026.
//

#include "RenderManager.h"

#include <set>

#include "Components/Camera.h"
#include "Components/Renderer.h"
#include "Managers/Collisions/CollisionsManager.h"
#include "Managers/Scene/SceneManager.h"

#include <tuple>

RenderManager::RenderManager(sf::RenderWindow* win, tgui::Gui* gui, float referenceHeight)
    : window(win), gui(gui), referenceHeight(referenceHeight)
{
    tgui::Texture::setDefaultSmooth(false);
}

void RenderManager::registerRenderer(Renderer* renderer)
{
    mRenderers.push_back(renderer);
}

void RenderManager::unregisterRenderer(const Renderer* renderer)
{
    std::erase_if(mRenderers,[&renderer](const Renderer* r){ return r==renderer;} );
}

void RenderManager::renderAll()
{
    window->clear();

    // best sorting algo for almost sorted lists;
    std::ranges::sort(mRenderers, std::less{}, [](const Renderer* r) {
        return std::tuple{r->getLayer(), r->getSortY()};
    });

    if (mainCamera){
        mainCamera->applyView(*window);
        for (auto& renderer : mRenderers)
        {
            renderer->render(*window);
        }
    }

    gui->draw();
}

void RenderManager::setMainCamera(Camera* cam)
{
    mainCamera = cam;
    handleResize(window->getSize());
}

void RenderManager::unsetMainCamera(const Camera* cam)
{
    if (mainCamera == cam) mainCamera = nullptr;
}

void RenderManager::handleResize(const sf::Vector2u& size) const
{
    if (mainCamera) {
        mainCamera->updateSize(size);
    }
}
