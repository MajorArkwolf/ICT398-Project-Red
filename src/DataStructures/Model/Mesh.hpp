#pragma once

#include <string>
#include <vector>

#include <assimp/anim.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Model/Models/DataTypes.hpp"
#include "View/Renderer/DrawStruct.hpp"
#include "View/Renderer/Shader.hpp"

class Mesh {
  public:
    /// Vertices used in the mesh.
    std::vector<Vertex> vertices = {};
    /// Indices used in the mesh.
    std::vector<unsigned int> indices = {};
    /// Textures used in a mesh.
    std::vector<TextureB> textures = {};
    /// Index buffer location.
    unsigned int VAO = {};

    /**
     * Constructs a mesh object.
     * @param newVertices vertices used in the mesh.
     * @param newIndices indices used in the mesh.
     * @param newTextures textures used in a mesh.
     */
    Mesh(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices,
         std::vector<TextureB> newTextures);

    /**
     * Draw function for the model.
     * @param shader used to draw the model.
     */
    void Draw(Shader& shader);

    /**
     * Add bone data to the the given vertix.
     * @param VectorID vertex to add bone data too
     * @param BoneID the bone id
     * @param Weight the weight of the given bone
     */
    void AddBoneData(unsigned VectorID, unsigned BoneID, float Weight);

    /**
     * Move the data onto the GPU
     */
    void MoveToGPU();

  private:
    /// Buffer ID's.
    unsigned int VBO = 0, EBO = 0;
};
