#pragma once
#include <string>

#pragma once
#include <string>
#include "Renderer/MathTypes.h"

#pragma once
#include <string>

namespace TONGUE
{
    class Shader
    {
    public:
        bool Load(const std::string& vert, const std::string& frag);
        void Bind() const;
        void Unbind() const;
    };
}
