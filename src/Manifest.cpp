#include "../shared/Manifest.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"

namespace CosmeticLoader
{
    template<class U, class T>
    Manifest<U, T>::Manifest(std::string filePath) : filePath(filePath)
    {
        std::string json = readfile(filePath);
        rapidjson::Document d;
        d.Parse(json.c_str());

        fileName = d["fileName"].GetString();
        descriptor = T(d["descriptor"]);
        config = U(d["conifg"]);
    }

    template<class U, class T>
    const std::string& Manifest<U, T>::get_filePath() const
    {
        return filePath;
    }

    template<class U, class T>
    const std::string& Manifest<U, T>::get_fileName() const
    {
        return fileName;
    }

    template<class U, class T>
    const T& Manifest<U, T>::get_descriptor() const
    {
        return descriptor;
    }

    template<class U, class T>
    const U& Manifest<U, T>::get_config() const
    {
        return config;
    }
}