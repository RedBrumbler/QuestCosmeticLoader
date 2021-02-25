#include "../shared/CosmeticLoader.hpp"

namespace CosmeticLoader
{
    template<class T>
    CosmeticLoader<T>::CosmeticLoader(std::string filePath, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(T(filePath), assetName, assetType) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(T manifest, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(manifest, (AssetTypeMap){assetName, assetType}) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(std::string filePath, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(T(filePath), callback, assetName, assetType) {};

    template<class T>
    CosmeticLoader<T>::CosmeticLoader(T manifest, CosmeticCallback callback, std::string assetName, Il2CppReflectionType* assetType) : CosmeticLoader<T>(manifest, callback, (AssetTypeMap){assetName, assetType}) {};

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
    }

    template<class T>
    const T& CosmeticLoader<T>::get_manifest() const
    {
        return manifest;
    }

    template<class T>
    Il2CppObject* CosmeticLoader<T>::get_asset(std::string name)
    {
        if (name = "") return assetMap.begin()->second;
        
        AssetMap::iterator it = assetMap.find(name);
        if (it != assetMap.end())
        {
            return it->second;
        }
        return nullptr;
    }
    
}