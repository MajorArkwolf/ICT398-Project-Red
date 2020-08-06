#include "Mesh.hpp"
#include <utility>
#include "Engine/Renderer/OpenGL.hpp"

Mesh::Mesh(std::vector<Vertex> new_vertices, std::vector<unsigned int> new_indices,
           std::vector<TextureB> new_textures) {
    this->vertices_ = std::move(new_vertices);
    this->indices_  = std::move(new_indices);
    this->textures_ = std::move(new_textures);

}

void Mesh::Draw(Shader& shader) {
    view::OpenGL::DrawModel(shader, vao_, textures_, indices_);
}
void Mesh::AddBoneData(unsigned vector_id, int bone_id, float weight) {
    for (int x = 0; x < 4; ++x) {
        if (vertices.at(VectorID).bone_weight[x] == 0.0f) {
            vertices.at(VectorID).bone_ids[x]    = bone_id;
            vertices.at(VectorID).bone_weight[x] = weight;
            return;
        }
    }
    assert(0);
}

void Mesh::MoveToGPU() {
    view::OpenGL::SetupMesh(vao_, vbo_, ebo_, this->vertices_, this->indices_);
}
