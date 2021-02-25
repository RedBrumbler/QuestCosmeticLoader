#pragma once
#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace CosmeticLoader
{
    class Descriptor
    {
        public:
            Descriptor(std::string name, std::string author, std::string description);
            Descriptor(rapidjson::Value& val);

            virtual const std::string& get_name() const;
            virtual const std::string& get_author() const;
            virtual const std::string& get_description() const;

        protected:
            std::string name;
            std::string author;
            std::string description;
    };
}