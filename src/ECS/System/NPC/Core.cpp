#include "Core.hpp"

#include "ECS/Component/NPC.hpp"
#include "ECS/Component/Basic.hpp"
#include "ECS/Component/Board.hpp"
#include "ECS/System/NPC/Tools.hpp"

#include "glm/glm.hpp"

#include <random>

namespace System {

const static double EMOTION_QUEUE_TURNOVER_RATE = 1.0 / 60.0;

const static double MOOD_INTENSITY_REDUCTION_RATE = 1.0 / 150.0;

const static double VARIABLE_IDLE_TIME = 3.5;

const static double MINIMUM_IDLE_TIME = 1.0;

const static double SIT_IDLE_TIME = 6.0;

const static double USE_IDLE_TIME = 4.0;

const static float INTERACTION_RANGE = 1.0f;

constexpr float NPC_SPEED = 3.0f;

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
            auto &target_entity_beliefs = npc_bdi.beliefs_properties.find(goal.entity);
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
                                    value_gathered = static_cast<component::PhysicBody*>(component_returned)->mass;
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
                                            value_gathered = static_cast<component::Transform*>(component_returned)->scale.x;
                                            break;
                                        case npc::Components::kAxisY:
                                            value_gathered = static_cast<component::Transform*>(component_returned)->scale.y;
                                            break;
                                        case npc::Components::kAxisZ:
                                            value_gathered = static_cast<component::Transform*>(component_returned)->scale.z;
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
                                        value_gathered = static_cast<component::Transform*>(component_returned)->pos.x;
                                        break;
                                    case npc::Components::kAxisY:
                                        value_gathered = static_cast<component::Transform*>(component_returned)->pos.y;
                                        break;
                                    case npc::Components::kAxisZ:
                                        value_gathered = static_cast<component::Transform*>(component_returned)->pos.z;
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
                                        value_gathered = static_cast<component::PhysicBody*>(component_returned)->linear_velocity.x;
                                        break;
                                    case npc::Components::kAxisY:
                                        value_gathered = static_cast<component::PhysicBody*>(component_returned)->linear_velocity.y;
                                        break;
                                    case npc::Components::kAxisZ:
                                        value_gathered = static_cast<component::PhysicBody*>(component_returned)->linear_velocity.z;
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
                                    if (INTERACTION_RANGE >= (glm::distance(
                                        static_cast<component::Transform*>(component_returned)->pos,
                                        static_cast<component::Transform*>(component2_returned)->pos))) {
                                        // Set the value gathered to 1.0 to indicate the entity is within range
                                        value_gathered = 1.0f;
                                    }
                                    // By default the found valuewill be 0.0f, so that doesn't need to be added
                                    found_value = true;
                                }
                            }
                                break;
                            case npc::Properties::kGrabber:
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
            //DeleteDesireChildren(npc_bdi.desires, desire.first);
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

    // Move the NPC to the prepare phase
    auto &npc_behaviour_state = registry.get<component::BehaviourState>(entity);
    ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kPrepare);
}

void NPCPrepare(entt::registry& registry, const entt::entity& entity) {
    // Preferably we would generate a tree of Desires and Intentions to establish a plan to achieve the root Desire
    // This is no longer viable within the deadline, so instead the triggered Intentions will be tracked
    //   and a target Intention to perform will be identified.

    // Gather the NPC components
    auto &npc_bdi = registry.get<component::BDI>(entity);
    auto &npc_characteristics = registry.get<component::Characteristics>(entity);
    auto &npc_behaviour_state = registry.get<component::BehaviourState>(entity);

    // Determine what Desires have been fulfilled and assign a priority by their hierarchy
    std::map<int, std::set<int>> fulfilled_desires = FulfilledDesires(npc_bdi, 0, npc_bdi.root_desires);

    // Gather the identifiers of Desires that would trigger an intention
    std::set<int> trigger_desires;
    for (auto &current_intention: npc_bdi.intentions) {
        // Store the identifier of the Desire that would trigger the Intention
        trigger_desires.insert(current_intention.first);
    }

    // Filter out the above data to only include Desire identifiers used by Intentions
    std::set<int> intersect_store;
    for (auto &current_layer: fulfilled_desires) {
        // Only retain the identifiers that are also found in the gathered trigger Desires
        std::set_intersection(current_layer.second.begin(), current_layer.second.end(),
                              trigger_desires.begin(), trigger_desires.end(),
                              std::inserter(intersect_store, intersect_store.begin()));
        current_layer.second = intersect_store;

        // Clear the data from the temp store
        intersect_store.clear();
    }

    // Track the to-be-determined Intention & Plan
    int new_trigger_identifier = -1;
    int new_plan_identifier = -1;

    // Determine which Intention & Plan should be actioned, going backwards through the hierarchy
    for (auto current_desire_layer = fulfilled_desires.rbegin(); current_desire_layer != fulfilled_desires.rend(); ++current_desire_layer) {
        // Catch if the current layer doesn't has any Desire identifiers remaining
        if (current_desire_layer->second.empty()) {
            // Move onto the layer above in the hierarchy, none remain after the filtering
            continue;
        }

        // Determine which Desire trigger to respond to
        auto target_desire_trigger = npc_bdi.intentions.begin();
        if (current_desire_layer->second.size() == 1) {
            // Save performance if possible
            target_desire_trigger = npc_bdi.intentions.find(*current_desire_layer->second.begin());
        }
        else {
            // Calculate the weighting for the triggered Intentions and gather their iterators
            std::list<double> raw_distributions;
            std::vector<std::map<int, std::vector<component::Plan>>::iterator> intention_iterators;
            for (auto &current_trigger: current_desire_layer->second) {
                // Store the iterator to the triggered Intentions and set its baseline distribution
                intention_iterators.push_back(npc_bdi.intentions.find(current_trigger));
                raw_distributions.push_back(0.5);

                // If this was the same trigger as before, apply a modifier
                if (npc_behaviour_state.prior_intention.first == current_trigger) {
                    // Scale the modifier relative to the NPC's emotional state
                    *raw_distributions.rbegin() -= 0.5 * (double) EmotionalStateOverall(npc_characteristics);
                }
            }

            // Randomly pick from the triggered Intentions, using the calculated weightings
            std::mt19937 random_number_generator (std::chrono::system_clock::now().time_since_epoch().count());
            std::discrete_distribution<int> discrete_distributions(raw_distributions.begin(), raw_distributions.end());
            target_desire_trigger = intention_iterators[discrete_distributions(random_number_generator)];
        }

        // Catch an empty set of Plans in the selected triggered Intention
        if (target_desire_trigger->second.empty()) {
            // Break out of the loop, this is undefined behaviour
            break;
        }

        // Determine which plan from the selected intention should be Actioned
        int target_intention_plan = 0;
        if (target_desire_trigger->second.size() == 1) {
            // Save performance if possible
            new_trigger_identifier = target_desire_trigger->first;
            new_plan_identifier = target_intention_plan;
        }
        else if (1 < target_desire_trigger->second.size()) {
            // Calculate the weighting for the triggered Plans
            std::list<double> raw_distributions;
            std::vector<int> plan_identifiers;
            for (auto &current_plan: target_desire_trigger->second) {
                // Set the current Plan's baseline distribution
                raw_distributions.push_back(0.5);
                plan_identifiers.push_back(raw_distributions.size() - 1);

                // If this was the same trigger as before, apply a modifier
                if (npc_behaviour_state.prior_intention.second == raw_distributions.size() - 1) {
                    // Scale the modifier relative to the NPC's emotional state
                    *raw_distributions.rbegin() -= 0.5 * (double) EmotionalStateOverall(npc_characteristics);
                }

                // Catch the NPC's Characteristics being applicable
                if (npc_characteristics.personality.find(current_plan.action) != npc_characteristics.personality.end()) {
                    // Apply the modifier, no scaling will be done here for now
                    *raw_distributions.rbegin() += npc_characteristics.personality[current_plan.action];
                }
            }

            // Randomly pick from the triggered Intentions, using the calculated weightings
            std::mt19937 random_number_generator (std::chrono::system_clock::now().time_since_epoch().count());
            std::discrete_distribution<int> discrete_distributions(raw_distributions.begin(), raw_distributions.end());
            target_intention_plan = plan_identifiers[discrete_distributions(random_number_generator)];

            // Track the new target Intention & Plan
            new_trigger_identifier = target_desire_trigger->first;
            new_plan_identifier = target_intention_plan;
        }

        // Break out of the loop, job's done
        break;
    }

    // Update the current Intention stored the the NPC's BehaviourState component
    ChangeIntention(npc_behaviour_state, new_trigger_identifier, new_plan_identifier);

    // Move the NPC to the respond phase
    ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kRespond);
}

void NPCRespond(entt::registry& registry, const entt::entity& entity) {
    // Catch if the NPC has not yet determined which Intention to action
    auto &npc_behaviour_state = registry.get<component::BehaviourState>(entity);
    if ((npc_behaviour_state.current_intention.first < 0) || npc_behaviour_state.current_intention.second < 0) {
        // Move the NPC to the idle phase, hopefully the next loop will trigger an Intention
        ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kIdle);
    }

    // Catch invalid Intentions
    auto &npc_bdi = registry.get<component::BDI>(entity);
    if (npc_bdi.intentions.find(npc_behaviour_state.current_intention.first) == npc_bdi.intentions.end()) {
        // Move the NPC to the idle phase, hopefully the next loop will trigger an Intention
        ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kIdle);
    }
    else if ((npc_behaviour_state.current_intention.second < 0) ||
             (npc_bdi.intentions[npc_behaviour_state.current_intention.first].size() < npc_behaviour_state.current_intention.second)) {
        // Move the NPC to the idle phase, hopefully the next loop will trigger an Intention
        ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kIdle);
    }

    // Keep a simple and quick reference to the current Intention/Plan
    //std::cout << npc_behaviour_state.current_intention.first << " " << npc_behaviour_state.current_intention.second << std::endl;
    auto find_first = npc_bdi.intentions.find(npc_behaviour_state.current_intention.first);
    if (find_first == npc_bdi.intentions.end()) {
        std::cout << "ERROR: NPC Respond first intention out of range!\n";
        return;
    }
    auto var = npc_bdi.intentions.at(npc_behaviour_state.current_intention.first);
    if (var.empty() && npc_behaviour_state.current_intention.second < 0 || npc_behaviour_state.current_intention.second > var.size()) {
        std::cout << "ERROR: NPC Respond second intention out of range!\n";
        return;
    }
    component::Plan &current_plan = var.at(npc_behaviour_state.current_intention.second);

    // Attempt to respond the current Intention using the NPC's Plan.
    auto &npc_transform = registry.get<component::Transform>(entity);
    switch (current_plan.action) {
        case npc::Actions::kTraverse:
            // Init action (only perform once)
            if (!npc_behaviour_state.has_begun_response) {
                if (!registry.has<component::Moving>(entity)) {
                    auto e = registry.view<component::Board>()[0];
                    if (!registry.has<component::Board>(e)) {
                        return;
                    }
                    auto &board = registry.get<component::Board>(e);
                    auto &moving = registry.emplace<component::Moving>(entity);
                    moving.speed = NPC_SPEED;
                    moving.SetLastNode(registry, board.GetClosestNode(npc_transform.pos));
                }
                auto e = registry.view<component::Board>()[0];
                if (!registry.has<component::Board>(e)) {
                    return;
                }
                auto &board = registry.get<component::Board>(e);
                auto &moving = registry.get<component::Moving>(entity);
                moving.move_list = board.FindPath(registry, moving.last_node, board.GetClosestNode(registry.get<component::Transform>(current_plan.entity).pos));
                //This may cause strange behaviour if an object is out of distance but in the same node.
                if (!moving.move_list.empty()) {
                    moving.is_moving = true;
                }
                // Prevent this from being repeated
                npc_behaviour_state.has_begun_response = true;
            }

            // Perform action
            // Managed Externally by AISystem.cpp

            // Check if action has finished
            if (registry.has<component::Moving>(entity) && !registry.get<component::Moving>(entity).is_moving) {
                // Ending actions
                // Swap back to observing state, this will also deal with the emotional response
                ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
            }
            // End of Traverse Action
            break;
        case npc::Actions::kSit:
            // Check that the NPC is within range of the target it will 'sit' on/at
            if (registry.has<component::Transform>(current_plan.entity)) {
                // Check if the NPC is within the interaction range
                auto &target_transform = registry.get<component::Transform>(current_plan.entity);

                // Catch if the NPC is not within the interaction range to be able to sit
                if (INTERACTION_RANGE < glm::distance(npc_transform.pos, target_transform.pos)) {
                    // Swap back to observing state, this will also deal with the emotional response
                    ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
                }
            }
            else {
                // Swap back to observing state, this will also deal with the emotional response
                ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
            }

            // Init action (only perform once)
            if (!npc_behaviour_state.has_begun_response) {
                // Make the NPC somewhat statically 'sit'
                if (registry.has<component::Animation>(entity)) {
                    auto &anim = registry.get<component::Animation>(entity);
                    anim.animator_.LoadAnimation("WORKING", true);
                }

                // Prevent this from being repeated
                npc_behaviour_state.has_begun_response = true;
            }

            // Check if action has finished
            if (SIT_IDLE_TIME < npc_behaviour_state.current_dt) {
                // Swap back to observing state, this will also deal with the emotional response
                ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
            }
            break;
        case npc::Actions::kReorient:
        case npc::Actions::kPush:
        case npc::Actions::kGrab:
        case npc::Actions::kDrop:
            // Currently unsupported
            ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kIdle);
            break;
        case npc::Actions::kObserve:
            // Just swap back to the observation stage
            ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
            break;
        case npc::Actions::kUse:
            // Check that the NPC is within range of the target it will 'use'
            if (registry.has<component::Transform>(current_plan.entity)) {
                // Check if the NPC is within the interaction range
                auto &target_transform = registry.get<component::Transform>(current_plan.entity);

                // Catch if the NPC is not within the interaction range to be able to sit
                if (INTERACTION_RANGE < glm::distance(npc_transform.pos, target_transform.pos)) {
                    // Swap back to observing state, this will also deal with the emotional response
                    ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
                }
            }
            else {
                // Swap back to observing state, this will also deal with the emotional response
                ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
            }

            // Init action (only perform once)
            if (!npc_behaviour_state.has_begun_response) {
                // Make the NPC somewhat statically 'sit'
                if (registry.has<component::Animation>(entity)) {
                    auto &anim = registry.get<component::Animation>(entity);
                    anim.animator_.LoadAnimation("PUNCH", false);
                }

                // Prevent this from being repeated
                npc_behaviour_state.has_begun_response = true;
            }

            // Check if action has finished
            if (USE_IDLE_TIME < npc_behaviour_state.current_dt) {
                // Swap back to observing state, this will also deal with the emotional response
                ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kObserve);
            }
            break;
        default:
            // Catch other unsupported
            ChangeBehaviouralState(npc_behaviour_state, npc::Stages::kIdle);
            break;
    }
}

void NPCIdle(entt::registry& registry, const entt::entity& entity) {
    // Catch prior NPC Response action animations passing through
    auto &npc_behaviour_state = registry.get<component::BehaviourState>(entity);
    if (!npc_behaviour_state.has_begun_response) {
        // Start the NPC's animation
        if (registry.has<component::Animation>(entity)) {
            auto &anim = registry.get<component::Animation>(entity);
            anim.animator_.LoadAnimation("IDLE", false);
        }

        // Prevent this from being repeated
        npc_behaviour_state.has_begun_response = true;
    }

    // Calculate the NPC's idle time limit, relative to its overall mood intensity
    auto &npc_characteristics = registry.get<component::Characteristics>(entity);
    double idle_time_limit = MINIMUM_IDLE_TIME + VARIABLE_IDLE_TIME * static_cast<double>(EmotionalStateOverallIntensity(npc_characteristics));

    // Catch if the NPC should be idling any longer
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
        if (1.0 < npc_state.emotion_turnover_dt * EMOTION_QUEUE_TURNOVER_RATE) {
            // Avoid popping the emotion queue if it is empty
            if (!npc_characteristics.emotions.empty()) {
                // Pop the oldest emotion from the NPC's emotional queue
                npc_characteristics.emotions.pop_front();
            }

            // Clear the NPC's emotional turnover timer
            npc_state.emotion_turnover_dt = 0.0;
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
