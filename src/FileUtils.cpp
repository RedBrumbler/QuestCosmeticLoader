#include "FileUtils.hpp"
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <filesystem>
#include "beatsaber-hook/shared/utils/utils.h"

using namespace std;

static Logger& getUtilsLogger()
{
    static Logger* logger = new Logger({"Cosmetics Loader File Utils", "0.1.0"}, LoggerOptions(false, true));
    return *logger;
}

#define INFO(value...) getUtilsLogger().info(value);
#define ERROR(value...) getUtilsLogger().error(value);

namespace FileUtils
{
    string GetFileName(string path, bool removeExtension)
    {
        if (path.find('/')) path.erase(path.find_last_of('/'));
        if (removeExtension) path = remove_extension(path);
        return path;
    }

    string remove_extension(string path)
    {
        return path.erase(path.find_last_of('.') - 1);
    }

    string GetExtension(string path)
    {
        if(path.find_last_of(".") != string::npos)
            return path.substr(path.find_last_of(".")+1);
        return "";
    }

    bool GetFilesInFolderPath(string extension, string filePath, vector<string>& out)
    {
        INFO("Finding files in %s", filePath.c_str());
        bool foundTheExtension = false; 
        DIR* fileDir = opendir(filePath.data());
        struct dirent *files;
        if(fileDir != NULL)
        {
            while((files = readdir(fileDir)) != NULL)
            {
                char* nameptr = files->d_name;
                if (!nameptr)
                {
                    ERROR("File name was nullptr, skipping...");
                    continue;
                }
                string fileName(nameptr);
                if (fileName == "." || fileName == "..") continue;
                string foundExtension = GetExtension(fileName);
                if(foundExtension.find(extension) != string::npos)
                {
                    out.push_back(fileName);
                    foundTheExtension = true; 
                }
            }
            closedir(fileDir);
        }
        return foundTheExtension;
    }  
    void mkdir(string dir) 
    { 
        makeFolder(dir); 
    }

    void makeFolder(string directory)
    {
        if (!direxists(directory.c_str()))
        {    
            string theDir = "";
            for (auto c : directory)
            {
                if (c == ' ')
                {
                theDir += "\\ ";
                }
                else theDir += c;
            }
            int makePath = mkpath(theDir.data());
            if (makePath == -1)
            {
                ERROR("Failed to make path %s", directory.c_str());
            }
        }
    }
}