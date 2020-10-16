#include "Core.hpp"

#include "ECS/Component/NPC.hpp"

namespace System {

const static int EMOTION_QUEUE_TURNOVER_RATE = 1.0f / 240.0f;

const static float MOOD_INTENSITY_REDUCTION_RATE = 1.0f / 360.0f;

void NPCInit(entt::registry& registry, entt::entity& entity) {
    // Regenerate default values for all NPC components
    registry.emplace_or_replace<component::BDI>(entity);

    // The following lines are not required as EnTT dependencies should manage the rest.
    //registry.emplace_or_replace<component::Characteristics>(entity);
    //registry.emplace_or_replace<component::BehaviourState>(entity);
}

void NPCUnInit(entt::registry& registry, entt::entity& entity) {
    // Delete any of the Entity's NPC components that exist
    registry.remove_if_exists<component::BDI>(entity);

    // The following lines are not required as EnTT dependencies should manage the rest.
    //registry.remove_if_exists<component::Characteristics>(entity);
    //registry.remove_if_exists<component::BehaviourState>(entity);
}

void NPCImport(entt::registry& registry, entt::entity& entity, std::string path) {

}

void NPCExport(entt::registry& registry, entt::entity& entity, std::string path) {

}

void NPCObserve(entt::registry& registry, entt::entity& entity) {

}

void NPCPrepare(entt::registry& registry, entt::entity& entity) {

}

void NPCRespond(entt::registry& registry, entt::entity& entity) {

}

void NPCIdle(entt::registry& registry, entt::entity& entity) {

}

void NPCsFixedUpdate(entt::registry& registry, double t, double dt) {
    // Loop through the NPCs that have all of the required data structures
    auto npc_view = registry.view<component::BDI, component::Characteristics, component::BehaviourState>();
    for (auto npc_entity: npc_view) {
        // Get the NPC's BehaviourState and update the current behaviour's step counters
        auto npc_state = npc_view.get<component::BehaviourState>(npc_entity);
        npc_state.steps_current++;
        npc_state.steps_accumulated++;

        // Get the NPC's characteristics and gradually reduce the intensity of its mood
        auto npc_characteristics = npc_view.get<component::Characteristics>(npc_entity);
        if (npc_characteristics.mood < 0.0f) {
            npc_characteristics.mood += MOOD_INTENSITY_REDUCTION_RATE * dt;
        }
        else {
            npc_characteristics.mood -= MOOD_INTENSITY_REDUCTION_RATE * dt;
        }

        // Gradually reduce the contents of the NPC's emotional queue
        if (npc_state.steps_accumulated * dt * EMOTION_QUEUE_TURNOVER_RATE >= 1.0f) {
            // Avoid popping the emotion queue if it is empty
            if (!npc_characteristics.emotions.empty()) {
                // Pop the oldest emotion from the NPC's emotional queue
                npc_characteristics.emotions.pop();
            }
        }

        // Check the NPC's current state and call the appropriate behaviour state manager
        switch (npc_state.current) {
            case npc::Stages::kPrepare:
                NPCPrepare(registry, npc_entity);
                break;
            case npc::Stages::kObserve:
                NPCObserve(registry, npc_entity);
                break;
            case npc::Stages::kRespond:
                NPCRespond(registry, npc_entity);
                break;
            case npc::Stages::kIdle:
                NPCIdle(registry, npc_entity);
                break;
            default:;
                // Do nothing
        }
    }
}

void NPCSystemInit(entt::registry& registry) {
    // Set the NPC construction dependencies to ensure that all NPC components are generated simultaneously
    registry.on_construct<component::BDI>().connect<&entt::registry::emplace_or_replace<component::Characteristics>>();
    registry.on_construct<component::BDI>().connect<&entt::registry::emplace_or_replace<component::BehaviourState>>();
    registry.on_construct<component::Characteristics>().connect<&entt::registry::emplace_or_replace<component::BDI>>();
    registry.on_construct<component::Characteristics>().connect<&entt::registry::emplace_or_replace<component::BehaviourState>>();
    registry.on_construct<component::BehaviourState>().connect<&entt::registry::emplace_or_replace<component::Characteristics>>();
    registry.on_construct<component::BehaviourState>().connect<&entt::registry::emplace_or_replace<component::BDI>>();

    // Set the NPC destruction dependencies to ensure that all NPC components are deleted simultaneously
    registry.on_destroy<component::BDI>().connect<&entt::registry::remove_if_exists<component::Characteristics>>();
    registry.on_destroy<component::BDI>().connect<&entt::registry::remove_if_exists<component::BehaviourState>>();
    registry.on_destroy<component::Characteristics>().connect<&entt::registry::remove_if_exists<component::BDI>>();
    registry.on_destroy<component::Characteristics>().connect<&entt::registry::remove_if_exists<component::BehaviourState>>();
    registry.on_destroy<component::BehaviourState>().connect<&entt::registry::remove_if_exists<component::Characteristics>>();
    registry.on_destroy<component::BehaviourState>().connect<&entt::registry::remove_if_exists<component::BDI>>();
}

} // namespace System
