#ifndef SPRITESHEET_LOADER_H 
#define SPRITESHEET_LOADER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <unordered_map>


std::vector<sf::Texture> load_spritesheet(std::string& path, int size);
std::vector<sf::Texture>load_spritesheet_map(std::string& path, int size);

sf::VertexArray createGrid(float gridSize, int gridLinesCount);







#endif
