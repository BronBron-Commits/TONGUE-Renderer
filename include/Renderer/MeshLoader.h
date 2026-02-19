#pragma once
#include <string>
#include <memory>
#include "Renderer/Mesh.h"

namespace TONGUE {

std::shared_ptr<Mesh> LoadMeshFromOBJ(const std::string& path);

}
