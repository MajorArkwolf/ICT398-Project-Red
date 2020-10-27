#include "ECS/Component/NPC.hpp"
#include "ECS/Component/Basic.hpp"

#include "glm/glm.hpp"

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
     * @param [in] new_state The state that will be made current.
     * @param [in] starting_dt The starting time counter value for the new state.
     * @note Can be used to swap the current and prior values.
     */
void ChangeBehaviouralState(component::BehaviourState& target,
                         npc::Stages new_state,
                         double starting_dt = 0.0f);

    /**
     * @brief Assigns a new current Intention to an NPC's BehaviouralState component.
     * Moves the current Intention over to the prior Intention.
     * @param [in,out] target A BehaviourState component of an NPC.
     * @param [in] trigger The trigger identifier of the new Intention.
     * @param [in] plan The Plan identifier of the new Intention.
     */
void ChangeIntention(component::BehaviourState& target,
                     int trigger = -1, int plan = -1);

    /**
     * @brief Tests a provided value meets a Goal's conditions.
     * Additionally updated the Goal's history according to the outcome of the test result.
     * @param [in,out] target A Goal from a BDI Desire Component of an NPC.
     * @param [in] value The value to test the Goal with.
     * @warning Invalid range values from the provided Goal will result in undefined behaviour!
     */
void TestGoal(component::Goal& target, float value);

    /**
     * @brief Deletes the child Desires from a specified 'root'.
     * Additionally cleans up any incorrect child listings in the root desire.
     * @note If an invalid root is provided, no changes will be made.
     * @param [in,out] desire_store The collection of desires to modify.
     * @param [in] root The 'root' Desire to begin operation from.
     * @warning No checks will be made to avoid issues from circular relationships between Desires!
     */
void DeleteDesireChildren(std::map<int, component::Desire>& desire_store, int root);

    /**
     * @brief Gathers the identifiers of Desires that have been fulfilled, including their children.
     * If a Desire is fulfilled, its tracked children will be ignored.
     * @note This will call itself recursively when the child Desires have tracked children.
     * @param [in] target The BDI component of the target NPC.
     * @param [in] current_layer The identifier of the current layer in the Desire hierarchy
     * @param [in] child_identifiers The collection of identifiers for child Desire identifiers.
     * @param [in] fulfilled_hierarchy The sets of already known fulfilled Desire identifiers, mapped to their hierarchy layer.
     * @warning Different Desires with the same or recursive children will lead to undefined behaviour!
     * @return The sets of fulfilled Desire identifiers, mapped to their hierarchy layer.
     */
std::map<int, std::set<int>> FulfilledDesires(component::BDI& target, int current_layer,
                                              std::set<int>& child_identifiers,
                                              std::map<int, std::set<int>> fulfilled_hierarchy = {{}});

    /**
     * @brief Calculates if the NPC is within range of a target Entity.
     * If the provided type is invalid, the NPC should never be within range.
     * @param entity_position The position of the NPC Entity.
     * @param target_position The position of the target Entity.
     * @param target_type The type of Entity that has been targeted.
     * @return True if the NPC is within range, False otherwise.
     */
bool EntityIsWithinRange(glm::vec3& entity_position, glm::vec3& target_position,
                         component::InteractableObject::Type target_type);

} // namespace System
