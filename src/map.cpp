#include "../include/map.h"
#include "map.h"

Map::Map(int tile_size)
{
    this->tile_size = tile_size;
}
void Map::flood_fill(std::vector<std::tuple<int, int>> xy, int texture_id, int layer, std::string texture_path)
{
    std::vector<std::tuple< int , int>> remaining_tiles = xy;
    while(remaining_tiles.size() > 0){
        std::vector<std::tuple<int, int>> new_remaining_tiles;
        for(auto& tile : remaining_tiles){
            int x, y;
            std::tie(x, y) = tile;
            map_data.push_back(std::make_tuple(x, y, texture_id, layer, texture_path));
            // check if the tile above is empty
            if(std::find(map_data.begin(), map_data.end(), std::make_tuple(x, y - tile_size, texture_id, layer, texture_path)) == map_data.end()){
                new_remaining_tiles.push_back(std::make_tuple(x, y - tile_size));
            }
            // check if the tile below is empty
            if(std::find(map_data.begin(), map_data.end(), std::make_tuple(x, y + tile_size, texture_id, layer, texture_path)) == map_data.end()){
                new_remaining_tiles.push_back(std::make_tuple(x, y + tile_size));
            }
            // check if the tile to the left is empty
            if(std::find(map_data.begin(), map_data.end(), std::make_tuple(x - tile_size, y, texture_id, layer, texture_path)) == map_data.end()){
                new_remaining_tiles.push_back(std::make_tuple(x - tile_size, y));
            }
            // check if the tile to the right is empty
            if(std::find(map_data.begin(), map_data.end(), std::make_tuple(x + tile_size, y, texture_id, layer, texture_path)) == map_data.end()){
                new_remaining_tiles.push_back(std::make_tuple(x + tile_size, y));
            }
        }
        remaining_tiles = new_remaining_tiles;
    }
}

