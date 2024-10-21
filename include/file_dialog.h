#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

class FileDialog {
public:
    FileDialog(const std::string& config, const std::string& path_var);

    void Open(bool *open, std::string& type, std::string* path);
    void Save(bool *open);
    void Dialog(bool *open);


    private:
        std::string path_string;
        std::string config;
        std::filesystem::path path;
        std::filesystem::path file_path;


};

#endif // FILE_DIALOG_H