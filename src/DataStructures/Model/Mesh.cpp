#include "Mesh.hpp"
#include <utility>
#include "Engine/Renderer/OpenGL.hpp"

Mesh::Mesh(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices,
           std::vector<TextureB> newTextures) {
    this->vertices = std::move(newVertices);
    this->indices  = std::move(newIndices);
    this->textures = std::move(newTextures);

}

void Mesh::Draw(Shader& shader) {
    View::OpenGL::DrawModel(shader, VAO, textures, indices);
}
void Mesh::AddBoneData(unsigned VectorID, int BoneID, float Weight) {
    for (int x = 0; x < 4; ++x) {
        if (vertices.at(VectorID).bone_weight[x] == 0.0f) {
            vertices.at(VectorID).bone_ids[x]    = BoneID;
            vertices.at(VectorID).bone_weight[x] = Weight;
            return;
        }
    }
    assert(0);
}

void Mesh::MoveToGPU() {
    View::OpenGL::SetupMesh(VAO, VBO, EBO, this->vertices, this->indices);
}
