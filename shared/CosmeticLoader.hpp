#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "../shared/Manifest.hpp"
#include "../shared/AssetBundle.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

namespace CosmeticLoader
{
    template<class T = Manifest<Config, Descriptor>>
    class CosmeticLoader
    {
        typedef std::map<std::string, Il2CppObject*> AssetMap;
        typedef std::function<void(std::string, Il2CppObject*)> CosmeticCallback;
        typedef std::map<std::string, Il2CppReflectionType*> AssetTypeMap;

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
            virtual void AddToMap(Il2CppObject* obj, std::string name);
            
            virtual void OnBundleLoaded(AssetBundle* bundle);

            virtual void OnAssetLoaded(Asset* asset, std::string name);
            
            AssetBundle* bundle;

            AssetTypeMap assetTypeMap = {};
            AssetMap assetMap = {};

            bool hasCallback = false;
            CosmeticCallback callback;
            
            T manifest;
    };
}