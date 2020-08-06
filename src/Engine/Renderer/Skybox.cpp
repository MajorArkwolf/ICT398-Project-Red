#include "Skybox.hpp"
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include "Engine/Engine.hpp"

view::Skybox::~Skybox() {
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);
}

void view::Skybox::Init() {
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    auto basepath = redengine::Engine::get().GetBasePath();

    faces.emplace_back(basepath / "res" / "images" / "skybox" / "right.jpg");
    faces.emplace_back(basepath / "res" / "images" / "skybox" / "left.jpg");
    faces.emplace_back(basepath / "res" / "images" / "skybox" / "top.jpg");
    faces.emplace_back(basepath / "res" / "images" / "skybox" / "bottom.jpg");
    faces.emplace_back(basepath / "res" / "images" / "skybox" / "front.jpg");
    faces.emplace_back(basepath / "res" / "images" / "skybox" / "back.jpg");
    cubemapTexture = loadCubemap(faces);

    auto vs = basepath / "res" / "shader" / "skybox_vert.vs";
    auto fs = basepath / "res" / "shader" / "skybox_frag.fv";
    shader = std::make_unique<Shader>(vs, fs, "");
    shader->Use();
    shader->SetInt("skybox", 0);
}

unsigned int view::Skybox::loadCubemap(const std::vector<std::filesystem::path>& mFaces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < mFaces.size(); i++) {
        unsigned char *data =
            stbi_load(mFaces[i].string().c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                         height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i]
                      << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}  

void view::Skybox::draw(const glm::mat4& view, const glm::mat4& projection) const {
    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
    shader->Use();
    shader->SetMat4("sk_view", view);
    shader->SetMat4("sk_projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void view::Skybox::update() {}