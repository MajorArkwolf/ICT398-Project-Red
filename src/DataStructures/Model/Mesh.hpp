#pragma once

#include <assimp/anim.h>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "DataStructures/Model/DataTypes.hpp"
#include "Engine/Renderer/DrawStruct.hpp"
#include "Engine/Renderer/Shader.hpp"

class Mesh {
  public:
    /// Vertices used in the mesh.
    std::vector<Vertex> vertices_ = {};
    /// Indices used in the mesh.
    std::vector<unsigned int> indices_ = {};
    /// Textures used in a mesh.
    std::vector<TextureB> textures_ = {};
    /// Index buffer location.
    unsigned int vao_ = {};

    /**
     * Constructs a mesh object.
     * @param new_vertices vertices used in the mesh.
     * @param new_indices indices used in the mesh.
     * @param new_textures textures used in a mesh.
     */
    Mesh(std::vector<Vertex> new_vertices, std::vector<unsigned int> new_indices,
         std::vector<TextureB> new_textures);

    /**
     * Draw function for the model.
     * @param shader used to draw the model.
     */
    void Draw(Shader& shader);

    /**
     * Add bone data to the the given vertix.
     * @param vector_id vertex to add bone data too
     * @param bone_id the bone id
     * @param weight the weight of the given bone
     */
    void AddBoneData(unsigned vector_id, int bone_id, float weight);

    /**
     * Move the data onto the GPU
     */
    void MoveToGPU();

  private:
    /// Buffer ID's.
    unsigned int vbo_ = 0, ebo_ = 0;
};
