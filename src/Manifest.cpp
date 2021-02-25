#include "Manifest.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"

namespace CosmeticLoader
{
    const std::string& Manifest::get_filePath() const
    {
        return filePath;
    }

    const std::string& Manifest::get_fileName() const
    {
        return fileName;
    }
}