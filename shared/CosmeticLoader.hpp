#pragma once
#include <string>
#include <vector>
#include <functional>

typedef std::function<void(Il2CppObject*)> CosmeticCallback;

namespace CosmeticLoader
{
    class CosmeticLoader
    {
        public:

        protected:
            CosmeticCallback callback;
    };
}