#include "../include/spritesheet_loader.h"
#include <SFML/Graphics.hpp>
#include "spritesheet_loader.h"



std::vector<sf::Texture> load_spritesheet(std::string& path, int size)
{  
    std::vector<sf::Texture> textures;
    sf::Image image;
    if (!image.loadFromFile(path)) {
        std::cout << "Error loading spritesheet" << std::endl;
        return textures;
    }

    for (int y = 0; y < image.getSize().y; y += size+1) {
        for (int x = 0; x < image.getSize().x; x += size+1) {
            sf::Texture texture;
            if (texture.loadFromImage(image, sf::IntRect(x, y, size, size))) {
                textures.push_back(texture);
            }
        }
    }
    return textures;
}

std::vector<sf::Texture>load_spritesheet_map(std::string &path, int size)
{
    std::vector<sf::Texture> texture_map;
  
    sf::Image image;
    if (!image.loadFromFile(path)) {
        std::cout << "Error loading spritesheet" << std::endl;
        return texture_map;
    }
    for(int y = 0; y < image.getSize().y; y += size+1){
        for(int x = 0; x < image.getSize().x; x += size+1){
            sf::Texture texture;
            if(texture.loadFromImage(image, sf::IntRect(x, y, size, size))){
                texture_map.push_back(texture);
               
            }
        }
    }
    return texture_map;
}

sf::VertexArray createGrid(float gridSize, int gridLinesCount) {
    sf::VertexArray grid(sf::Lines, gridLinesCount * 4);  // Reserve space for all the vertices

    // Create vertical and horizontal lines
    for (int i = 0; i < gridLinesCount; ++i) {
        float position = i * gridSize;

        // Vertical lines
        grid[i * 4 + 0] = sf::Vertex(sf::Vector2f(position, 0), sf::Color::Black);
        grid[i * 4 + 1] = sf::Vertex(sf::Vector2f(position, gridLinesCount * gridSize), sf::Color::White);

        // Horizontal lines
        grid[i * 4 + 2] = sf::Vertex(sf::Vector2f(0, position), sf::Color::Black);
        grid[i * 4 + 3] = sf::Vertex(sf::Vector2f(gridLinesCount * gridSize, position), sf::Color::White);
    }

    return grid;
}

