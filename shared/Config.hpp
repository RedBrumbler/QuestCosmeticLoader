#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace CosmeticLoader
{
    class Config
    {
        public:
            Config(rapidjson::Value& val);
    }
}