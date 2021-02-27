#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace CosmeticsLoader
{
    class Config
    {
        public:
            Config() {};
            Config(rapidjson::Value& val) {};
    };
}