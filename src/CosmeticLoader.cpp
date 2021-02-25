#include "../shared/CosmeticLoader.hpp"

namespace CosmeticLoader
{
    template<class T>
    CosmeticLoader<T>::CosmeticLoader(std::string filePath, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(T(filePath), assetName, assetType) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(T manifest, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(manifest, {assetName, assetType}) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(std::string filePath, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(T(filePath), callback, assetName, assetType) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(T manifest, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(manifest, callback, {assetName, assetType}) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(std::string filePath, CosmeticCallback callback, AssetTypeMap assetTypeMap) : CosmeticLoader<T>(T(filePath), callback, assetTypeMap) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(T manifest, CosmeticCallback callback, AssetTypeMap assetTypeMap) : CosmeticLoader<T>(manifest, assetTypeMap)
    {
        this->callback = callback;
        hasCallback = true;
    }

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(std::string filePath, AssetTypeMap assetTypeMap) : CosmeticLoader<T>(T(filePath), assetTypeMap) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(T manifest, AssetTypeMap assetTypeMap) : manifest(manifest)
    {
        this->assetTypeMap = assetTypeMap;
        
        std::function<void(AssetBundle*)> bundleCallback = std::bind(&CosmeticLoader<T>::OnBundleLoaded, this);
        
    	std::string fileName = TransformFilePath(manifest.get_filePath)
        AssetBundle::LoadFromFileAsync(manifest.get_filePath(), bundleCallback);
    }

    template<class T>
    const T& CosmeticLoader<T>::get_manifest() const
    {
        return manifest;
    }

    template<class T>
    Il2CppObject* CosmeticLoader<T>::get_asset(std::string name)
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
    
    template<class T>
    void CosmeticLoader<T>::AddToMap(Il2CppObject* obj, std::string name)
    {
        assetMap[name] = obj;
    }

    template<class T>
    void CosmeticLoader<T>::OnBundleLoaded(AssetBundle* bundle)
    {
        this->bundle = bundle;
        for (auto p : assetTypeMap)
        {
            // bind method for the callback with the name already set, so that way we already know what to use (placeholders do be pog)
            std::function<void(Asset*, std::string name)> assetCallback = std::bind(&CosmeticLoader<T>::OnAssetLoaded, this, std::placeholders::_1, p.key);
            
            // load asset
            bundle->LoadAssetAsync(p.key, assetCallback, p.value);
        }
    }

    template<class T>
    void CosmeticLoader<T>::OnAssetLoaded(Il2CppObject* asset, std::string name)
    {
        asset = CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "Instantiate", asset));
        CRASH_UNLESS(il2cpp_utils::RunMethod("UnityEngine", "Object", "DontDestroyOnLoad", asset));

        // add to our map
        AddToMap(asset, name);

        // call user defined callback
        if (hasCallback) callback(asset, name);
    }

    std::string CosmeticLoader::TransformFilePath(std::string path, std::string newFileName)
    {
        path = path.substr(0, path.find_last_of('/'));
        path += newFileName;
        return path;
    }
}