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
    View::OpenGL::DrawModel(shader, vao_, textures_, indices_);
}
void Mesh::AddBoneData(unsigned vector_id, int bone_id, float weight) {
    for (int x = 0; x < 4; ++x) {
        if (vertices_.at(vector_id).BoneWeight[x] == 0.0f) {
            vertices_.at(vector_id).BoneIDs[x]    = bone_id;
            vertices_.at(vector_id).BoneWeight[x] = weight;
            return;
        }
    }
    assert(0);
}

void Mesh::MoveToGPU() {
    View::OpenGL::SetupMesh(vao_, vbo_, ebo_, this->vertices_, this->indices_);
}
