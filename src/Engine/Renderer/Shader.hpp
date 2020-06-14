#pragma once
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

using std::string;

class Shader {
  public:
    /**
     * Constructs a shader on the fly
     * @param vertexPath path the vertex shader
     * @param fragmentPath path the fragment shader
     * @param geometryPath path the geometry shader
     */
    Shader(const char *vertexPath, const char *fragmentPath,
           const char *geometryPath = nullptr);

    /**
     * Activates the shader program
     */
    void use() const;

    /**
     * Sets a boolean in the shader program.
     * @param name of the variable.
     * @param value to set the variable to.
     */
    void setBool(const std::string &name, bool value) const;

    /**
    * Sets a integer in the shader program.
    * @param name of the variable.
    * @param value to set the variable to.
    */
    void setInt(const std::string &name, int value) const;

    /**
    * Sets a float in the shader program.
    * @param name of the variable.
    * @param value to set the variable to.
    */
    void setFloat(const std::string &name, float value) const;

    /**
     * Sets a vec2 in the shader program.
     * @param name of the variable.
     * @param value to set the variable to.
     */
    void setVec2(const std::string &name, const glm::vec2 &value) const;

    /**
     * Overloaded function, sets a vec2 in the shader program.
     * @param name of the variable.
     * @param x sets the x value.
     * @param y sets the y value.
     */
    void setVec2(const std::string &name, float x, float y) const;

    /**
     * Sets a vec3 in the shader program.
     * @param name of the variable.
     * @param value to set the variable to.
     */
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    /**
     * Overloaded function, sets a vec3 in the shader program.
     * @param name of the variable.
     * @param x sets the x value.
     * @param y sets the y value.
     * @param z sets the z value.
     */
    void setVec3(const std::string &name, float x, float y, float z) const;

    /**
     * Sets a vec4 in the shader program.
     * @param name of the variable.
     * @param value to set the variable to.
     */
    void setVec4(const std::string &name, const glm::vec4 &value) const;

    /**
     * Overloaded function, sets a vec4 in the shader program.
     * @param name of the variable.
     * @param x sets the x value.
     * @param y sets the y value.
     * @param z sets the z value.
     * @param w sets the w value.
     */
    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    /**
     * Sets a Mat2 in the shader program.
     * @param name of the variable.
     * @param mat to set the variable to.
     */
    void setMat2(const std::string &name, const glm::mat2 &mat) const;

    /**
     * Sets a Mat3 in the shader program.
     * @param name of the variable.
     * @param mat to set the variable to.
     */
    void setMat3(const std::string &name, const glm::mat3 &mat) const;

    /**
     * Sets a Mat4 in the shader program.
     * @param name of the variable.
     * @param mat to set the variable to.
     */
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    /**
     * Pass a vector of Mat4's into the shader.
     * @param name of the variable
     * @param matArray vector contain matrices, capped at 100.
     */
    void setMat4Array(const std::string &name, const std::vector<glm::mat4> &matArray) const;

    /**
     * Returns the ID of the given shader in use.
     * @return ID for shader.
     */
    unsigned int getId() const;

  private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, const std::string& type);

    unsigned int ID = {};
};
