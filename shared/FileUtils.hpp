#pragma once
#include <string>
#include<vector>

namespace FileUtils
{
    void makeFolder(std::string directory);
    void mkdir(std::string dir);
    std::string GetFileName(std::string path, bool removeExtension = false);
    std::string GetExtension(std::string path);
    std::string remove_extension(std::string path);
    bool GetFilesInFolderPath(std::string extension, std::string filePath, std::vector<std::string>& out);
} 