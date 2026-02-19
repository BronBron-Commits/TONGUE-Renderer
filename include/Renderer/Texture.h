#pragma once
#include <string>

#pragma once
#include <string>

#pragma once

namespace TONGUE
{
    class Texture
    {
    public:
        bool Load(const char* path);
        void Bind(unsigned slot = 0) const;
        void Unbind() const;
    };
}
