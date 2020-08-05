#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <glad/glad.h>  // NOLINT
#include <glm/glm.hpp>

class Shader {
 public:
  /**
   * Constructs a shader on the fly
   * @param vertex path the vertex shader
   * @param fragment path the fragment shader
   * @param geo path the geometry shader
   */
  Shader(const std::filesystem::path &vertex,
         const std::filesystem::path &fragment,
         const std::filesystem::path &geo);

  /**
   * Activates the shader program
   */
  void Use() const;

  /**
   * Sets a boolean in the shader program.
   * @param name of the variable.
   * @param value to set the variable to.
   */
  void SetBool(const std::string &name, bool value) const;

  /**
   * Sets a integer in the shader program.
   * @param name of the variable.
   * @param value to set the variable to.
   */
  void SetInt(const std::string &name, int value) const;

  /**
   * Sets a float in the shader program.
   * @param name of the variable.
   * @param value to set the variable to.
   */
  void SetFloat(const std::string &name, float value) const;

  /**
   * Sets a vec2 in the shader program.
   * @param name of the variable.
   * @param value to set the variable to.
   */
  void SetVec2(const std::string &name, const glm::vec2 &value) const;

  /**
   * Overloaded function, sets a vec2 in the shader program.
   * @param name of the variable.
   * @param x sets the x value.
   * @param y sets the y value.
   */
  void SetVec2(const std::string &name, float x, float y) const;

  /**
   * Sets a vec3 in the shader program.
   * @param name of the variable.
   * @param value to set the variable to.
   */
  void SetVec3(const std::string &name, const glm::vec3 &value) const;

  /**
   * Overloaded function, sets a vec3 in the shader program.
   * @param name of the variable.
   * @param x sets the x value.
   * @param y sets the y value.
   * @param z sets the z value.
   */
  void SetVec3(const std::string &name, float x, float y, float z) const;

  /**
   * Sets a vec4 in the shader program.
   * @param name of the variable.
   * @param value to set the variable to.
   */
  void SetVec4(const std::string &name, const glm::vec4 &value) const;

  /**
   * Overloaded function, sets a vec4 in the shader program.
   * @param name of the variable.
   * @param x sets the x value.
   * @param y sets the y value.
   * @param z sets the z value.
   * @param w sets the w value.
   */
  void SetVec4(const std::string &name, float x, float y, float z, float w)
      const;

  /**
   * Sets a Mat2 in the shader program.
   * @param name of the variable.
   * @param mat to set the variable to.
   */
  void SetMat2(const std::string &name, const glm::mat2 &mat) const;

  /**
   * Sets a Mat3 in the shader program.
   * @param name of the variable.
   * @param mat to set the variable to.
   */
  void SetMat3(const std::string &name, const glm::mat3 &mat) const;

  /**
   * Sets a Mat4 in the shader program.
   * @param name of the variable.
   * @param mat to set the variable to.
   */
  void SetMat4(const std::string &name, const glm::mat4 &mat) const;

  /**
   * Pass a vector of Mat4's into the shader.
   * @param name of the variable
   * @param mat_array vector contain matrices, capped at 100.
   */
  void SetMat4Array(const std::string &name,
                    const std::vector<glm::mat4> &mat_array)
      const;

  /**
   * Returns the ID of the given shader in use.
   * @return ID for shader.
   */
  unsigned int GetID() const;

 private:
  // utility function for checking shader compilation/linking errors.
  void CheckCompileErrors(GLuint shader, const std::string &type);

  unsigned int id_ = {};
};
