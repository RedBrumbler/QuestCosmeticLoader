#pragma once
#include <functional>
#include "../shared/AssetBundle.hpp"

namespace CosmeticsLoader
{
    typedef std::function<void(std::string name, Il2CppObject*)> AssetLoaderCallback;
    class AssetLoader
    {
        public:
            AssetLoader(AssetLoaderCallback callback, AssetBundle* bundle, std::string name, Il2CppReflectionType* type) : callback(callback), name(name)
            {
                Asset* asset = bundle->LoadAsset(name, type);
                AssetCallback(asset);
            }

        private:
            void AssetCallback(Asset* asset)
            {
                callback(name, asset);
                delete(this);
            }

            AssetLoaderCallback callback;
            std::string name;
    };
}