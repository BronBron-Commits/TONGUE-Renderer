
#include "Renderer/Shader.h"
#include <string>

namespace TONGUE {

bool Shader::Load(const std::string&, const std::string&) {
    return true;
}

void Shader::Bind() const {}
void Shader::Unbind() const {}

} // namespace TONGUE
