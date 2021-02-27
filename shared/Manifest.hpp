#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "../shared/Descriptor.hpp"
#include "../shared/Config.hpp"
#include <string>

namespace CosmeticsLoader
{
    template<class U = Config, class T = Descriptor>
    class Manifest
    {
        public:
            Manifest(std::string filePath) : filePath(filePath)
            {
                std::string json = readfile(filePath);
                rapidjson::Document d;
                d.Parse(json.c_str());

                fileName = d["androidFileName"].GetString();
                descriptor = T(d["descriptor"]);
                config = U(d["conifg"]);
            }

            /// @brief returrns a const reference to the filepath string
            virtual const std::string& get_filePath() const
            {
                return filePath;
            }

            /// @brief returns the fileName, will need appending of _platform though
            virtual const std::string& get_fileName() const
            {
                return fileName;
            }
            
            /// @brief returns a const reference to the descriptor
            virtual const T& get_descriptor() const
            {
                return descriptor;
            }

            /// @brief returns a const reference to the config
            virtual const U& get_config() const
            {
                return config;
            }

        protected:

            std::string filePath = "";
            // file path for the manifest
            
            std::string fileName = ""; 
            // file name of the bundle

            T descriptor = T();
            // descriptor of template type T

            U config = U();
            // config of template type U
    };
}