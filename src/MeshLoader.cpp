#include "Renderer/MeshLoader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace TONGUE {

struct SimpleVertex {
    float x, y, z;
};

std::shared_ptr<Mesh> LoadMeshFromOBJ(const std::string& path) {
    std::cout << "[MeshLoader] Loading OBJ: " << path << std::endl;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "[MeshLoader] Failed to open OBJ file!" << std::endl;
        return nullptr;
    }
    std::vector<SimpleVertex> simpleVerts;
    std::vector<uint32_t> indices;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            SimpleVertex v;
            iss >> v.x >> v.y >> v.z;
            simpleVerts.push_back(v);
        } else if (type == "f") {
            uint32_t idx[4];
            int count = 0;
            while (iss >> idx[count]) {
                idx[count] -= 1; // OBJ is 1-based
                count++;
            }
            if (count == 3) {
                indices.push_back(idx[0]);
                indices.push_back(idx[1]);
                indices.push_back(idx[2]);
            } else if (count == 4) {
                // quad: split into two triangles
                indices.push_back(idx[0]); indices.push_back(idx[1]); indices.push_back(idx[2]);
                indices.push_back(idx[0]); indices.push_back(idx[2]); indices.push_back(idx[3]);
            }
        }
    }
    // Convert to Vertex format for Mesh::Upload
    std::vector<Vertex> verts;
    for (const auto& v : simpleVerts) {
        Vertex vert;
        vert.position = {v.x, v.y, v.z};
        vert.normal = {0, 0, 0}; // No normals in simple OBJ
        vert.uv = {0, 0}; // No UVs in simple OBJ
        verts.push_back(vert);
    }
    auto mesh = std::make_shared<Mesh>();
    mesh->Upload(verts, indices);
    return mesh;
}

} // namespace TONGUE
