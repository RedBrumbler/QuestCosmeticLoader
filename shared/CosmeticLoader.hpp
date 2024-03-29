#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "../shared/Manifest.hpp"
#include "../shared/AssetBundle.hpp"
#include "../shared/AssetLoader.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

#ifdef HAS_CODEGEN
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#endif

static Logger& getLoaderLogger()
{
    static Logger* logger = new Logger({"QuestCosmeticLoader", "0.2.1"}, LoggerOptions(false, true));
    return *logger;
}

namespace CosmeticsLoader
{
    typedef std::map<std::string, Il2CppObject*> AssetMap;
    typedef std::function<void(std::string, Il2CppObject*)> CosmeticCallback;
    typedef std::map<std::string, Il2CppReflectionType*> AssetTypeMap;

    template<class T = Manifest<Config, Descriptor>>
    class CosmeticLoader
    {
        public:
            CosmeticLoader(std::string filePath, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(T(filePath), assetName, assetType) {};
            CosmeticLoader(T manifest, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(manifest, MakeMap(assetName, assetType)) {};

            CosmeticLoader(std::string filePath, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(T(filePath), callback, assetName, assetType) {};
            CosmeticLoader(T manifest, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(manifest, callback, MakeMap(assetName, assetType)) {};

            CosmeticLoader(std::string filePath, CosmeticCallback callback, AssetTypeMap assetTypeMap) : CosmeticLoader<T>(T(filePath), callback, assetTypeMap) {};
            CosmeticLoader(T manifest, CosmeticCallback callback, AssetTypeMap assetTypeMap) : manifest(manifest)
            {
                this->assetTypeMap = assetTypeMap;
                this->callback = callback;
                this->hasCallback = true;
                bundle = LoadBundle();
                OnBundleLoaded(bundle);
            }

            CosmeticLoader(std::string filePath, AssetTypeMap assetTypeMap) : CosmeticLoader<T>(T(filePath), assetTypeMap) {};
            CosmeticLoader(T manifest, AssetTypeMap assetTypeMap) : manifest(manifest)
            {
                this->assetTypeMap = assetTypeMap;

                bundle = LoadBundle();
                OnBundleLoaded(bundle);
            }

            /// @brief returns a const reference to the manifest for this loader
            virtual const T& get_manifest() const
            {
                return manifest;
            }
            
            /// @brief gets the specific loaded asset
            virtual Il2CppObject* get_asset(std::string name = "")
            {
                // if nothing loaded, return nullptr
                if (assetMap.size() == 0) return nullptr;

                // if name not given, return the first thing loaded
                if (name == "") return assetMap.begin()->second;

                // find the name in the map
                AssetMap::iterator it = assetMap.find(name);
                if (it != assetMap.end())
                {
                    return it->second;
                }

                // if not found return nullptr
                return nullptr;
            }

        protected:
            static std::string TransformFilePath(std::string path, std::string newFileName)
            {
                path = path.substr(0, path.find_last_of('/') + 1);
                path += newFileName;
                return path;
            }

            virtual void AddToMap(Il2CppObject* obj, std::string name)
            {
                assetMap[name] = obj;
            }
            
            virtual void OnBundleLoaded(AssetBundle* bundle)
            {
                this->bundle = bundle;
                if (!bundle) return;
                for (auto p : assetTypeMap)
                {
                    using namespace std::placeholders;
                    // bind method for the callback with the name already set, so that way we already know what to use (placeholders do be pog)
                    AssetLoaderCallback assetCallback = std::bind(&CosmeticLoader<T>::OnAssetLoaded, this, _1, _2);
                    getLoaderLogger().info("Starting asset load for %s", p.first.c_str());
                    // load asset
                    new AssetLoader(assetCallback, bundle, p.first, p.second);
                }
            }

            virtual void OnAssetLoaded(std::string name, Il2CppObject* asset)
            {
                if (!asset) 
                {
                    getLoaderLogger().error("Asset was nullptr, returning");
                    return;
                }
                else getLoaderLogger().info("Asset %s loaded succesfully", name.c_str());

                #ifdef HAS_CODEGEN
                UnityEngine::GameObject* newAsset = UnityEngine::Object::Instantiate((UnityEngine::GameObject*)asset);
                UnityEngine::Object::DontDestroyOnLoad(newAsset);
                #else
                Il2CppObject* newAsset = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "Instantiate", asset));
                CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "DontDestroyOnLoad", newAsset));
                #endif

                // add to our map
                AddToMap(newAsset, name);

                // call user defined callback
                if (hasCallback) callback(name, newAsset);
                else getLoaderLogger().info("There was no callback to call for asset %s", name.c_str());
            }
            
            AssetBundle* bundle = nullptr;

            AssetTypeMap assetTypeMap = {};
            AssetMap assetMap = {};

            bool hasCallback = false;
            CosmeticCallback callback;
            
            T manifest;
        private:
            AssetBundle* LoadBundle()
            {
                std::vector<uint8_t> data = {};
                
                if (!ZipUtils::GetBytesFromZipFile(manifest.get_filePath(), manifest.get_fileName(), data))
                {
                    getLoaderLogger().info("Error loading bundle '%s' from path '%s'", manifest.get_fileName().c_str(), manifest.get_filePath().c_str());
                    return nullptr;
                }
                
                AssetBundle* bundle = AssetBundle::LoadFromMemory(data);
                return bundle;
            } 

            static AssetTypeMap MakeMap(std::string name, Il2CppReflectionType* type)
            {
                CosmeticsLoader::AssetTypeMap newMap = {};
                newMap[name] = type;
                return newMap;
            }
    };
}