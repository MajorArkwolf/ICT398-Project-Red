#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::filesystem::path &vertex,
               const std::filesystem::path &fragment,
               const std::filesystem::path &geo = "") {
  // 1. retrieve the vertex/fragment source code from filePath
  auto vertexCode = std::string();
  auto fragmentCode = std::string();
  auto geometryCode = std::string();
  auto vShaderFile = std::ifstream();
  auto fShaderFile = std::ifstream();
  auto gShaderFile = std::ifstream();
  auto vShaderStream = std::stringstream();
  auto fShaderStream = std::stringstream();

  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    vShaderFile.open(vertex);
    fShaderFile.open(fragment);

    if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
        std::cout << "Failed to open shader files\n";
        std::cout << "Vertix: " << vShaderFile.is_open() << '\n';
        std::cout << "Frag: " << fShaderFile.is_open() << '\n' << '\n';
    }

    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    // if geometry shader path is present, also load a geometry shader
    if (!geo.empty()) {
      gShaderFile.open(geo);
      auto gShaderStream = std::stringstream();
      gShaderStream << gShaderFile.rdbuf();
      gShaderFile.close();
      geometryCode = gShaderStream.str();
    }
  } catch (std::ifstream::failure &e) {
    std::cout << "Vertex Shader: " << vertex << " Fragment Shader: "
              << fragment << "\n";
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
              << "\n";
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // 2. compile shaders
  unsigned int vertexId = 0, fragmentId = 0;

  // vertex shader
  vertexId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexId, 1, &vShaderCode, nullptr);
  glCompileShader(vertexId);
  CheckCompileErrors(vertexId, "VERTEX");

  // fragment Shader
  fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentId, 1, &fShaderCode, nullptr);
  glCompileShader(fragmentId);
  CheckCompileErrors(fragmentId, "FRAGMENT");

  // if geometry shader is given, compile geometry shader
  unsigned int geometry = 0;
  if (!geo.empty()) {
    const char *gShaderCode = geometryCode.c_str();
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, nullptr);
    glCompileShader(geometry);
    CheckCompileErrors(geometry, "GEOMETRY");
  }

  // shader Program
  id_ = glCreateProgram();
  glAttachShader(id_, vertexId);
  glAttachShader(id_, fragmentId);
  if (!geo.empty())
    glAttachShader(id_, geometry);
  glLinkProgram(id_);
  CheckCompileErrors(id_, "PROGRAM");

  // delete the shaders as they're linked into our program now
  // and are no longer necessary
  glDeleteShader(vertexId);
  glDeleteShader(fragmentId);
  if (!geo.empty()) {
    glDeleteShader(geometry);
  }
}

void Shader::Use() const {
  glUseProgram(id_);
}

void Shader::SetBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string &name, float x, float y, float z,
                     float w) const {
  glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::CheckCompileErrors(GLuint shader, const std::string &type) {
  GLint success = false;
  GLchar infoLog[1024] = {};
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- "  // NOLINT
                << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- "  // NOLINT
                << std::endl;
    }
  }
}

unsigned int Shader::GetID() const {
  return id_;
}

void Shader::SetMat4Array(const std::string &name,
                          const std::vector<glm::mat4> &matArray) const {
  if (!matArray.empty()) {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), matArray.size(),
                       GL_FALSE, glm::value_ptr(matArray[0]));
  }
}
