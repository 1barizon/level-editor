cmake_minimum_required(VERSION 3.5.0)
project(projecto VERSION 0.1.0 LANGUAGES C CXX)

# Definir a versão do padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_library(file_dialog src/file_dialog.cpp)
add_library(spritesheet_loader src/spritesheet_loader.cpp)
add_library(map src/map.cpp)
add_executable(projecto src/main.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)


target_link_libraries(projecto sfml-graphics sfml-window sfml-system ImGui-SFML::ImGui-SFML file_dialog spritesheet_loader nlohmann_json::nlohmann_json map)
# Vincule as bibliotecas ao file_dialog
target_link_libraries(file_dialog ImGui-SFML::ImGui-SFML)
target_link_libraries(spritesheet_loader sfml-graphics)
target_link_libraries(map nlohmann_json::nlohmann_json)



# Defina o caminho onde as DLLs da SFML estão localizadas
set(SFML_DLL_DIR "C:\\Users\\joaod\\vcpkg\\installed\\x64-windows\\debug\\bin")

# Após o build, copie as DLLs para a pasta de saída do executável
add_custom_command(TARGET projecto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SFML_DLL_DIR}/sfml-graphics-d-2.dll"
    "${SFML_DLL_DIR}/sfml-window-d-2.dll"
    "${SFML_DLL_DIR}/sfml-system-d-2.dll"
    "${SFML_DLL_DIR}/zlibd1.dll"
    "${SFML_DLL_DIR}/bz2d.dll"
    "${SFML_DLL_DIR}/libpng16d.dll"

    $<TARGET_FILE_DIR:projecto>
)
target_include_directories(projecto PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_include_directories(file_dialog PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_include_directories(spritesheet_loader PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_include_directories(map PRIVATE "${CMAKE_SOURCE_DIR}/include")