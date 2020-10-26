#include "Tools.hpp"

namespace System {

float EmotionalStateOverall(component::Characteristics& target) {
    // Determine the collective weighted emotional response from the NPC's queue
    float collective_emotions = 0.0f;
    for (int i = 0; i < target.emotions.size(); i++) {
        // Calculate the weighting for this emotion
        float relative_pos = (float)(i + 1) / (float)target.emotions.size();
        float weighting = 0.25f + 0.5f * relative_pos;

        // Add the weighted emotional response to the total
        collective_emotions += target.emotions[i].emotion * weighting;
    }

    // Scale the intensity of the emotions relative to how many were in the queue
    collective_emotions = collective_emotions * (1.0f / (float)target.emotions.size());

    // Add the scaled and weighted collective emotions to the NPC's mood and enforce bounds
    float overall = target.mood + collective_emotions;
    if (overall < 1.0f) {
        overall = 1.0f;
    } else if (-1.0f < overall) {
        overall = -1.0f;
    }

    // Return the NPC's current overall emotional state
    return overall;
}

float EmotionalStateOverallIntensity(component::Characteristics& target) {
    // Get the overall emotional state of the NPC and just prevent negative values
    float intensity = EmotionalStateOverall(target);
    if (intensity < 0.0f)
        intensity = intensity * (-1.0f);
    return intensity;
}

void ChangeBehaviouralState(component::BehaviourState& target, npc::Stages new_state,
                            double starting_dt) {
    // Copy the current values from the current state to the prior
    target.prior = target.current;

    // Assign the current state the newly provided values
    target.current = new_state;
    target.current_dt = starting_dt;

    // Indicate the current response has yet to be begin
    target.has_begun_response = false;
}

void ChangeIntention(component::BehaviourState& target, int trigger, int plan) {
    // Copy the current values from the current intention to the prior
    target.prior_intention = target.current_intention;

    // Assign the current Intention the newly assigned values
    target.current_intention.first = trigger;
    target.current_intention.second = plan;

    // Indicate the current response has yet to be begin
    target.has_begun_response = false;
}

void TestGoal(component::Goal& target, float value) {
    // Perform the calculations depending on the requested condition
    bool result = false;
    switch (target.condition) {
        case npc::Conditions::kInRange:
            result = (target.range_min < value) && (value < target.range_max);
            break;
        case npc::Conditions::kNotInRange:
            result = (target.range_max < value) || (value < target.range_min);
            break;
        case npc::Conditions::kBelowRange:
            result = value < target.range_min;
            break;
        case npc::Conditions::kNotBelowRange:
            result = target.range_min <= value;
            break;
        case npc::Conditions::kAboveRange:
            result = target.range_max < value;
            break;
        case npc::Conditions::kNotAboveRange:
            result = value <= target.range_max;
            break;
        default:
            // Any value other than the above is not supported and undefined, so just return false
            result = false;
    }

    // Update the Goal's history
    if (result) {
        // Indicate success
        target.history = npc::Outcomes::kSuccess;
    }
    else {
        // Indicate failure
        target.history = npc::Outcomes::kFailure;
    }
}

void DeleteDesireChildren(std::map<int, component::Desire>& desire_store, int root) {
    // TODO: This
    // Not enough time before the 27th, this will not be used for the short-term solution
}

std::map<int, std::set<int>> FulfilledDesires(component::BDI& target, int current_layer,
                                              std::set<int>& child_identifiers,
                                              std::map<int, std::set<int>> fulfilled_hierarchy) {
    // Iterate through the current set of child Desire identifiers
    for (auto current_identifier: child_identifiers) {
        // Get access to the current Desire
        auto current_desire = target.desires.find(current_identifier);

        // Catch an invalid identifier
        if (current_desire == target.desires.end()) {
            // Remove the invalid child identifier from the tracked set
            child_identifiers.erase(current_identifier);

            // Skip to the next identifier in the loop
            continue;
        }

        // Check if the currently accessed desire has been achieved
        if (current_desire->second.history == npc::Outcomes::kSuccess) {
            // Add the identifier of the current Desire to the fulfilled hierarchy
            fulfilled_hierarchy[current_layer].insert(current_identifier);
        }
        else {
            // Identify if this Desire's children have been fulfilled, this is recursive
            FulfilledDesires(target, current_layer + 1,
                             current_desire->second.children,
                             fulfilled_hierarchy);
        }
    }

    // Return the 'hierarchical' map of sets of identified Desires
    return fulfilled_hierarchy;
}

} // namespace System
