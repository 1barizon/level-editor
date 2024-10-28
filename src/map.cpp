#include "../include/map.h"
#include "map.h"
#include <queue>


Map::Map(int tile_size): tile_size(tile_size) {}

std::unordered_map<std::vector<int>, std::pair<int, std::string>, VectorHash> Map::Get_visible(std::vector<int> pos, int X_size, int Y_size)
{
    std::unordered_map<std::vector<int>, std::pair<int, std::string>, VectorHash> Visible_Map;
    Visible_Map.clear();
    for(const auto& layer: Tile_Map){
        for(const auto& tile: layer.second){
            if(tile.first[0] > pos[0] - tile_size && tile.first[0] < pos[0] + X_size + tile_size && tile.first[1] > pos[1] - tile_size && tile.first[1] < pos[1] + Y_size + tile_size){
                Visible_Map[tile.first] = tile.second;
            }
        }
    }
    
    return Visible_Map;
}

void Map::add_tile(std::vector<int> pos, int id, std::string texture_path, int layer)
{
    // look if the layer exists
    if(Tile_Map.find(layer) == Tile_Map.end())
    {
        Tile_Map[layer] = {};
    }
    Tile_Map[layer][pos] = {id, texture_path};
}

void Map::delete_tile(std::vector<int> pos, int layer)
{
    if(Tile_Map.find(layer) != Tile_Map.end())
    {
        if(Tile_Map[layer].find(pos) != Tile_Map[layer].end())
        {
            Tile_Map[layer].erase(pos);
        }
    }
   
}

void Map::flood_fill(std::vector<int> pos, int id, std::string texture_path, int layer)
{
    std::queue<std::vector<int>> q;
    q.push(pos);
    std::vector<std::vector<int>> directions = {{tile_size, 0}, {-tile_size, 0}, {0, tile_size}, {0, -tile_size}};
    
    while (!q.empty()) {
        std::vector<int> current = q.front();
        q.pop();
        
        // Add the current tile
        add_tile(current, id, texture_path, layer);
        
        // Check all four directions
        for (const auto& dir : directions) {
            std::vector<int> neighbor = {current[0] + dir[0], current[1] + dir[1]};
            
            // If the neighbor is not already filled
            if (Tile_Map[layer].find(neighbor) == Tile_Map[layer].end()) {
                q.push(neighbor);
            }
        }
    }
}
