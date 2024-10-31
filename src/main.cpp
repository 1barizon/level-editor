#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <tuple>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../include/file_dialog.h"
#include "../include/spritesheet_loader.h" 
#include "../include/map.h"



using json = nlohmann::json;



int main() {
    // Criação da janela SFML
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Level Editor");
    window.setFramerateLimit(60);

    // mouse pos
    


    // Inicializar ImGui-SFML
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    // variaves


    std::filesystem::path path = std::filesystem::current_path();
    // map variables
    FileDialog dialog_open_map("Open File", path.string());
    bool dialog_open_map_var  = false;
    std::string map_path = "";
    std::string map_type = ".json";
    std::vector<int> scroll = {0,0};
    int scroll_speed = 5;

    // sprite variables
    bool dialog_open_sprite_var = false;
    FileDialog dialog_open_sprite("Open File", path.string());
    std::string sprite_path = "";
    std::string sprite_type = ".png";
    std::vector<sf::Texture> textures;
    std::vector<std::vector<sf::Texture>> vector_textures;
    std::vector<std::string> list_textures_paths;
    std::vector<int> sprites_sizes;
    int currentSprite = 0;
    bool sprite_size_var = false;
    int sprite_size = 20;
    int sprite_index = 0;
    bool textures_loaded = false;
    sf::Texture selected_texture;
    int selected_texture_id;
    std::string selected_texture_path = "";
    int layer = 0;

    // refactoring for perfomance  
    std::unordered_map<std::string, std::vector<sf::Texture>> spriteSheets;  // spriteSheets < 'nome do arquivo' , sprite >
    std::string selected_spriteSheet;
    std::vector<std::vector<int>> selection_pos = { {0, 0}, {0, 0} };




    // view
    sf::View view = window.getDefaultView();
    
    bool grid_settings = false;
    bool show_grid  = true;
    bool auto_tile_settings = false;
    bool editing = true;
    bool selection_mode = false;
    int tile_size = 20;
    int grid_size = 100; // pixels 
    float scale = 1.5f;
    view.zoom(1.0f/scale);
    sf::VertexArray grid = createGrid(sprite_size,100);
    

    // map
   
    Map Level_Map(tile_size);
    std::vector<int> render_pos = {0, 0};
    std::unordered_map<std::vector<int>, std::pair<int, std::string>, VectorHash> Visible_Map;
    int pos_x;
    int pos_y;
 
    // Loop principal da janela
    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, view);
        pos_x = int((worldPos.x - tile_size) / tile_size) * tile_size + tile_size; 
        pos_y = int((worldPos.y - tile_size) / tile_size) * tile_size + tile_size;
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Passar eventos para o ImGui
            ImGui::SFML::ProcessEvent(event);
        }

        float deltaTime = deltaClock.restart().asSeconds();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            view.move(-scroll_speed, 0);
            scroll[0] -= scroll_speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            view.move(scroll_speed, 0);
            scroll[0] += scroll_speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            view.move(0, -scroll_speed);
            scroll[1] -= scroll_speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            view.move(0, scroll_speed);
            scroll[1] += scroll_speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
           Level_Map.auto_tile(selection_pos[0], selection_pos[1], layer);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
            Level_Map.flood_fill({pos_x, pos_y}, selected_texture_id, selected_texture_path, layer);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.x > 320 && mousePos.y > 44 && editing) {
        
            if(!selected_texture.getSize().x == 0){
                std::vector<int> pos = {pos_x, pos_y};
                Level_Map.add_tile(pos, selected_texture_id, selected_texture_path, layer);
            }
         
            
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mousePos.x > 320 && mousePos.y > 44 && editing) {
            std::vector<int> pos = {pos_x, pos_y};
            Level_Map.delete_tile(pos, layer);
        }
    

        // crtl + l and k to change layer
        static bool ctrlPressed = false;
        static bool lPressed = false;
        static bool kPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            ctrlPressed = true;
        } else {
            ctrlPressed = false;
        }

        if (ctrlPressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            if (!lPressed) {
                layer += 1;
                lPressed = true;
            }
            } else {
            lPressed = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            if (!kPressed) {
                layer -= 1;
                kPressed = true;
            }
            } else {
            kPressed = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                selection_mode = true;
                if(selection_pos[0] == std::vector<int>{0, 0}){
                    selection_pos[0] = {pos_x, pos_y};
                }
            }
            else {
                
                selection_pos[1] = {pos_x, pos_y};
                
            }


            if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
                selection_mode = false;
                selection_pos = { {0, 0}, {0, 0} };
            }
        }
    


       


       
        ImGui::SFML::Update(window, deltaClock.restart());
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open")) { 
                    dialog_open_map_var = true;
                }
            // função para salvar o mapa em json no path
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                ImGui::EndMenu();
            }
            // load sprites
            if (ImGui::BeginMenu("Sprites")) {
                if (ImGui::MenuItem("Load Spritesheet")) {
                    dialog_open_sprite_var = true;
                    
                }
                if(ImGui::MenuItem("Sprite size")){
                    sprite_size_var = true;
                    
                }
                ImGui::EndMenu();
            }
            // grid settings
            if (ImGui::BeginMenu("Grid")) {
                grid_settings = true;
              
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Auto Tile Config")){
                auto_tile_settings = true;
                
                ImGui::EndMenu();
            }

            




            ImGui::EndMainMenuBar();
        }   

        if(selection_mode ==  true||auto_tile_settings == true || grid_settings == true || sprite_size_var == true || dialog_open_sprite_var == true || dialog_open_map_var == true){
            editing = false;
        }else{
            editing = true;
        }
         
      


        // mudar lista de texturas para alocar os path das texturas
        // e carregar as texturas no vetor de texturas
        

        // load map
        if(dialog_open_map_var== true){
            dialog_open_map.Open(&dialog_open_map_var, map_type, &map_path);
        }
       


        // load sprite_sheet
        if(dialog_open_sprite_var == true){
            dialog_open_sprite.Open(&dialog_open_sprite_var, sprite_type, &sprite_path);
            if(sprite_path != "" && spriteSheets.find(sprite_path) == spriteSheets.end()){
                spriteSheets[sprite_path] = load_spritesheet_map(sprite_path, sprite_size);
                Level_Map.TilesConfigs.push_back({sprite_path, {}});
                sprite_path = "";
            }
        }
        // auto tile settings
        if(auto_tile_settings == true){
            ImGui::SetNextWindowSize(ImVec2(250, 250));
            ImGui::Begin("Auto Tile Settings", &auto_tile_settings);
            // mostrar as imagens do spriteSheet 
            // a imagem aparece no meio da window e o usuario seleciona as bordas, as bordas sao quadrados cinzas selecionaveis que ficam verde 
            static bool selectedBorders[8] = { false }; // false: not selected, true: selected
            ImGui::Text("Select Borders:");
            
            if (selected_texture.getSize().x != 0) {
            ImVec2 textureSize(50, 50);
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 texturePos = ImVec2((windowSize.x - textureSize.x) / 2, (windowSize.y - textureSize.y) / 2);
            
            ImGui::SetCursorPos(texturePos);
            ImGui::Image(selected_texture, textureSize);
            
            ImVec2 borderPos[8] = {
            ImVec2(texturePos.x - 50, texturePos.y - 50), // top-left
            ImVec2(texturePos.x, texturePos.y - 50), // top
            ImVec2(texturePos.x + 50, texturePos.y - 50), // top-right
            ImVec2(texturePos.x - 50, texturePos.y), // left
            ImVec2(texturePos.x + 50, texturePos.y), // right
            ImVec2(texturePos.x - 50, texturePos.y + 50), // bottom-left
            ImVec2(texturePos.x, texturePos.y + 50), // bottom
            ImVec2(texturePos.x + 50, texturePos.y + 50) // bottom-right
            };

            const char* borderLabels[8] = {
            "[-1,-1]", "[-1,0]", "[-1,1]", "[0,-1]", "[0,1]", "[1,-1]", "[1,0]", "[1,1]"
            };
            std::vector<std::vector<int>> border_list_label = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};

            for (int i = 0; i < 8; ++i) {
                ImGui::SetCursorPos(borderPos[i]);
                ImGui::PushID(i);
            
                if (ImGui::Button(borderLabels[i], ImVec2(50, 50))) {
                selectedBorders[i] = !selectedBorders[i];
                if (selectedBorders[i]) {
                std::cout << "Selected: " << borderLabels[i] << std::endl;
                    
                    Level_Map.TilesConfigs.back().border_list[selected_texture_id].push_back(border_list_label[i]);
                }
                }
            
                ImGui::PopID();
            }
            } else {
                ImGui::Text("No texture selected.");
            }

            ImGui::End();
        }
       
        // sprite size
        if(sprite_size_var == true){
            ImGui::SetNextWindowSize(ImVec2(200, 100));
            ImGui::Begin("Sprite Size", &sprite_size_var);
            ImGui::InputInt("Size", &sprite_size);
            ImGui::End();
        }
        // grid settings
        if(grid_settings == true){
            ImGui::SetNextWindowSize(ImVec2(300, 100));
            ImGui::Begin("Grid Settings", &grid_settings);
            ImGui::InputInt("Tile Size", &tile_size);
            ImGui::InputInt("Grid Size", &grid_size);
            ImGui::Checkbox("Show Grid", &show_grid);
            grid = createGrid(tile_size, grid_size);
            ImGui::End();
        }
        // side UI
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        ImGui::SetNextWindowSize(ImVec2(300,window.getSize().y-20));
        ImGui::Begin("Sprites");
        if(spriteSheets.size() > 0){
            for(const auto& item: spriteSheets){
                std::filesystem::path path(item.first);
                if(ImGui::Button(path.filename().string().c_str())){
                    selected_spriteSheet = item.first;
                }
            }
            ImGui::Separator(); 
            if(selected_spriteSheet != ""){
                for(const auto& item: spriteSheets[selected_spriteSheet]){      
                    sf::Sprite sprite_img(item);
                    sprite_img.setScale(2.0f, 2.0f);
                    if(ImGui::ImageButton(sprite_img)){
                        selected_texture = item;
                        selected_texture_id = &item - &spriteSheets[selected_spriteSheet][0];
                        selected_texture_path = selected_spriteSheet;
                        
                        
                    }
                }
            }  
        }
        

        ImGui::End();
    
        
       
        


// ------------------------------------------------------


        // Limpar a janela SFML
        window.clear(sf::Color::White);
        
        window.setView(view);
        // draw grid
        if(show_grid){
            window.draw(grid);
        }



        sf::Vector2f viewSize = view.getSize(); 
        sf::Vector2f viewCenter = view.getCenter();
        sf::Vector2f topLeft(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);
        render_pos[0] = (int((topLeft.x + tile_size / 2) / tile_size) * tile_size);
        render_pos[1] = (int((topLeft.y + tile_size / 2) / tile_size) * tile_size);
        // draw map
        Visible_Map = Level_Map.Get_visible(render_pos, int(viewSize.x), int(viewSize.y));
        for(const auto& item: Visible_Map){
            sf::Sprite sprite(spriteSheets[item.second.second][item.second.first]);
            sprite.setPosition(item.first[0], item.first[1]);
            window.draw(sprite);
        }
        

        // draw sprites mouse
        if(!selected_texture.getSize().x == 0){
            
            sf::Sprite sprite(selected_texture);
            
            sprite.setPosition(pos_x, pos_y);
            sprite.setColor(sf::Color(255, 255, 255, 125));
            window.draw(sprite);
        }

         // selection mod 
        if(selection_mode == true){
            sf::RectangleShape selectionRect;
            selectionRect.setPosition(sf::Vector2f(selection_pos[0][0], selection_pos[0][1]));
            if (selection_pos[1] == std::vector<int>{0, 0}) {
                selectionRect.setSize(sf::Vector2f(pos_x - selection_pos[0][0], pos_y - selection_pos[0][1]));
            } else {
                selectionRect.setSize(sf::Vector2f(selection_pos[1][0] - selection_pos[0][0], selection_pos[1][1] - selection_pos[0][1]));
            }
            selectionRect.setFillColor(sf::Color(0, 0, 255, 50)); // Semi-transparent blue
            selectionRect.setOutlineColor(sf::Color(0, 0, 255, 150)); // Blue outline
            selectionRect.setOutlineThickness(1.0f);
            window.draw(selectionRect);
        }   

        
       // Renderizar o ImGui
      
   
        ImGui::SFML::Render(window);
         // Draw layer in the right bottom corner
        sf::Text layerText;
        sf::Font font;
        if (!font.loadFromFile("C:\\Users\\joaod\\Documents\\projeto\\assets\\font\\alterebro-pixel-font.ttf")) {
            // Handle error
        }
        layerText.setFont(font);
        layerText.setString("Layer: " + std::to_string(layer));
        layerText.setCharacterSize(56);
        layerText.setFillColor(sf::Color::Black);
        layerText.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize().x - layerText.getLocalBounds().width - 10, window.getSize().y - layerText.getLocalBounds().height - 30)));
        window.draw(layerText);

        // selection 
        
        
        
        window.display();
    }

    // Encerrar o ImGui-SFML
    ImGui::SFML::Shutdown();

    return 0;
}
