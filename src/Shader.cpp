
#include "Renderer/Shader.h"
#include <string>

namespace TONGUE {

bool Shader::Load(const std::string&, const std::string&) {
    return true;
}

void Shader::Bind() const {}
void Shader::Unbind() const {}

// Removed SetMatrix and SetVector for now

} // namespace TONGUE
