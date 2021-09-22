#pragma once
// copied from Sc2ad bs-utils https://github.com/sc2ad/QuestBS-Utils

#include <functional>
#include <vector>

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"

static ModInfo info = {"QuestCosmeticLoader", "0.2.1"};

static Logger& getBundleLogger()
{
    static Logger* logger = new Logger(info, LoggerOptions(false, true));
    return *logger;
}

static bool CheckAssetClass(Il2CppObject* asset);
static Il2CppString* CreateIl2CppStringPathAndEnsureExists(std::string_view filePath);

namespace CosmeticsLoader {
    typedef Il2CppObject Asset;

    /// @class A C++ wrapper around the C# AssetBundle type.
    class AssetBundle : public Il2CppObject {
      typedef std::function<void(Asset*)> AssetCallback;
      typedef std::function<void(AssetBundle*)> AssetBundleCallback;

      public:
        AssetBundle() = delete;
        /// @brief Loads an asset bundle from the provided file path, then calls the given callback.
        /// @param filePath The file path of the asset bundle to load.
        /// @param callback The callback to perform after the asset bundle has been loaded.
        /// @return True if successful, false otherwise.
        static bool LoadFromFileAsync(std::string_view filePath, AssetBundleCallback callback) 
        {
            getBundleLogger().info("Loading bundle @ %s", filePath.data());
            auto* assetFilePath = RET_0_UNLESS(getBundleLogger(), CreateIl2CppStringPathAndEnsureExists(filePath));

            auto* bundleAsync = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::RunMethod("UnityEngine", "AssetBundle", "LoadFromFileAsync", assetFilePath));
            RET_0_UNLESS(getBundleLogger(), il2cpp_utils::SetPropertyValue(bundleAsync, "allowSceneActivation", true));

            auto method = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::FindMethodUnsafe(bundleAsync, "add_completed", 1));
            auto action = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::MakeDelegate(method, 0, new AssetBundleCallback(callback), AssetBundleComplete));

            RET_0_UNLESS(getBundleLogger(), il2cpp_utils::RunMethod(bundleAsync, method, action));
            getBundleLogger().info("Began loading asset bundle '%s' async.", filePath.data());
            return true;
        }
        /// @brief Loads an asset bundle from the provided file path, blocking until finished.
        /// @param filePath The file path of the asset bundle to load.
        /// @return The AssetBundle* loaded (nullptr on failure).
        static AssetBundle* LoadFromFile(std::string_view filePath) 
        {
            auto* assetFilePath = RET_0_UNLESS(getBundleLogger(), CreateIl2CppStringPathAndEnsureExists(filePath));
            return RET_0_UNLESS(getBundleLogger(), il2cpp_utils::RunMethod<AssetBundle*>("UnityEngine", "AssetBundle", "LoadFromFile", assetFilePath));
        }

        static AssetBundle* LoadFromMemory(std::vector<uint8_t>& data)
        {
            getBundleLogger().info("loading bundle from %lu bytes in memory", data.size());
            using LoadFromMemory = function_ptr_t<AssetBundle*, Array<uint8_t>*, unsigned int>;
            static LoadFromMemory loadFromMemory = reinterpret_cast<LoadFromMemory>(il2cpp_functions::resolve_icall("UnityEngine.AssetBundle::LoadFromMemory_Internal"));
            Array<uint8_t>* dataArray = il2cpp_utils::vectorToArray(data);
            return loadFromMemory(dataArray, 0);
        }

        /// @brief Loads the asset with the given name and type from this bundle, then calls the given callback.
        /// @param assetName The name of the asset to load.
        /// @param callback The callback to perform after the asset has been loaded.
        /// @param assetType The Il2CppReflectionType* of the asset to load. Defaults to UnityEngine.GameObject if nullptr.
        /// @return True if successful, false otherwise.
        bool LoadAssetAsync(std::string_view assetName, AssetCallback callback, Il2CppReflectionType* assetType) 
        {
            if (!assetType) assetType = CRASH_UNLESS(il2cpp_utils::GetSystemType("UnityEngine", "GameObject"));
            auto* nameStr = CRASH_UNLESS(il2cpp_utils::createcsstr(assetName));
            getBundleLogger().info("Loading asset '%s' asynchronously.", assetName.data());

            auto* assetAsync = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::RunMethod(this, "LoadAssetAsync", nameStr, assetType));

            auto* method = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::FindMethodUnsafe(assetAsync, "add_completed", 1));
            auto* action = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::MakeDelegate(method, 0, new AssetCallback(callback), AssetComplete));

            RET_0_UNLESS(getBundleLogger(), il2cpp_utils::RunMethod(assetAsync, method, action));
            getBundleLogger().info("Began loading asset async");
            return true;
        }

        /// @brief Loads the asset with the given name and type from this bundle, blocking until finished.
        /// @param assetName The name of the asset to load.
        /// @param assetType The Il2CppReflectionType* of the asset to load. Defaults to UnityEngine.GameObject if nullptr.
        /// @return The Asset* loaded (nullptr on failure).
        Asset* LoadAsset(std::string_view assetName, Il2CppReflectionType* assetType) 
        {
            if (!assetType) assetType = CRASH_UNLESS(il2cpp_utils::GetSystemType("UnityEngine", "GameObject"));
            auto* nameStr = CRASH_UNLESS(il2cpp_utils::createcsstr(assetName));

            getBundleLogger().info("Loading asset '%s' synchronously.", assetName.data());
            
            //std::vector<Il2CppClass*> gen = std::vector<Il2CppClass*>{};

            //auto* ___internal__method = ::il2cpp_utils::FindMethod(this, "LoadAsset", gen, ::il2cpp_utils::ExtractTypes(nameStr, assetType));            
            return RET_0_UNLESS(getBundleLogger(), il2cpp_utils::RunMethod(this, "LoadAsset", nameStr, assetType));
        }



      private:
        static void AssetBundleComplete(AssetBundleCallback* callback, Il2CppObject* assetBundleCreateRequest) 
        {
            getBundleLogger().info("UnityAssetLoader: AssetBundleCreateRequestComplete Called!");
            auto* assetBundle = RET_V_UNLESS(getBundleLogger(), il2cpp_utils::GetPropertyValue<AssetBundle*>(assetBundleCreateRequest, "assetBundle"));
            (*callback)(assetBundle);
            il2cpp_utils::RunMethod(assetBundleCreateRequest, "Finalize");
            getBundleLogger().info("UnityAssetLoader: AssetBundleCreateRequestComplete Finished!");
        }
        
        static void AssetComplete(AssetCallback* callback, Il2CppObject* assetBundleRequest) 
        {
            getBundleLogger().info("AssetBundle::AssetComplete called!");
            CRASH_UNLESS(il2cpp_utils::GetPropertyValue<bool>(assetBundleRequest, "isDone").value_or(false));
            auto* asset = RET_V_UNLESS(getBundleLogger(), il2cpp_utils::GetPropertyValue<Asset*>(assetBundleRequest, "asset"));
            (*callback)(asset);
            il2cpp_utils::RunMethod(assetBundleRequest, "Finalize");
            getBundleLogger().info("AssetBundle::AssetComplete finished!");
        }
    };
}
DEFINE_IL2CPP_ARG_TYPE(CosmeticsLoader::AssetBundle*, "UnityEngine", "AssetBundle");

bool CheckAssetClass(Il2CppObject* asset) 
{
    auto* cAsset = CRASH_UNLESS(il2cpp_functions::object_get_class(asset));
    if (cAsset == il2cpp_functions::defaults->string_class) {
        Il2CppString* str = reinterpret_cast<Il2CppString*>(asset);
        getBundleLogger().error("asset is string?! '%s'", to_utf8(csstrtostr(str)).c_str());
        return false;
    } else {
        getBundleLogger().info("asset is %s ?", il2cpp_utils::ClassStandardName(cAsset).c_str());
    }
    return true;
}

Il2CppString* CreateIl2CppStringPathAndEnsureExists(std::string_view filePath) 
{
    auto* assetFilePath = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::createcsstr(filePath));
    bool pathExists = RET_0_UNLESS(getBundleLogger(), il2cpp_utils::RunMethod<bool>("System.IO", "File", "Exists", assetFilePath));
    RET_0_UNLESS(getBundleLogger(), pathExists);
    return assetFilePath;
}
