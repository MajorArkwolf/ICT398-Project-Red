#pragma once
#include <imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Engine/SubModules/PrefabRepo.hpp"

class Shader;

class PrefabGUI {
public:
    /**
     * Default Constructor
     */
    PrefabGUI();
    /**
     * Default Destructor
     */
    ~PrefabGUI() = default;

    /**
     * Draw call to draw the models in the scene.
     * @param shader program used to draw the models.
     * @param projection the projection matrix from the camera.
     * @param view view matrix of the camera.
     */
    void Draw(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view);

    /**
     * Returns the currently loaded prefab
     * @return prefab loaded
     */
    redengine::prefab GetPrefab() const;

private:
    /// The prefab that will be edited.
    redengine::prefab prefab_loaded_;
    /// Main menu will give the option to edit or create a new prefab
    bool main_menu_ = true;
    /// Begin creating a new prefab
    bool create_new_prefab_ = false;
    /// This menu will let you get a prefab that is already loaded in and edit it.
    bool get_prefab_ = false;
    /// main menu that will display all the options you can edit.
    bool main_edit_menu_ = false;
    /// physics menu that will let you add collidors.


    //------Component Menu's-------
    bool model_component_ = false;
    bool transform_component_ = false;
    bool physics_edit_menu = false;
    bool affordance_edit_menu = false;

    ImVec2 button_size_ = ImVec2(150, 30);

    /**
     * Load existing prefab menu
     */
    void LoadExistingPrefab();

    /**
     * Main Menu for loading prefabs
     */
    void MainMenu();

    /**
     * Create a new prefab
     */
    void CreatePrefab();

    /**
     * Main Menu of prefab entity
     */
    void MainEntityMenu();

    /**
     * Model menu
     */
    void ModelComponentMenu();

    /**
     * Transform Menu
     */
    void TransformComponentMenu();

    /**
     * Physics Main Menu
     */
    void PhysicsMainMenu();

    /**
     * Affordance Menu
     */
    void AffordanceMenu();
};
