#ifndef MAP_H 
#define MAP_H

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Map {
    public:
        Map::Map(int tile_size);
        // x , y , id , layer , texture_path
        std::vector<std::tuple<int, int, int, int,std::string>> map_data;


        void flood_fill(std::vector<std::tuple<int, int>> xy , int texture_id, int layer, std::string texture_path);


    private:
        int tile_size;
        json map;



};















#endif MAP_H

