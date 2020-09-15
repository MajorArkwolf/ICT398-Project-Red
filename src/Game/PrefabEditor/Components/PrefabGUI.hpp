#pragma once
#include <imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Engine/SubModules/PrefabRepo.hpp"

class PrefabGUI {
public:
    PrefabGUI();

    ~PrefabGUI() = default;

    void Draw();



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
    /// Used to determine what on the list is selected.
    int listbox_item_current_ = 0;

    ImVec2 button_size_ = ImVec2(150, 30);

    void LoadExistingPrefab();

    void MainMenu();

    void CreatePrefab();

    void MainEntityMenu();
};
