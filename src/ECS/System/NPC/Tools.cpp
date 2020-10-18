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

bool TestGoal(component::Goal& target, float value) {
    // Perform the calculations depending on the requested condition
    switch (target.condition) {
        case npc::Conditions::kInRange:
            return (target.range_min < value) && (value < target.range_max);
        case npc::Conditions::kNotInRange:
            return (target.range_max < value) || (value < target.range_min);
        case npc::Conditions::kBelowRange:
            return value < target.range_min;
        case npc::Conditions::kNotBelowRange:
            return target.range_min <= value;
        case npc::Conditions::kAboveRange:
            return target.range_max < value;
        case npc::Conditions::kNotAboveRange:
            return value <= target.range_max;
    }

    // Any value other than the above is not supported and undefined, so just return false
    return false;
}

} // namespace System
