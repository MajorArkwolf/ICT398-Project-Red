#include "Core.hpp"

#include "ECS/Component/NPC.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/System/NPC/Tools.hpp"

#include "glm/glm.hpp"

namespace System {

const static double EMOTION_QUEUE_TURNOVER_RATE = 1.0 / 60.0;

const static double MOOD_INTENSITY_REDUCTION_RATE = 1.0 / 150.0;

const static double VARIABLE_IDLE_TIME = 3.5;

const static double MINIMUM_IDLE_TIME = 1.0;

const static float INTERACTION_RANGE = 1.0f;

void NPCImport(entt::registry& registry, const entt::entity& entity, std::string path) {
    // TODO: This
    // Not enough time before the 27th, just hard-code the npc initialization for now
}

void NPCExport(entt::registry& registry, const entt::entity& entity, std::string path) {
    // TODO: This
    // Not enough time before the 27th, just hard-code the npc initialization for now
}

void NPCObserve(entt::registry& registry, const entt::entity& entity) {
    // Gather the NPC's BDI and iterate through its Desires
    auto &npc_bdi = registry.get<component::BDI>(entity);
    for (auto &desire: npc_bdi.desires) {
        // Keep track of if this Desire resolution was identified
        npc::Outcomes desire_outcome = npc::Outcomes::kSuccess;

        // Iterate through the current Desire's Goals
        for (auto &goal: desire.second.goals) {
            // Observe only if the Entity has a Belief about the Goal's target Entity
            auto target_entity_beliefs = npc_bdi.beliefs_properties.find(goal.entity);
            if (target_entity_beliefs != npc_bdi.beliefs_properties.end()) {
                // Observe only if the Beliefs of the target Entity includes the Goal's property
                if (target_entity_beliefs->second.find(goal.property) != target_entity_beliefs->second.end()) {
                    // Check if the Goal's target Entity exists within the registry
                    if (registry.valid(goal.entity)) {
                        // Track the value of the target property
                        bool found_value = false;
                        float value_gathered = 0.0f;
                        void* component_returned = nullptr;
                        void* component2_returned = nullptr;

                        // Split logic here for the gathering of target property values
                        switch (goal.property) {
                            case npc::Properties::kExists:
                                // We know the entity exists, so pass a value of 1 in
                                value_gathered = 1.0f;
                                found_value = true;
                                break;
                            case npc::Properties::kType:
                                // Currently unsupported
                                break;
                            case npc::Properties::kSize:
                                // Attempt to get the component if it exists
                                component_returned = registry.try_get<component::PhysicBody>(goal.entity);
                                if (component_returned != nullptr) {
                                    // Gather the component's property value
                                    value_gathered = (float) static_cast<component::PhysicBody*>(component_returned)->mass;
                                    found_value = true;
                                }
                                break;
                            case npc::Properties::kMass:
                                {
                                    // Attempt to get the component if it exists
                                    component_returned = registry.try_get<component::PhysicBody>(goal.entity);
                                    if (component_returned != nullptr) {
                                        // Gather the value from the specified axis
                                        switch (goal.element) {
                                            case npc::Components::kDefault:
                                            case npc::Components::kAxisX:
                                                value_gathered = (float) static_cast<component::Transform*>(component_returned)->scale.x;
                                                break;
                                            case npc::Components::kAxisY:
                                                value_gathered = (float) static_cast<component::Transform*>(component_returned)->scale.y;
                                                break;
                                            case npc::Components::kAxisZ:
                                                value_gathered = (float) static_cast<component::Transform*>(component_returned)->scale.z;
                                                break;
                                        }
                                        found_value = true;
                                    }
                                }
                                break;
                            case npc::Properties::kShape:
                                // Currently unsupported
                                break;
                            case npc::Properties::kPosition:
                                {
                                    // Attempt to get the component if it exists
                                    component_returned = registry.try_get<component::PhysicBody>(goal.entity);
                                    if (component_returned != nullptr) {
                                        // Gather the value from the specified axis
                                        switch (goal.element) {
                                            case npc::Components::kDefault:
                                            case npc::Components::kAxisX:
                                                value_gathered = (float) static_cast<component::Transform*>(component_returned)->pos.x;
                                                break;
                                            case npc::Components::kAxisY:
                                                value_gathered = (float) static_cast<component::Transform*>(component_returned)->pos.y;
                                                break;
                                            case npc::Components::kAxisZ:
                                                value_gathered = (float) static_cast<component::Transform*>(component_returned)->pos.z;
                                                break;
                                        }
                                        found_value = true;
                                    }
                                }
                                break;
                            case npc::Properties::kVelocity:
                                {
                                    // Attempt to get the component if it exists
                                    component_returned = registry.try_get<component::PhysicBody>(goal.entity);
                                    if (component_returned != nullptr) {
                                        // Gather the value from the specified axis
                                        switch (goal.element) {
                                            case npc::Components::kDefault:
                                            case npc::Components::kAxisX:
                                                value_gathered = (float) static_cast<component::PhysicBody*>(component_returned)->linear_velocity.x;
                                                break;
                                            case npc::Components::kAxisY:
                                                value_gathered = (float) static_cast<component::PhysicBody*>(component_returned)->linear_velocity.y;
                                                break;
                                            case npc::Components::kAxisZ:
                                                value_gathered = (float) static_cast<component::PhysicBody*>(component_returned)->linear_velocity.z;
                                                break;
                                        }
                                        found_value = true;
                                    }
                                }
                                break;
                            case npc::Properties::kAcceleration:
                                // Currently unsupported
                                break;
                            case npc::Properties::kOrientation:
                                {
                                    // Attempt to get the component if it exists
                                    component_returned = registry.try_get<component::PhysicBody>(goal.entity);
                                    if (component_returned != nullptr) {
                                        // Gather the value from the specified axis
                                        switch (goal.element) {
                                            case npc::Components::kDefault:
                                                [[fallthrough]];
                                            case npc::Components::kAxisX:
                                                value_gathered = glm::degrees(glm::eulerAngles(static_cast<component::Transform*>(component_returned)->rot).x);
                                            break;
                                            case npc::Components::kAxisY:
                                                value_gathered = glm::degrees(glm::eulerAngles(static_cast<component::Transform*>(component_returned)->rot).y);
                                            break;
                                            case npc::Components::kAxisZ:
                                                value_gathered = glm::degrees(glm::eulerAngles(static_cast<component::Transform*>(component_returned)->rot).z);
                                            break;
                                        }
                                        found_value = true;
                                    }
                                }
                                break;
                            case npc::Properties::kRange:
                                {
                                    // Attempt to get the components if they exist
                                    component_returned = registry.try_get<component::Transform>(entity);
                                    component2_returned = registry.try_get<component::Transform>(goal.entity);
                                    if ((component_returned != nullptr) && (component2_returned != nullptr)) {
                                        // Calculate if the entity is within range and test the Desire
                                        TestGoal(goal,
                                                 INTERACTION_RANGE >= (glm::distance(
                                                     static_cast<component::Transform*>(component_returned)->pos,
                                                     static_cast<component::Transform*>(component2_returned)->pos)));
                                        found_value = true;
                                    }
                                }
                                break;
                            case npc::Properties::kGrabber:
                                // Not supported
                                break;
                            case npc::Properties::kGrabbee:
                                // Not supported
                                break;
                        }

                        // If the property was gathered, test it
                        if (found_value) {
                            TestGoal(goal, value_gathered);
                        }
                        else {
                            // Indicate that the outcome of the Goal and Desire could not be identified
                            goal.history = npc::Outcomes::kUnknown;
                        }
                    }
                    else {
                        // Catch the goal being that the NPC doesn't want the target Entity to exist
                        if (goal.property == npc::Properties::kExists) {
                            TestGoal(goal, 0.0f);
                        }
                        else {
                            // Indicate that the outcome of the Goal and Desire could not be identified
                            goal.history = npc::Outcomes::kUnknown;
                        }
                    }
                }
                else {
                    // Indicate that the outcome of the Goal and Desire could not be identified
                    goal.history = npc::Outcomes::kUnknown;
                }
            }
            else {
                // Indicate that the outcome of the Goal and Desire could not be identified
                goal.history = npc::Outcomes::kUnknown;
            }

            // Update the tracked Desire outcome if required
            if (goal.history < desire_outcome) {
                desire_outcome = goal.history;
            }
        }

        // Store the Desire's history
        desire.second.history = desire_outcome;

        // Catch fulfilment of the Desire
        if (desire.second.history == npc::Outcomes::kSuccess) {
            // Generate a positive emotional response
            auto &emotional_data = registry.get<component::Characteristics>(entity);
            component::EmotiveResponse reaction(entity, 0.5f, std::tuple<int, component::Desire>(desire.first, desire.second));
            emotional_data.emotions.push_back(reaction);

            // Add a slight change to the NPC's mood
            emotional_data.mood += 0.05f;

            // Delete the child Desires as they are no longer needed
            DeleteDesireChildren(npc_bdi.desires, desire.first);
        }
        // Catch failure of the Desire
        else if (desire.second.history == npc::Outcomes::kFailure) {
            // Generate a negative emotional response
            auto &emotional_data = registry.get<component::Characteristics>(entity);
            component::EmotiveResponse reaction(entity, -0.5f, std::tuple<int, component::Desire>(desire.first, desire.second));
            emotional_data.emotions.push_back(reaction);

            // Add a slight change to the NPC's mood
            emotional_data.mood -= 0.05f;
        }
    }

    // Move the NPC to the response phase
    auto &npc_behaviour_state = registry.get<component::BehaviourState>(entity);
    ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kRespond, -1.0);
}

void NPCPrepare(entt::registry& registry, const entt::entity& entity) {
    // Generate a tree of Desires and Intentions to establish a plan to achieve the root Desire
    // TODO: This
    // Not enough time before the 27th, just treat the Desires and Intentions all as root for now

    // Move the NPC to the idle phase
    auto &npc_behaviour_state = registry.get<component::BehaviourState>(entity);
    ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kIdle);
}

void NPCRespond(entt::registry& registry, const entt::entity& entity) {
    // Catch if the NPC has not yet determined which Intention to action
    auto &state = registry.get<component::BehaviourState>(entity);
    if (state.current_intention < 0) {
        // Determine which Intention to Action

    }

    // Action the current Intention

    // Move back to idling after the current action is done.
}

void NPCIdle(entt::registry& registry, const entt::entity& entity) {
    // Calculate the NPC's idle time limit, relative to its overall mood intensity
    auto &npc_characteristics = registry.get<component::Characteristics>(entity);
    double idle_time_limit = MINIMUM_IDLE_TIME + VARIABLE_IDLE_TIME * EmotionalStateOverallIntensity(npc_characteristics);

    // Catch if the NPC should be idling any longer
    auto &npc_behaviour_state = registry.get<component::BehaviourState>(entity);
    if (idle_time_limit < npc_behaviour_state.current_dt) {
        // Move back to the observation state
        ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
    }
}

void NPCsUpdate(entt::registry& registry, double t, double dt) {
    // Loop through the NPCs that have all of the required data structures
    auto npc_view = registry.view<component::BDI, component::Characteristics, component::BehaviourState>();
    for (auto &npc_entity: npc_view) {
        // Get the NPC's BehaviourState and update the current behaviour state's delta timers
        auto &npc_state = npc_view.get<component::BehaviourState>(npc_entity);
        npc_state.current_dt += dt;
        npc_state.emotion_turnover_dt += dt;

        // Get the NPC's characteristics and calculate the mood reduction amount
        auto &npc_characteristics = npc_view.get<component::Characteristics>(npc_entity);
        auto mood_reduction = (float) (MOOD_INTENSITY_REDUCTION_RATE * dt);

        // Decrease the intensity of the NPC's mood
        if (npc_characteristics.mood < 0.0f) {
            // Catch reductions that would cause a swing in the type of mood
            if (0.0f < npc_characteristics.mood + mood_reduction) {
                // Make the NPC's mood fully neutral/apathetic
                npc_characteristics.mood = 0.0f;
            }
            else {
                // Make the NPC's mood slightly less negative
                npc_characteristics.mood += mood_reduction;
            }
        }
        else {
            // Catch reductions that would cause a swing in the type of mood
            if (npc_characteristics.mood - mood_reduction < 0.0f) {
                // Make the NPC's mood fully neutral/apathetic
                npc_characteristics.mood = 0.0f;
            }
            else {
                // Make the NPC's mood slightly less positive
                npc_characteristics.mood -= mood_reduction;
            }
        }

        // Gradually reduce the contents of the NPC's emotional queue
        if (1.0f < npc_state.emotion_turnover_dt * EMOTION_QUEUE_TURNOVER_RATE) {
            // Avoid popping the emotion queue if it is empty
            if (!npc_characteristics.emotions.empty()) {
                // Pop the oldest emotion from the NPC's emotional queue
                npc_characteristics.emotions.pop_front();
            }

            // Clear the NPC's emotional turnover timer
            npc_state.emotion_turnover_dt = 0.0f;
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
