#include "OpenGL.hpp"
#include <iostream>
#include "Engine/Engine.hpp"
#include "stb_image.h"
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION


void view::OpenGL::Draw() {
    auto &engine = redengine::Engine::get();
    if (!WindowMinimized()) {
        camera_ = &engine.game_stack_.getTop()->camera;
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        engine.game_stack_.getTop()->GUIStart();
        int width = 0, height = 0;
        glfwGetWindowSize(engine.window_, &width, &height);
        glm::mat4 projection =
                glm::perspective(glm::radians(camera_->zoom_),
                                 static_cast<double>(width) / static_cast<double>(height), 0.1, 100000.0);
        glm::mat4 view = camera_->GetViewMatrix();
        glm::mat4 skybox_view = glm::mat4(glm::mat3(camera_->GetViewMatrix()));
        engine.game_stack_.getTop()->Display(model_shader_.get(), projection, view);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        sky_box_.draw(skybox_view, projection);
        engine.game_stack_.getTop()->GUIEnd();
    }
    glfwSwapBuffers(engine.window_);
}

void view::OpenGL::Init() {
    int width = 0;
    int height = 0;

    auto &engine = redengine::Engine::get();
    auto base_path = engine.GetBasePath();
    glfwGetWindowSize(engine.window_, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    sky_box_.Init();


    auto model_vert = base_path / "res" / "shader" / "model_shader.vs";
    auto model_frag = base_path / "res" / "shader" / "model_shader.fs";
    model_shader_ = std::make_shared<Shader>(model_vert, model_frag, "");

    auto lighting_vert = base_path / "res" / "shader" / "lighting_shader.vs";
    auto lighting_frag = base_path / "res" / "shader" / "lighting_shader.fs";
    light_shader_ = std::make_shared<Shader>(lighting_vert, lighting_frag, "");

}

void view::OpenGL::DeInit() {

}

void view::OpenGL::DrawModel(std::shared_ptr<Shader> shader, unsigned int &VAO, const std::vector<TextureB> &textures,
                             const std::vector<unsigned int> &indices) {
    // bind appropriate textures
    unsigned int diffuse_nr = 1;
    unsigned int specular_nr = 1;
    unsigned int normal_nr = 1;
    unsigned int height_nr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuse_nr++);
        else if (name == "texture_specular")
            number =
                    std::to_string(specular_nr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normal_nr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(height_nr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader->GetID(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void view::OpenGL::SetupMesh(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
                             std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned int>(vertices.size()) * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(indices.size()) * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(0));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, normal)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, tex_coords)));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, tangent)));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, bitangent)));
    // BoneID's
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex),
                           reinterpret_cast<void *>(offsetof(Vertex, bone_ids)));
    //Bone Weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, bone_weight)));

    glBindVertexArray(0);
}

void view::OpenGL::ResizeWindow() {
    auto &engine = redengine::Engine::get();
    int width = 0, height = 0;
    glfwGetWindowSize(engine.window_, &width, &height);
    engine.SetLastWindowXSize(width);
    engine.SetLastWindowYSize(height);
    UpdateViewPort(0, 0, width, height);
}

unsigned int view::OpenGL::TextureFromFile(const std::string &path, const std::filesystem::path &directory,
                                           [[maybe_unused]] bool gamma) {
    auto new_dir = directory;
    std::filesystem::path filename = new_dir.remove_filename() / path;

    unsigned int texture_id = 0;
    glGenTextures(1, &texture_id);

    int width = 0, height = 0, nrComponents = 0;
    // filename to C string may not work on other OS's please verify it does.
    unsigned char *data =
            stbi_load(filename.string().c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Enable should you need alpha, this will clamp textures to the edge to
        // ensure that weird stuff doesnt happen.
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }
    return texture_id;
}

void view::OpenGL::SetCameraOnRender(engine::Camera &main_camera) {
    camera_ = &main_camera;
}

//void View::OpenGL::sortDrawDistance() {
//    glm::vec3 cpos = {camera->Position.x, static_cast<float>(camera->Position.y), static_cast<float>(camera->Position.z)};
//    for (auto &e : drawQue) {
//        e.distance = glm::distance(e.pos, cpos);
//    }
//    sort(drawQueTransparent.begin(), drawQueTransparent.end(),
//         [](const View::Data::DrawItem& lhs, const View::Data::DrawItem& rhs)->bool {
//             return lhs.distance > rhs.distance;
//         });
//}

void view::OpenGL::ToggleWireFrame() {
    wire_frame_ = !wire_frame_;
}

bool view::OpenGL::WindowMinimized() {
    auto &engine = redengine::Engine::get();
    int width = 0, height = 0;
    glfwGetWindowSize(engine.window_, &width, &height);
    return width == 0 || height == 0;
}

void view::OpenGL::UpdateViewPort(int bl, int br, int tl, int tr) {
    glViewport(bl, br, tl, tr);
}

//void View::OpenGL::AddToQueTransparent(View::Data::DrawItem &drawItem) {
//    drawQueTransparent.push_back(drawItem);
//}

view::OpenGL::~OpenGL() {

}
