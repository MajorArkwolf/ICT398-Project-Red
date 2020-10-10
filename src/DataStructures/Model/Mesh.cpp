#include "Mesh.hpp"
#include <utility>
#include "Engine/Renderer/OpenGL.hpp"

Mesh::Mesh(std::vector<Vertex> new_vertices, std::vector<unsigned int> new_indices,
           std::vector<TextureB> new_textures, model::Material material, glm::mat4 transformation) {
    this->vertices_ = std::move(new_vertices);
    this->indices_  = std::move(new_indices);
    this->textures_ = std::move(new_textures);
    this->material_ = material;
    this->transform_ = transformation;
}

void Mesh::Draw(Shader* shader, const glm::mat4& model_matrix) {
    // bind appropriate textures
    unsigned int diffuse_nr = 1;
    unsigned int specular_nr = 1;
    unsigned int normal_nr = 1;
    unsigned int height_nr = 1;
    bool has_texture = false;
    for (unsigned int i = 0; i < textures_.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures_[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuse_nr++);
            has_texture = true;
        }
        else if (name == "texture_specular") {
            number =
                    std::to_string(specular_nr++); // transfer unsigned int to stream
        }
        else if (name == "texture_normal") {
            number = std::to_string(normal_nr++); // transfer unsigned int to stream
        }
        else if (name == "texture_height") {
            number = std::to_string(height_nr++); // transfer unsigned int to stream
        }
        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader->GetID(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }

    //auto matrix = model_matrix * transform_;
    shader->SetMat4("model", model_matrix);
    shader->SetBool("has_texture", has_texture);
    shader->SetVec3("material.ambient", material_.Ambient);
    shader->SetVec3("material.diffuse", material_.Diffuse);
    shader->SetVec3("material.specular", material_.Specular);
    shader->SetFloat("material.shininess", material_.Shininess);

    // draw mesh
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}
void Mesh::AddBoneData(unsigned vector_id, int bone_id, float weight) {
    for (int x = 0; x < 4; ++x) {
        if (vertices_.at(vector_id).bone_weight[x] == 0.0f) {
            vertices_.at(vector_id).bone_ids[x]    = bone_id;
            vertices_.at(vector_id).bone_weight[x] = weight;
            return;
        }
    }
    assert(0);
}

void Mesh::MoveToGPU() {
    view::OpenGL::SetupMesh(vao_, vbo_, ebo_, this->vertices_, this->indices_);
}
