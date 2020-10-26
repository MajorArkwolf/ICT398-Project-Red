#include "NPCDemo.hpp"

#include "DataStructures/Model/Overload.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Board.hpp"
#include "ECS/Component/Model.hpp"
#include "ECS/Component/Player.hpp"
#include "Engine/Engine.hpp"
#include "Engine/SubModules/JsonLoader.hpp"
#include "ECS/System/NPC/Core.hpp"
#include "ECS/System/NPC/Tools.hpp"

static inline void ToggleRenderer(physics::PhysicsWorld &pe, bool val) {
    // Only toggle if the provided value is different
    if (pe.GetRendererStatus() != val) {
        // Toggle the renderer
        pe.ToggleRenderer();
    }
}

NPCDemo::NPCDemo() {
    // Setup the Physics
    physics_world_.SetECS(&ecs_);

    // Setup the camera
    camera = engine::Camera();
    camera.position_ = glm::vec3(0.0f, 10.0f, 0.0f);

    // Setup the input management
    relativeMouse = true;
    auto &engine = redengine::Engine::get();
    engine.GetMouseMode(relativeMouse);

    // Setup the pathfinding
    auto &board = ecs_.CreateEntity();
    auto &board_component = board.AddComponent<component::Board>(&ecs_, &physics_world_, glm::vec3(-505.0f, 79.f, 305.0f), 34, 22, 2.5f);

    // Load the scene and objects from file
    std::filesystem::path path = "";
    path.append("NPCDemo");
    path.append("Scene.json");
    JSONLoader::LoadScene(path, &ecs_, &physics_world_);

    // Initialize the NPC ECS component dependencies and systems
    System::NPCSystemInit(ecs_.GetRegistry());
}

void NPCDemo::Init() {
    // Create and configure the player entity for the 'big-world'
    auto big_player = ecs_.CreateEntity();
    big_player.AddComponent<component::Player>();
    auto filepath = redengine::Engine::get().GetBasePath() / "res" / "model" / "Character" / "ClothedMan.gltf";
    auto &b_model = big_player.AddComponent<component::Model>(filepath);
    b_model.draw_model = false;
    auto &trans = big_player.AddComponent<component::Transform>();
    auto &playerComp = big_player.GetComponent<component::Player>();
    playerComp.height_ = 0.0f;
    playerComp.camera_height_offset_ = 170.0f;
    trans.pos = glm::vec3{0.f, playerComp.height_, 0.f};
    trans.scale = {22.f, 22.f, 22.f};
    big_player.AddComponent<component::PhysicBody>();
    auto &phys = big_player.GetComponent<component::PhysicBody>();
    phys.is_player = true;
    physics_world_.AddCollisionBody(big_player.GetID(), playerComp.camera_.position_, glm::quat(glm::vec3(0, 0, 0)));
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    auto playerShape = physics_engine.CreateCapsuleShape(50, 90);
    physics_world_.AddCollider(big_player.GetID(), playerShape, {0.f, 90.f, 0.f}, {1.0f, 0.f, 0.f, 0.f});
    playerComp.camera_.movement_speed_ = 0.15f;
    playerComp.camera_.position_ = glm::vec3{-465.0f, playerComp.height_ + playerComp.camera_height_offset_, 330.0f};
    auto &anim =
        big_player.AddComponent<component::Animation>(
            big_player.GetComponent<component::Model>().id_);
    auto idle = "IDLE";
    anim.animator_.LoadAnimation(idle);
    player_.SetBigPlayer(big_player);

    // Create and configure the player entity for the 'little-world'
    auto little_player = ecs_.CreateEntity();
    auto &little_p = little_player.AddComponent<component::Player>();
    auto &little_model = little_player.AddComponent<component::Model>(filepath);
    auto &little_tran = little_player.AddComponent<component::Transform>();
    little_p.height_ = 80.0f;
    little_p.camera_height_offset_ = 2.8f;
    little_tran.pos = glm::vec3{-465.0f, little_p.height_, 330.0f};
    little_tran.scale = glm::vec3{0.5f, 0.5f, 0.5f};
    little_p.camera_.position_ = glm::vec3{-465.0f, little_p.height_ + little_p.camera_height_offset_, 330.0f};
    little_p.camera_.movement_speed_ = 0.01f;
    auto &little_anim =
        little_player.AddComponent<component::Animation>(
            little_player.GetComponent<component::Model>().id_);
    little_anim.animator_.LoadAnimation(idle);
    auto &phys_little = little_player.AddComponent<component::PhysicBody>();
    phys_little.is_player = true;
    physics_world_.AddCollisionBody(little_player.GetID(), playerComp.camera_.position_, glm::quat(glm::vec3(0, 0, 0)));
    auto playerShapeLittle = physics_engine.CreateCapsuleShape(1, 3);
    physics_world_.AddCollider(little_player.GetID(), playerShapeLittle, {0.f, 1.7f, 0.f}, {1.0f, 0.f, 0.f, 0.f});
    player_.SetLittlePlayer(little_player);



    // Find the Identifiers of the Entities with the NPC personality hook component and store them
    auto npc_hook_view = ecs_.GetRegistry().view<component::NPCPersonalityID>();
    auto &reg = ecs_.GetRegistry();
    std::set<entt::entity> id_npcs_tom;
    std::set<entt::entity> id_npcs_alistair;
    std::set<entt::entity> id_npcs_jessica;
    std::set<entt::entity> id_npcs_kiera;
    for (auto &npc_hook_id: npc_hook_view) {
        // Catch the type
        auto npc_entity = ecs_.GetEntity(npc_hook_id);
        switch (npc_entity->GetComponent<component::NPCPersonalityID>().ID) {
            case 0:
                // Track the entity's identifier
                id_npcs_tom.insert(npc_entity->GetID());
                break;
            case 1:
                // Track the entity's identifier
                id_npcs_alistair.insert(npc_entity->GetID());
                break;
            case 2:
                // Track the entity's identifier
                id_npcs_jessica.insert(npc_entity->GetID());
                break;
            case 3:
                // Track the entity's identifier
                id_npcs_kiera.insert(npc_entity->GetID());
                break;
            default:
                assert(false);
                break;
        }
    }

    // Find the Identifiers of the Entities with the Type hook component and store them
    auto interactive_hook_view = ecs_.GetRegistry().view<component::InteractableObject>();
    std::set<entt::entity> id_interactables_bench;
    std::set<entt::entity> id_interactables_book;
    std::set<entt::entity> id_interactables_rock;
    std::set<entt::entity> id_interactables_tree;
    for (auto &interactive_hook_id: interactive_hook_view) {
        // Catch the type
        auto interactive_entity = ecs_.GetEntity(interactive_hook_id);
        switch (interactive_entity->GetComponent<component::InteractableObject>().type) {
            case component::InteractableObject::Type::bench:
                // Track the entity's identifier
                id_interactables_bench.insert(interactive_hook_id);
                break;
            case component::InteractableObject::Type::book:
                // Track the entity's identifier
                id_interactables_book.insert(interactive_hook_id);
                break;
            case component::InteractableObject::Type::rock:
                // Track the entity's identifier
                id_interactables_rock.insert(interactive_hook_id);
                break;
            case component::InteractableObject::Type::tree:
                // Track the entity's identifier
                id_interactables_tree.insert(interactive_hook_id);
                break;
            case component::InteractableObject::Type::npc:
                break;
            default:
                assert(false);
                break;
        }
    }

    // Iterate through all of the Entity's that have a NPCPersonalityID hook component
    for (auto &npc_hook_id: npc_hook_view) {
        // Generate and gather the NPC's BDI component
        auto &npc_bdi = reg.emplace_or_replace<component::BDI>(npc_hook_id);

        // Configure the NPC's generic property beliefs about itself
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kExists);
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kSize);
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kMass);
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kPosition);
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kVelocity);
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kOrientation);
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kRange);
        npc_bdi.beliefs_properties[npc_hook_id].insert(npc::Properties::kType);

        // Configure the NPC's generic affordance beliefs about itself
        npc_bdi.beliefs_affordances[npc_hook_id].insert(npc::Actions::kObserve);
        npc_bdi.beliefs_affordances[npc_hook_id].insert(npc::Actions::kTraverse);

        // Iterate through all of the known "Tom" NPCs
        for (auto &tom_identifier: id_npcs_tom) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kSize);
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[tom_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[tom_identifier].insert(npc::Actions::kPush);
            npc_bdi.beliefs_affordances[tom_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[tom_identifier].insert(npc::Actions::kUse);
            npc_bdi.beliefs_affordances[tom_identifier].insert(npc::Actions::kTraverse);
        }

        // Iterate through all of the known "Alistair" NPCs
        for (auto &alistair_identifier: id_npcs_alistair) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kSize);
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[alistair_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[alistair_identifier].insert(npc::Actions::kPush);
            npc_bdi.beliefs_affordances[alistair_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[alistair_identifier].insert(npc::Actions::kUse);
            npc_bdi.beliefs_affordances[alistair_identifier].insert(npc::Actions::kTraverse);
        }

        // Iterate through all of the known "Jessica" NPCs
        for (auto &jessica_identifier: id_npcs_jessica) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kSize);
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[jessica_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[jessica_identifier].insert(npc::Actions::kPush);
            npc_bdi.beliefs_affordances[jessica_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[jessica_identifier].insert(npc::Actions::kUse);
            npc_bdi.beliefs_affordances[jessica_identifier].insert(npc::Actions::kTraverse);
        }

        // Iterate through all of the known "Kiera" NPCs
        for (auto &kiera_identifier: id_npcs_kiera) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kSize);
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[kiera_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[kiera_identifier].insert(npc::Actions::kPush);
            npc_bdi.beliefs_affordances[kiera_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[kiera_identifier].insert(npc::Actions::kUse);
            npc_bdi.beliefs_affordances[kiera_identifier].insert(npc::Actions::kTraverse);
        }

        // Iterate through all of the known book entities
        for (auto &book_identifier: id_interactables_book) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kShape);
            npc_bdi.beliefs_properties[book_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[book_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[book_identifier].insert(npc::Actions::kTraverse);
            npc_bdi.beliefs_affordances[book_identifier].insert(npc::Actions::kUse);
            npc_bdi.beliefs_affordances[book_identifier].insert(npc::Actions::kPush);
            npc_bdi.beliefs_affordances[book_identifier].insert(npc::Actions::kGrab);
            npc_bdi.beliefs_affordances[book_identifier].insert(npc::Actions::kDrop);
        }

        // Iterate through all of the known tree entities
        for (auto &tree_identifier: id_interactables_tree) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kShape);
            npc_bdi.beliefs_properties[tree_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[tree_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[tree_identifier].insert(npc::Actions::kTraverse);
        }

        // Iterate through all of the known bench entities
        for (auto &bench_identifier: id_interactables_bench) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kShape);
            npc_bdi.beliefs_properties[bench_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[bench_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[bench_identifier].insert(npc::Actions::kTraverse);
            npc_bdi.beliefs_affordances[bench_identifier].insert(npc::Actions::kSit);
        }

        // Iterate through all of the known rock entities
        for (auto &rock_identifier: id_interactables_rock) {
            // Configure the NPC's generic property beliefs about the entity at the current identifier
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kExists);
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kPosition);
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kOrientation);
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kVelocity);
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kMass);
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kRange);
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kShape);
            npc_bdi.beliefs_properties[rock_identifier].insert(npc::Properties::kType);

            // Configure the NPC's generic affordance beliefs about the entity at the current identifier
            npc_bdi.beliefs_affordances[rock_identifier].insert(npc::Actions::kObserve);
            npc_bdi.beliefs_affordances[rock_identifier].insert(npc::Actions::kTraverse);
            npc_bdi.beliefs_affordances[rock_identifier].insert(npc::Actions::kSit);
            npc_bdi.beliefs_affordances[rock_identifier].insert(npc::Actions::kPush);
        }

        // Get the other NPC component data structures
        auto &npc_characteristics = reg.emplace_or_replace<component::Characteristics>(npc_hook_id);
        auto &npc_behavior_state = reg.emplace_or_replace<component::BehaviourState>(npc_hook_id);

        // Configure unique properties for the NPC
        switch (reg.get<component::NPCPersonalityID>(npc_hook_id).ID) {
                // "Tom"-specific configuration
            case 0: {
                // "Tom"-specific BDI Desires
                npc_bdi.desires[1].parent = 0;
                npc_bdi.desires[1].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[1].children.insert(2);
                npc_bdi.desires[1].goals.emplace_back(
                    component::Goal(
                        *id_npcs_alistair.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[2].parent = 1;
                npc_bdi.desires[2].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[2].goals.emplace_back(
                    component::Goal(
                        *id_npcs_alistair.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));

                // "Tom"-specific BDI Root Desires
                npc_bdi.root_desires.insert(1);

                // "Tom"-specific BDI Intentions
                npc_bdi.intentions[1].emplace_back(
                    component::Plan(
                        npc::Actions::kUse,
                        *id_npcs_alistair.begin(),
                        -1, {0}));
                npc_bdi.intentions[2].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_npcs_alistair.begin(),
                        1, {1}));

                // "Tom"-specific Characteristics
                npc_characteristics.mood = 0.85f;
                npc_characteristics.traits[npc::Properties::kRange] = -0.1;
                npc_characteristics.personality[npc::Actions::kSit] = 0.3;
                npc_characteristics.personality[npc::Actions::kTraverse] = 0.1;

                // "Tom"-specific BehaviourState
                npc_behavior_state.current = npc::Stages::kObserve;
                npc_behavior_state.current_intention.first = -1;
                npc_behavior_state.current_intention.second = -1;
                break;
            }
                // "Alistair"-specific configuration
            case 1: {
                // "Alistair"-specific BDI Desires
                npc_bdi.desires[1].parent = 0;
                npc_bdi.desires[1].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[1].children.insert(4);
                npc_bdi.desires[1].goals.emplace_back(
                    component::Goal(
                        *id_npcs_tom.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));
                npc_bdi.desires[1].goals.emplace_back(
                    component::Goal(
                        *id_interactables_rock.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[2].parent = 0;
                npc_bdi.desires[2].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[2].children.insert(5);
                npc_bdi.desires[2].goals.emplace_back(
                    component::Goal(
                        *id_npcs_tom.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));
                npc_bdi.desires[2].goals.emplace_back(
                    component::Goal(
                        *id_interactables_tree.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[3].parent = 0;
                npc_bdi.desires[3].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[3].children.insert(6);
                npc_bdi.desires[3].goals.emplace_back(
                    component::Goal(
                        *id_npcs_tom.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));
                npc_bdi.desires[3].goals.emplace_back(
                    component::Goal(
                        *id_interactables_bench.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[4].parent = 1;
                npc_bdi.desires[4].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[4].goals.emplace_back(
                    component::Goal(
                        *id_interactables_rock.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));
                npc_bdi.desires[5].parent = 2;
                npc_bdi.desires[5].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[5].goals.emplace_back(
                    component::Goal(
                        *id_interactables_tree.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));
                npc_bdi.desires[6].parent = 3;
                npc_bdi.desires[6].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[6].goals.emplace_back(
                    component::Goal(
                        *id_interactables_bench.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));

                // "Alistair"-specific BDI Root Desires
                npc_bdi.root_desires.insert(1);
                npc_bdi.root_desires.insert(2);
                npc_bdi.root_desires.insert(3);

                // "Alistair"-specific BDI Intentions
                npc_bdi.intentions[1].emplace_back(
                    component::Plan(
                        npc::Actions::kSit,
                        *id_interactables_rock.begin(),
                        -1, {0}));
                npc_bdi.intentions[2].emplace_back(
                    component::Plan(
                        npc::Actions::kSit,
                        *id_interactables_tree.begin(),
                        -1, {0}));
                npc_bdi.intentions[3].emplace_back(
                    component::Plan(
                        npc::Actions::kSit,
                        *id_interactables_bench.begin(),
                        -1, {0}));
                npc_bdi.intentions[4].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_interactables_rock.begin(),
                        -1, {1}));
                npc_bdi.intentions[5].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_interactables_tree.begin(),
                        -1, {2}));
                npc_bdi.intentions[6].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_interactables_bench.begin(),
                        -1, {3}));

                // "Alistair"-specific Characteristics
                npc_characteristics.mood = 0.65f;
                npc_characteristics.traits[npc::Properties::kRange] = -0.15;
                npc_characteristics.personality[npc::Actions::kSit] = 0.3;
                npc_characteristics.personality[npc::Actions::kTraverse] = 0.1;

                // "Alistair"-specific BehaviourState
                npc_behavior_state.current = npc::Stages::kObserve;
                npc_behavior_state.current_intention.first = -1;
                npc_behavior_state.current_intention.second = -1;
                break;
            }
                // "Jessica"-specific configuration
            case 2: {
                // "Jessica"-specific BDI Desires
                npc_bdi.desires[1].parent = 0;
                npc_bdi.desires[1].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[1].children.insert(3);
                npc_bdi.desires[1].children.insert(4);
                npc_bdi.desires[1].children.insert(5);
                npc_bdi.desires[1].children.insert(6);
                npc_bdi.desires[1].goals.emplace_back(
                    component::Goal(
                        *id_npcs_alistair.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[2].parent = 1;
                npc_bdi.desires[2].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[2].goals.emplace_back(
                    component::Goal(
                        *id_npcs_alistair.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kNotInRange));
                npc_bdi.desires[3].parent = 1;
                npc_bdi.desires[3].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[3].children.insert(2);
                npc_bdi.desires[3].goals.emplace_back(
                    component::Goal(
                        *id_interactables_book.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[4].parent = 1;
                npc_bdi.desires[4].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[4].children.insert(2);
                npc_bdi.desires[4].goals.emplace_back(
                    component::Goal(
                        *id_interactables_rock.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[5].parent = 1;
                npc_bdi.desires[5].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[5].children.insert(2);
                npc_bdi.desires[5].goals.emplace_back(
                    component::Goal(
                        *id_interactables_tree.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[6].parent = 1;
                npc_bdi.desires[6].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[6].children.insert(2);
                npc_bdi.desires[6].goals.emplace_back(
                    component::Goal(
                        *id_interactables_bench.begin(),
                        npc::Properties::kRange,
                        npc::Components::kDefault,
                        0.99f, 1.01f,
                        npc::Conditions::kInRange));
                npc_bdi.desires[7].parent = 0;
                npc_bdi.desires[7].history = npc::Outcomes::kUnknown;
                npc_bdi.desires[7].children.insert(0);
                npc_bdi.desires[7].goals.emplace_back(
                    component::Goal(
                        *id_interactables_bench.begin(),
                        npc::Properties::kVelocity,
                        npc::Components::kDefault,
                        0.01f, 0.011f,
                        npc::Conditions::kBelowRange));

                // "Jessica"-specific BDI Root Desires
                npc_bdi.root_desires.insert(1);
                npc_bdi.root_desires.insert(7);

                // "Jessica"-specific BDI Intentions
                npc_bdi.intentions[1].emplace_back(
                    component::Plan(
                        npc::Actions::kSit,
                        *id_npcs_alistair.begin(),
                        -1, {0}));
                npc_bdi.intentions[2].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_npcs_alistair.begin(),
                        1, {0}));
                npc_bdi.intentions[2].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_interactables_book.begin(),
                        1, {0}));
                npc_bdi.intentions[2].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_interactables_rock.begin(),
                        1, {0}));
                npc_bdi.intentions[2].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_interactables_tree.begin(),
                        1, {0}));
                npc_bdi.intentions[2].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_interactables_bench.begin(),
                        1, {0}));
                npc_bdi.intentions[3].emplace_back(
                    component::Plan(
                        npc::Actions::kUse,
                        *id_interactables_book.begin(),
                        1, {0}));
                npc_bdi.intentions[3].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_npcs_alistair.begin(),
                        1, {0}));
                npc_bdi.intentions[4].emplace_back(
                    component::Plan(
                        npc::Actions::kUse,
                        *id_interactables_rock.begin(),
                        1, {0}));
                npc_bdi.intentions[4].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_npcs_alistair.begin(),
                        1, {0}));
                npc_bdi.intentions[5].emplace_back(
                    component::Plan(
                        npc::Actions::kUse,
                        *id_interactables_tree.begin(),
                        1, {0}));
                npc_bdi.intentions[5].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_npcs_alistair.begin(),
                        1, {0}));
                npc_bdi.intentions[6].emplace_back(
                    component::Plan(
                        npc::Actions::kUse,
                        *id_interactables_bench.begin(),
                        1, {0}));
                npc_bdi.intentions[6].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_npcs_alistair.begin(),
                        1, {0}));
                npc_bdi.intentions[7].emplace_back(
                    component::Plan(
                        npc::Actions::kTraverse,
                        *id_npcs_alistair.begin(),
                        1, {0}));

                // "Jessica"-specific Characteristics
                npc_characteristics.mood = 0.4f;
                npc_characteristics.traits[npc::Properties::kRange] = -0.1;
                npc_characteristics.personality[npc::Actions::kUse] = 0.45;
                npc_characteristics.personality[npc::Actions::kTraverse] = -0.2;

                // "Jessica"-specific BehaviourState
                npc_behavior_state.current = npc::Stages::kObserve;
                npc_behavior_state.current_intention.first = -1;
                npc_behavior_state.current_intention.second = -1;
                break;
            }
                // "Kiera"-specific configuration
            case 3: {
                // Scrapped - not enough time
                break;
            }
            default: {
                // Throw an error in debug mode
                assert(false);
                break;
            }
        }
    }
}

void NPCDemo::UnInit() {
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.ClearCamera();
}

void NPCDemo::Display(Shader *shader, const glm::mat4 &projection, const glm::mat4 &view) {
    auto &renderer = redengine::Engine::get().renderer_;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    ToggleRenderer(physics_world_, gui_manager.renderer_);
    renderer.SetCameraOnRender(player_.GetActiveCamera());
    ecs_.Draw(shader, projection, view);
}

void NPCDemo::GUIStart() {
    auto &engine = redengine::Engine::get();
    GUIManager::startWindowFrame();
    engine.GetGuiManager().DisplayEscapeMenu();
    engine.GetGuiManager().DisplayConsoleLog();
    engine.GetGuiManager().DisplayDevScreen(player_.GetActiveCamera());
    engine.GetGuiManager().DisplayInputRebindWindow();
    engine.GetGuiManager().DisplayQuitScreen();
    engine.GetGuiManager().DisplayAI(player_.GetActivePlayer().GetComponent<component::Player>().selected_entity, ecs_.GetRegistry());
}

void NPCDemo::GUIEnd() {
    GUIManager::EndWindowFrame();
}

void NPCDemo::Update(double t, double dt) {
    auto &renderer = redengine::Engine::get().renderer_;
    renderer.SetCameraOnRender(player_.GetActiveCamera());
    ecs_.Update(t, dt);
    camera.ProcessKeyboardInput(forward_, backward_, left_, right_, dt);
    player_.ProcessKeyboardInput(forward_, backward_, left_, right_, dt);
    player_.Update(t, dt);
    System::NPCsUpdate(ecs_.GetRegistry(), t, dt);
    //TODO: fix this to use just the phyiscs world instead.
    auto &physics_engine = redengine::Engine::get().GetPhysicsEngine();
    physics_engine.Update(t, dt);
}

void NPCDemo::FixedUpdate(double t, double dt) {
    player_.ProcessKeyboardInput(forward_, backward_, left_, right_, dt);
    player_.Update(t, dt);
    ecs_.FixedUpdate(t, dt);
}

void NPCDemo::HandleInputData(input::InputEvent inputData, double deltaTime) {
    using namespace input;
    auto &engine = redengine::Engine::get();
    auto &gui_manager = engine.GetGuiManager();
    auto handledMouse = false;
    std::visit(overload{
                   [&](std::monostate) {

                   },
                   [&](InputEvent::MouseEvent mouse) {

                   },
                   [&](InputEvent::KeyboardEvent keyboard) {
                       switch (inputData.type) {
                           case input::InputType::kKeyPressed: {
                               switch (keyboard.key) {
                                   case input::VirtualKey::W: {
                                       forward_ = true;
                                   } break;
                                   case input::VirtualKey::A: {
                                       left_ = true;
                                   } break;
                                   case input::VirtualKey::S: {
                                       backward_ = true;
                                   } break;
                                   case input::VirtualKey::D: {
                                       right_ = true;
                                   } break;
                                   case input::VirtualKey::X: {
                                       gui_manager.ToggleWindow("quitScreen");
                                   } break;
                               }
                           } break;
                           case input::InputType::kKeyReleased: {
                               switch (keyboard.key) {
                                   case input::VirtualKey::W: {
                                       forward_ = false;
                                   } break;
                                   case input::VirtualKey::A: {
                                       left_ = false;
                                   } break;
                                   case input::VirtualKey::S: {
                                       backward_ = false;
                                   } break;
                                   case input::VirtualKey::D: {
                                       right_ = false;
                                   } break;
                                   case input::VirtualKey::E: {
                                       player_.TogglePlayer();
                                   } break;
                                   case input::VirtualKey::R: {
                                       auto &player = player_.GetActivePlayer().GetComponent<component::Player>();
                                       player_.GetActivePlayer().GetComponent<component::Player>().selected_entity = engine.GetPhysicsEngine().RayCastSingle(player.camera_.position_, player.camera_.front_, 50.0f);
                                   } break;
                                   case input::VirtualKey::Q: {
                                       auto ent = ecs_.GetRegistry().view<component::Board>();
                                       for (auto &e : ent) {
                                           ecs_.GetRegistry().get<component::Board>(e).ToggleRenderer();
                                       }
                                   } break;
                                   case input::VirtualKey::kEscape:
                                       gui_manager.ToggleWindow("escapeMenu");
                               }
                               break;
                               default:
                                   break;
                           } break;
                       }
                   },
                   [&](InputEvent::dVector2 vec) {

                   },
                   [&](InputEvent::iVector2 vec) {
                       switch (inputData.type) {
                           case input::InputType::kCursorMoved: {
                               static int prev_x = 0;
                               static int prev_y = 0;

                               auto x = static_cast<double>(vec.x);
                               auto y = static_cast<double>(vec.y);
                               x = x * -1.0;
                               player_.GetActiveCamera().ProcessMouseMovement(prev_x - x, prev_y - y);
                               handledMouse = true;
                               prev_x = x;
                               prev_y = y;
                           } break;
                           case input::InputType::kMouseScrolled: {
                               double amountScrolledY = static_cast<double>(vec.y);
                               player_.GetActiveCamera().ProcessMouseScroll(amountScrolledY);
                           } break;
                           default: {

                           };
                               break;
                       }
                   }},
               inputData.data);
    if (!handledMouse) {
        engine.mouse_ = {0.0f, 0.0f};
    }
}
