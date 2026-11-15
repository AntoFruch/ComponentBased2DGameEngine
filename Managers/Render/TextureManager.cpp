//
// Created by Antonin Fruchet on 15/11/2026.
//

#include "TextureManager.h"

#include <iostream>

sf::Texture& TextureManager::getTexture(const std::string& path)
{
    if (!textureRegistry.contains(path))
    {
        loadTexture(path);
    }
    std::cout << "Texture at " << path << " retrieved" << std::endl;
    return textureRegistry.at(path);
}

void TextureManager::loadTexture(const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Texture at " << path << "not found" <<std::endl;
    }
    std::cout << "Texture at " << path << " loaded" << std::endl;
    textureRegistry.try_emplace(path, std::move(texture));
}
