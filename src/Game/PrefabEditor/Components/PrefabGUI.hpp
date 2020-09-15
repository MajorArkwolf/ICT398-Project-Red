#pragma once
#include <imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Engine/SubModules/PrefabRepo.hpp"

class Shader;

class PrefabGUI {
public:
    PrefabGUI();

    ~PrefabGUI() = default;

    void Draw(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view);



private:
    redengine::prefab prefab_loaded;
    ///Main menu will give the option to edit or create a new prefab
    bool main_menu_ = true;
    /// Begin creating a new prefab
    bool create_new_prefab_ = false;
    /// This menu will let you get a prefab that is already loaded in and edit it.
    bool get_prefab_ = false;
    /// main menu that will display all the options you can edit.
    bool main_edit_menu_ = false;

    //------Component Menu's-------
    bool model_component_ = false;
    bool transform_component_ = false;

    ImVec2 button_size_ = ImVec2(150, 30);

    void LoadExistingPrefab();

    void MainMenu();

    void CreatePrefab();

    void MainEntityMenu();

    void ModelComponentMenu();

    void TransformComponentMenu();
};
