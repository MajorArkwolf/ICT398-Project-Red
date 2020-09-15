#include "PrefabGUI.hpp"
#include "Engine/Engine.hpp"

namespace ImGui
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };

    bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return Combo(label, currIndex, vector_getter,
                     static_cast<void*>(&values), values.size());
    }

    bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ListBox(label, currIndex, vector_getter,
                       static_cast<void*>(&values), values.size());
    }

}

PrefabGUI::PrefabGUI() {

}

void PrefabGUI::Draw() {
    main_menu_ ? MainMenu() : (void)0;
    create_new_prefab_ ? CreatePrefab() : (void)0;
    main_edit_menu_ ? MainEntityMenu() : (void)0;
    get_prefab_ ? LoadExistingPrefab() : (void)0;
}

void PrefabGUI::MainMenu() {
    ImGui::SetNextWindowPos(ImVec2(0.5, 0.5), ImGuiCond_Always, ImVec2(-0.5, -0.5));
    ImGui::SetNextWindowSize(ImVec2(250, 500), 1);
    ImGui::Begin("Main Menu", &main_menu_,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button("New Prefab", button_size_)) {
        main_menu_ = false;
        create_new_prefab_ = true;
    }
    if (ImGui::Button("Loaded Prefab", button_size_)) {
        get_prefab_ = true;
        main_menu_ = false;
    }
    ImGui::End();
}

void PrefabGUI::CreatePrefab() {
    static char prefab_name[50] = {'\0'};
    ImGui::SetNextWindowPos(ImVec2(0.5, 0.5), ImGuiCond_Always, ImVec2(-0.5, -0.5));
    ImGui::SetNextWindowSize(ImVec2(250, 500), 1);
    ImGui::Begin("Create New Prefab", &create_new_prefab_,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::InputText("Prefab Name", prefab_name, IM_ARRAYSIZE(prefab_name));
    if (ImGui::Button("Submit", button_size_)) {
        main_edit_menu_ = true;
        create_new_prefab_ = false;
        prefab_loaded = redengine::prefab();
        prefab_loaded.name = std::string(prefab_name);
    }
    if (ImGui::Button("Exit", button_size_)) {
        main_menu_ = true;
        create_new_prefab_ = false;
    }
    ImGui::End();
}

void PrefabGUI::LoadExistingPrefab() {

    auto list = redengine::Engine::get().GetPrefabRepo().GetPrefabList();
    ImGui::SetNextWindowPos(ImVec2(0.5, 0.5), ImGuiCond_Always, ImVec2(-0.5, -0.5));
    ImGui::SetNextWindowSize(ImVec2(250, 500), 1);
    ImGui::Begin("Load Prefab", &get_prefab_,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::ListBox("Listbox\n(single select)", &listbox_item_current_, list);

    if (ImGui::Button("Get Entity", button_size_)) {
        get_prefab_ = false;
        main_edit_menu_ = true;
        prefab_loaded = redengine::Engine::get().GetPrefabRepo().GetPrefabMut(list.at(
                static_cast<unsigned long>(listbox_item_current_)));
    }
    if (ImGui::Button("Exit", button_size_)) {
        main_menu_ = true;
        get_prefab_ = false;
    }
    ImGui::End();
}

void PrefabGUI::MainEntityMenu() {
    ImGui::SetNextWindowPos(ImVec2(0.5, 0.5), ImGuiCond_Always, ImVec2(-0.5, -0.5));
    ImGui::SetNextWindowSize(ImVec2(250, 500), 1);
    ImGui::Begin("Load Prefab", &main_edit_menu_,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Editing: %s", prefab_loaded.name.c_str());
    if (ImGui::Button("Save and Submit", button_size_)) {
        main_menu_ = true;
        main_edit_menu_ = false;
    }
    if (ImGui::Button("Close, Dont Save", button_size_)) {
        main_menu_ = true;
        main_edit_menu_ = false;
    }
    ImGui::End();
}
