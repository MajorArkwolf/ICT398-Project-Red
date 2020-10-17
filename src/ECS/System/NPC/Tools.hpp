#include "ECS/Component/NPC.hpp"

namespace System {

    /**
     * @brief Calculates the overall emotional state of an NPC from its mood and emotional queue.
     * @note Emotional responses are weighted relative to their position in the queue.
     * @param target The NPC component storing its emotional states.
     * @return The overall emotional state, on a scale from -1.0f to 1.0f.
     */
float EmotionalStateOverall(component::Characteristics& target);

    /**
     * @brief Calculates the overall emotional state intensity of an NPC from its mood and emotional queue.
     * @note Emotional responses are weighted relative to their position in the queue.
     * @param target The NPC component storing its emotional states.
     * @return The overall emotional state intensity, on a scale from 0.0f to 1.0f.
     */
float EmotionalStateOverallIntensity(component::Characteristics& target);

} // namespace System
