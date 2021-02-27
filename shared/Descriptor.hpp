#pragma once
#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace CosmeticsLoader
{
    class Descriptor
    {
        public:
            Descriptor() {}
            Descriptor(std::string name, std::string author, std::string description) : name(name), author(author), description(description) {}
            Descriptor(rapidjson::Value& val)
            {
                name = val["objectName"].GetString();
                author = val["author"].GetString();
                description = val["description"].GetString();
            }

            virtual const std::string& get_name() const
            {
                return name;
            }

            virtual const std::string& get_author() const
            {
                return author;
            }

            virtual const std::string& get_description() const
            {
                return description;
            }

        protected:
            std::string name = "";
            std::string author = "";
            std::string description = "";
    };
}