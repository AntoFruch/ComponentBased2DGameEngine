//
// Created by Antonin Fruchet on 15/11/2026.
//

#ifndef COMPONENT_BASED_ARCH_TEXTUREMANAGER_H
#define COMPONENT_BASED_ARCH_TEXTUREMANAGER_H
#include <map>

#include "SFML/Graphics/Texture.hpp"
#include "TGUI/Font.hpp"


class ResourceManager {
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, tgui::Font> fonts;
public:
    ResourceManager() = default;

    [[nodiscard]] sf::Texture& getTexture(const std::string& path);
    [[nodiscard]] tgui::Font getFont(const std::string& path);

private:
    void loadTexture(const std::string& path);
    void loadFont(const std::string& path);
};



#endif //COMPONENT_BASED_ARCH_TEXTUREMANAGER_H
