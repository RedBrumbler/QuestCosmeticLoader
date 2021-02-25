#include "Descriptor.hpp"

namespace CosmeticLoader
{
    Descriptor::Descriptor(std::string name, std::string author, std::string description) : name(name), author(author), description(description) {}

    Descriptor::Descriptor(rapidjson::Value& val)
    {
        name = val["objectName"].GetString();
        author = val["author"].GetString();
        description = val["description"].GetString();
    }

    const std::string& Descriptor::get_name() const
    {
        return name;
    }

    const std::string& Descriptor::get_author() const
    {
        return author;
    }

    const std::string& Descriptor::get_description() const
    {
        return description;
    }
}