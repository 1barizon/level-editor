#include "../include/file_dialog.h"
#include <iostream>
#include <filesystem>
#include <imgui-SFML.h>
#include <imgui.h>
#include <string>



FileDialog::FileDialog(const std::string& config, const std::string& path_var){

    this->config = config;
    path_string = path_var;
    path = std::filesystem::path(path_var);
}





void FileDialog::Open(bool *open, std::string& type, std::string* subject_path) {



    ImGui::SetNextWindowSize(ImVec2(600,600));
    ImGui::Begin("File Dialog", open);
    ImGui::Text("Current Path: %s", path_string.c_str()); 
    ImGui::SameLine(ImGui::GetWindowWidth() - 80);
    if (ImGui::ArrowButton("Back", ImGuiDir_Left)) {
        path = path.parent_path();
        path_string = path.string();
    }
    ImGui::Separator();
    ImGui::Columns(3, "mycolumns");
    ImGui::Text("Folder Name");
    ImGui::NextColumn();
    ImGui::Text("File Name");
    ImGui::NextColumn();
    ImGui::Text("Type");
    ImGui::Separator();
    ImGui::Columns(1);


    ImGui::Columns(3, "");
    // iteracao nas pastas
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            std::string folder_name = entry.path().filename().string();
            if (ImGui::Selectable(folder_name.c_str())) {
                path = entry.path();
                path_string = path.string();
            }
        }
    }

    ImGui::NextColumn();
    // iteracao nos arquivos

    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string file_name = entry.path().filename().string();
            if (ImGui::Selectable(file_name.c_str())) {
                file_path = entry.path();

            }
        }
    }

    ImGui::NextColumn();
    // extesion
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string file_name = entry.path().filename().extension().string();
            ImGui::Text(file_name.c_str());
        }
    }

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::Columns(3, "mycolumns");

    if(std::filesystem::is_regular_file(file_path)) {
        ImGui::Text("File Path: %s", file_path.filename().string().c_str());
    }
    ImGui::NextColumn();

    if (ImGui::Button("Cancel")) {
        *open = false;
    }
    ImGui::NextColumn();

    if (ImGui::Button("Open")) {
        if ( file_path.filename().extension().string() == type) {
            std::cout << "File Opened" << std::endl;
            *open = false;
            
            *subject_path =  file_path.string(); 

        }
    }

    ImGui::End();
}