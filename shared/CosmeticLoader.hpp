#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "../shared/Manifest.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

typedef std::map<std::string, Il2CppObject*> AssetMap;
typedef std::function<void(std::string, Il2CppObject*)> CosmeticCallback;
typedef std::map<std::string, Il2CppReflectionType*> AssetTypeMap;

namespace CosmeticLoader
{
    template<class T = Manifest<Config, Descriptor>>
    class CosmeticLoader
    {
        public:
            CosmeticLoader(std::string filePath, std::string assetName, Il2CppReflectionType* assetType);
            CosmeticLoader(T manifest, std::string assetName, Il2CppReflectionType* assetType);

            CosmeticLoader(std::string filePath, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType);
            CosmeticLoader(T manifest, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType);

            CosmeticLoader(std::string filePath, CosmeticCallback callback, AssetTypeMap assetTypeMap);
            CosmeticLoader(T manifest, CosmeticCallback callback, AssetTypeMap assetTypeMap);

            CosmeticLoader(std::string filePath, AssetTypeMap assetTypeMap);
            CosmeticLoader(T manifest, AssetTypeMap assetTypeMap);

            /// @brief returns a const reference to the manifest for this loader
            virtual const T& get_manifest() const;
            
            /// @brief gets the specific loaded asset
            virtual Il2CppObject* get_asset(std::string name = "");

        protected:
            virtual void AddToMap(Il2CppObject* obj, std::string name)
            {
                assetMap[name] = obj;
            }
            
            virtual void OnBundleLoaded(Il2CppObject* bundle)
            {

            }

            virtual void OnAssetLoaded(Il2CppObject* asset, std::string name)
            {
                AddToMap(asset, name);
            }

            AssetMap assetMap = {};
            std::vector<std::string> assetNames = {};
            
            bool hasCallback = false;
            CosmeticCallback callback;
            T manifest;
    };
}