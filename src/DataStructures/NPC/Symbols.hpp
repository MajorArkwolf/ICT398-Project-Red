#pragma once

namespace npc {

    /**
     * @brief The set of entity properties that an NPC can perceive.
     * @warning Lists only symbolic representations, not values, of properties!
     */
enum class Properties {
    kExists, ///< Object permanence.
    kType, ///< Entity type.
    kSize, ///< Physics body size.
    kMass, ///< Physics body mass.
    kShape, ///< Physics body shape.
    kPosition, ///< Physics body position.
    kVelocity, ///< Physics body velocity.
    kAcceleration, ///< Physics body acceleration.
    kOrientation, ///< Physics body orientation.
    kRange, ///< Entity interaction range.
    kGrabber, ///< Identifier of holding entity.
    kGrabbee ///< Identifier of held entity.
};

    /**
     * @brief The set of actions that an NPC can perform.
     * @warning Lists only symbolic representations, not systems, of actions!
     */
enum class Actions {
    kTraverse, ///< Move its physics body to a specific location.
    kSit, ///< Move its physics body on top of another physics body.
    kReorient, ///< Rotate its physics body to a specific orientation.
    kPush, ///< Apply a force to a physics body other than its own.
    kGrab, ///< Hold a physics body other than its own.
    kDrop, ///< Release a physics body other than its own.
    kObserve, ///< Gather a more in-depth view of an entity.
    kUse ///< Perform a non-physics interaction with an entity.
};

    /**
     * @brief The set of components within a property that an NPC can perceive.
     * @note Used to resolve several data structures, such as an int or vec3.
     * @warning Lists only symbolic representations, not contents, of components!
     */
enum class Components {
    kDefault, ///< For single-component properties, or the first component.
    kAxisX, ///< The X axis component of a vec3 property.
    kAxisY, ///< The Y axis component of a vec3 property.
    kAxisZ ///< The Z axis component of a vec3 property.
};

    /**
     * @brief The set of outcomes that can occur to NPC events and actions.
     * @warning Lists only symbolic representations, not values, of outcomes!
     */
enum class Outcomes {
    kUnknown = 0, ///< For use when the outcome cannot be determined.
    kSuccess = 2, ///< Indicates a positive outcome to an NPC event or action.
    kFailure = 1 ///< Indicates a negative outcome to an NPC event or action.
};

    /**
     * @brief The set of conditions that can be used to establish NPC Goals.
     * @warning Lists only symbolic representations of conditional logic!
     */
enum class Conditions {
    kInRange, ///< The value must be within the condition's range.
    kNotInRange, ///< The value must NOT be within the condition's range.
    kBelowRange, ///< The value must be below the lower bounds of the condition's range.
    kNotBelowRange, ///< The value must NOT be below the lower bounds of the condition's range.
    kAboveRange, ///< The value must be above the upper bounds of the condition's range.
    kNotAboveRange ///< The value must NOT be above the upper bounds of the condition's range.
};

    /**
     * @brief The set of events that an NPC can emotionally react to.
     * @warning Lists only symbolic representations, not details, of events!
     */
enum class Events {
    kCollision, ///< The NPC collided with another entity.
    kIntentionEnded, ///< The NPC finished operating on an Intention.
    kDesireEnded, ///< The NPC finished operating on a Desire.
    kAnalysisEnded ///< The NPC finished operating its analysis.
};

    /**
     * @brief The set of generic stages of an NPC's behaviour.
     * @warning Lists only symbolic representations of general behaviour characteristics!
     */
enum class Stages {
    kIdle, ///< The NPC is not performing any significant actions.
    kPrepare, ///< The NPC is creating a branching hierarchy of Desires and Intentions.
    kObserve, ///< The NPC is establishing Beliefs from perceptions and testing Desires.
    kRespond, ///< The NPC is checking for triggered Intentions and performing their actions.
    kOther ///< The NPC's behaviour is in a misc. stage not otherwise listed.
};

} // namespace npc
