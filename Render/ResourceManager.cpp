//
// Created by Antonin Fruchet on 15/11/2026.
//

#include "ResourceManager.h"

#include <iostream>

sf::Texture& ResourceManager::getTexture(const std::string& path)
{
    if (!textures.contains(path))
    {
        loadTexture(path);
    }
    std::cout << "Texture at " << path << " retrieved" << std::endl;
    return textures.at(path);
}

tgui::Font ResourceManager::getFont(const std::string& path)
{
    if (!fonts.contains(path))
    {
        loadFont(path);
    }
    std::cout << "Texture at " << path << " retrieved" << std::endl;
    return fonts.at(path);
}

void ResourceManager::loadTexture(const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Texture at " << path << "not found" <<std::endl;
    }
    std::cout << "Texture at " << path << " loaded" << std::endl;
    textures.try_emplace(path, std::move(texture));
}

void ResourceManager::loadFont(const std::string& path)
{
    tgui::Font font{path};
    if (font == nullptr)
    {
        std::cerr << "Font at " << path << "not found" <<std::endl;
        return;
    }
    std::cout << "Font at " << path << " loaded" << std::endl;
    fonts.try_emplace(path, std::move(font));
}
