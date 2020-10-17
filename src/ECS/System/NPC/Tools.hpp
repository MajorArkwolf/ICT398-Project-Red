#include "ECS/Component/NPC.hpp"

namespace System {

    /**
     * @brief Calculates the overall emotional state of an NPC from its mood and emotional queue.
     * @note Emotional responses are weighted relative to their position in the queue.
     * @param [in] target A Characteristics component storing an NPC's mood and emotional states.
     * @return The overall emotional state, on a scale from -1.0f to 1.0f.
     */
float EmotionalStateOverall(component::Characteristics& target);

    /**
     * @brief Calculates the overall emotional state intensity of an NPC from its mood and emotional queue.
     * @note Emotional responses are weighted relative to their position in the queue.
     * @param [in] target A Characteristics component storing an NPC's mood and emotional states.
     * @return The overall emotional state intensity, on a scale from 0.0f to 1.0f.
     */
float EmotionalStateOverallIntensity(component::Characteristics& target);

    /**
     * @brief Assigns a new current state to an NPC's BehaviouralState component.
     * Moves the current state over to the prior state.
     * @param [in,out] target A BehaviourState component of an NPC.
     * @param new_state The state that will be made current.
     * @param starting_dt The starting time counter value for the new state.
     * @note Can be used to swap the current and prior values.
     */
void ChangeBehaviouralState(component::BehaviourState& target,
                         npc::Stages new_state,
                         double starting_dt = 0.0f);

} // namespace System
