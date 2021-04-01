#pragma once
#include <string>
#include <vector>
#include "zip/shared/zip.h"

namespace ZipUtils
{
    bool GetBytesFromZipFile(std::string zipPath, std::string fileName, std::vector<uint8_t>& out);
    
    namespace 
    {
        bool ReadZipFile(zip* z, std::string fileName, std::vector<uint8_t>& out);
    }
}