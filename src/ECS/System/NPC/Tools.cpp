#include "Tools.hpp"

namespace System {

float EmotionalStateOverall(component::Characteristics& target) {
    // Determine the collective weighted emotional response from the NPC's queue
    float collective_emotions = 0.0f;
    for (int i = 0; i < target.emotions.size(); i++) {
        // Calculate the weighting for this emotion
        float relative_pos = (float)(i + 1) / (float)target.emotions.size();
        float weighting = 0.25 + 0.5 * relative_pos;

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

float EmotionalStateIntensity(component::Characteristics& target) {
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
    target.prior_dt = target.current_dt;

    // Assign the current state the newly provided values
    target.current = new_state;
    target.current_dt = starting_dt;
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
    // Not enough time before the 27th, just treat the Desires and Intentions all as root for now
}

} // namespace System
