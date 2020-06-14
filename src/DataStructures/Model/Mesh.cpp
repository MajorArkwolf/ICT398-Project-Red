#include "Mesh.hpp"
#include <utility>
#include "View/Renderer/OpenGL.hpp"

Mesh::Mesh(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices,
           std::vector<TextureB> newTextures) {
    this->vertices = std::move(newVertices);
    this->indices  = std::move(newIndices);
    this->textures = std::move(newTextures);

}

void Mesh::Draw(Shader& shader) {
    View::OpenGL::DrawModel(shader, VAO, textures, indices);
}
void Mesh::AddBoneData(unsigned VectorID, unsigned BoneID, float Weight) {
    for (unsigned x = 0; x < 4; ++x) {
        if (vertices.at(VectorID).BoneWeight[x] == 0.0) {
            vertices.at(VectorID).BoneIDs[x]    = BoneID;
            vertices.at(VectorID).BoneWeight[x] = Weight;
            return;
        }
    }
    assert(0);
}

void Mesh::MoveToGPU() {
    View::OpenGL::SetupMesh(VAO, VBO, EBO, this->vertices, this->indices);
}
