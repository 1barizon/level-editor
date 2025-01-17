#ifndef MAP_H 
#define MAP_H

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <tuple>
#include <variant>
#include <functional>
using json = nlohmann::json;


struct VectorHash {
std::size_t operator()(const std::vector<int>& v) const {
    std::size_t seed = v.size();
    for (auto& i : v) {
        seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}
};

class Map {
    public:
        Map(int tile_size);

        struct AutoTileConfig {
            std::string texture_path;
            std::unordered_map<int, std::vector<std::vector<int>>> border_list;  // "id" : [combinações]   
        };

        std::vector<std::vector<int>> AutoTileCheckList = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        // x , y , id , layer , texture_path
        std::unordered_map<int, std::unordered_map<std::vector<int>, std::pair<int, std::string>, VectorHash>> Tile_Map;
        std::unordered_map<std::vector<int>, std::pair<int, std::string>, VectorHash> Get_visible(std::vector<int> pos, int X_size , int Y_size);
        std::vector<AutoTileConfig> TilesConfigs;
        // functions
        void add_tile(std::vector<int> pos, int id, std::string texture_path, int layer); 
        void delete_tile(std::vector<int> pos, int layer);
        void flood_fill(std::vector<int> pos, int id, std::string texture_path, int layer);
        void auto_tile(std::vector<int> initial_pos, std::vector<int> final_pos, int layer);
    private:
        
        int tile_size;
        json map;



};















#endif MAP_H

