#pragma once

#include <map>
#include <queue>
#include <set>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include <entt/entt.hpp>
#include "DataStructures/NPC/Symbols.hpp"

namespace component {

    /**
     * @brief A declaration of an NPC's possible resolution for a Goal.
     * @note This is used as a component of the BDI's Intentions.
     * @warning All identifiers stored must be verified before their use!
     */
struct Plan {
        /**
         * @brief The action that the NPC has planned to perform.
         * @warning Not guaranteed to be possible for the NPC to perform!
         */
    npc::Actions action_;

        /**
         * @brief The identifier of the target entity for the planned action.
         * @note Can be used to set the NPC itself as its target.
         */
    entt::entity entity_;

        /**
         * @brief The identifier of the Desire that the NPC has planned to resolve.
         * @warning This should NEVER be 0, that identifier is reserved for the desire root.
         */
    int desire_;

        /**
         * @brief The identifier(s) of the Desire's Goal(s) that the NPC has planned to resolve.
         * @note Can include 0 due to its referential nature as an iterator.
         */
    std::set<int> goals_;

        /**
         * @brief The previous outcome of attempting this Plan.
         * @note Assumes that kUnknown indicates no prior attempts have been made.
         */
    npc::Outcomes history_;

        /// The default constructor is not permitted for use.
    Plan() = delete;

        /**
         * @brief Object constructor, initializes Plan via parameter.
         * @param [in] action The action that the NPC has planned to perform.
         * @param [in] entity The identifier of the target entity for the planned action.
         * @param [in] desire The identifier of the Desire that the NPC has planned to resolve.
         * @param [in] goals The identifier(s) of the Desire's Goal(s) that the NPC has planned to resolve.
         * @param [in] history The previous outcome of attempting this Plan.
         */
    Plan(npc::Actions action, entt::entity entity, int desire,
         std::initializer_list<int> goals,
         npc::Outcomes history = npc::Outcomes::kUnknown);
};

    /**
     * @brief A declaration of an NPC's specific conditional requirement.
     * @note This is used as a component of the BDI's Desires.
     * @warning All identifiers stored must be verified before their use!
     */
struct Goal {
        /**
         * @brief The identifier of the target entity to check.
         * @note Can be used to set the NPC itself as its target.
         */
    entt::entity entity_;

        /// The identifier of the target entity's property to check.
    npc::Properties property_;

        /**
         * @brief The identifier of the target property's component to check.
         * @note Invalid values should be ignored and replaced with kDefault.
         */
    npc::Components component_;

        /**
         * @brief The lower bounds of the conditional range.
         * @warning Must be a smaller value than range_max_!
         */
    float range_min_;

        /**
         * @brief The upper bounds of the conditional range.
         * @warning Must be a larger value than range_min_!
         */
    float range_max_;

        /// The identifier of the Goal's conditional requirement.
    npc::Conditions condition_;

        /**
         * @brief The previous outcome of attempting to resolve this Goal.
         * @note Assumes that kUnknown indicates no prior attempts have been made.
         */
    npc::Outcomes history_;

        /// The default constructor is not permitted for use.
    Goal() = delete;

        /**
         * @brief Object constructor, initializes Goal via parameter.
         * @param [in] entity The identifier of the target entity to check.
         * @param [in] property The identifier of the target entity's property to check.
         * @param [in] component The identifier of the target property's component to check.
         * @param [in] range_min The lower bounds of the conditional range.
         * @param [in] range_max The upper bounds of the conditional range.
         * @param [in] condition The identifier of the Goal's conditional requirement.
         * @param [in] history The previous outcome of attempting to resolve this Goal.
         */
    Goal(entt::entity entity, npc::Properties property, npc::Components component,
         float range_min, float range_max, npc::Conditions condition,
         npc::Outcomes history = npc::Outcomes::kUnknown);
};

    /**
     * @brief An NPC's hierarchical collection of conditional requirements.
     * @note A Desire is only resolved when all of its Goals have been met.
     */
struct Desire {
        /**
         * @brief The identifier of the parent Desire.
         * Used to establish a hierarchy of Desires and branching tree of behaviour.
         * @note A value of 0 is used to indicate this is a root desire and has no parent.
         */
    int parent_;

        /**
         * @brief The Desire's collection of conditional requirements.
         * The Desire is only resolved when all of its Goals have been met.
         * @warning This should never be empty!
         */
    std::vector<Goal> goals_;

        /**
         * @brief The previous outcome of attempting to resolve this Desire.
         * Success only occurs when all of the Desire's Goals are met.
         * Failure only occurs when a Desire's Goal could not be met.
         * @note Assumes that kUnknown indicates no prior attempts have been made.
         */
    npc::Outcomes history_;

        /// The default constructor is not permitted for use.
    Desire() = delete;

        /**
         * @brief Object constructor, initializes Goal via parameter.
         * @param [in] parent The identifier of the parent Desire.
         * @param [in] goals The Desire's collection of conditional requirements.
         * @param [in] history The previous outcome of attempting to resolve this Desire.
         */
    Desire(int parent, std::initializer_list<Goal> goals,
           npc::Outcomes history = npc::Outcomes::kUnknown);
};

    /**
     * @brief An NPC's collection of Beliefs, Desires, and Intentions that influence its behaviour.
     * @note This will always be exclusively contextual to the NPC's perspective.
     */
struct BDI {
        /**
         * @brief The NPC's collection of knowledge of entity properties.
         * Each entity is tracked individually, being mapped to a unique property set.
         * @note An NPC should only gather values for Properties listed here.
         */
    std::map<entt::entity, std::set<npc::Properties>> beliefs_properties_;

        /**
         * @brief The NPC's collection of entity interactions it can perform.
         * Each entity is tracked individually, being mapped to a unique action set.
         * @note An NPC should only plan or perform Actions listed here.
         */
    std::map<entt::entity, std::set<npc::Actions>> beliefs_affordances_;

        /**
         * @brief The NPC's hierarchical collection of Desires to resolve.
         * All Desires are mapped to their unique identifier.
         * @warning No Desire should be mapped to a value of 0 or less!
         */
    std::map<int, Desire> desires_;

        /**
         * @brief The NPC's intended set of reactions to a Desire's resolution.
         * A set of one to many Plans is mapped to a trigger Desire's identifier.
         */
    std::map<int, std::set<Plan>> intentions_;

        /**
         * @brief Default object constructor, initialises BDI to default values.
         * Generates an NPC's BDI with no initial knowledge or content.
         */
    BDI() = default;

        /**
         * @brief Object constructor, initializes BDI via parameter.
         * Establishes an NPC's BDI with an initial knowledge of an entity.
         * @note Always generates npc::Beliefs::kExists for itself, even if not provided.
         * @param [in] entity The identifier of an entity, including itself.
         * @param [in] beliefs_properties The initial property Beliefs for the specified entity.
         * @param [in] beliefs_affordances The initial affordance Beliefs for the specified entity.
         * @param [in] desires The initial set of Desires, paired with their identifying value.
         * @param [in] intentions The initial set of Intentions, paired with their identifying value.
         * @warning Any Desires/Intentions with matching identifiers will be overwritten/dropped!
         */
    explicit BDI(
        entt::entity entity,
        std::initializer_list<npc::Properties> beliefs_properties = {},
        std::initializer_list<npc::Actions> beliefs_affordances = {},
        std::initializer_list<std::pair<const int, Desire>> desires = {},
        std::initializer_list<std::pair<const int, std::set<Plan>>> intentions = {});
};

    /// An emotional response to a specific NPC perception, construct, or circumstance.
struct EmotiveResponse {
        /**
         * @brief Data structure for recording a Belief Property emotive source.
         * Stores a copy of the Belief's symbols and value.
         */
    using SourceProperty = std::tuple<npc::Properties, npc::Components, float>;

        /**
         * @brief Data structure for recording a Belief Action emotive source.
         * Stores a copy of the Belief's symbol.
         */
    using SourceAffordance = npc::Actions;

        /**
         * @brief Data structure for recording an Intention emotive source.
         * Stores a copy of the Intention's identifier and contents.
         */
    using SourceIntention = std::tuple<int, Plan>;

        /**
         * @brief Data structure for recording a Desire emotive source.
         * Stores a copy of the Desire's identifier and contents.
         */
    using SourceDesire = std::tuple<int, Desire>;

        /**
         * @brief Data structure for recording a Goal emotive source.
         * Stores a copy of the Goal's identifiers (parent Desire and itself) and contents.
         */
    using SourceGoal = std::tuple<int, int, Goal>;

        /**
         * @brief Data structure for recording an Event emotive source.
         * Stores a copy of the Event's symbol.
         */
    using SourceEvent = npc::Events;

        /**
         * @brief Stores a copy of the emotive response's source.
         * It nature as a type-safe union enables it to store one of several source types.
         * @note Use std::holds_alternative for type checking, and std::visit for access and use.
         * @warning This will throw std::bad_variant_access if incorrectly treated!
         */
    using SourceWhat = std::variant<std::monostate, SourceProperty, SourceAffordance,
                                    SourceIntention, SourceDesire, SourceGoal, SourceEvent>;

        /**
         * @brief The identifier of the entity causing the emotive response.
         * @note Can be used to set the NPC itself as its target.
         */
    entt::entity entity_;

        /**
         * @brief The emotional response to the NPC perception, construct, or circumstance.
         * Higher values are positive, lower values are negative, zero is apathetic.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         */
    float emotion_;

        /**
         * @brief The source of the emotive response, relative to the tracked entity.
         * @warning This is a type-safe union (std::variant), and must be treated with care!
         * @see EmotiveResponse::SourceWhat
         */
    SourceWhat what_;

        /// The default constructor is not permitted for use.
    EmotiveResponse() = delete;

        /**
         * @brief Object constructor, initializes EmotiveResponse via parameter.
         * @param [in] entity The identifier of the entity causing the emotive response.
         * @param [in] emotion The emotional response to the NPC perception, construct, or circumstance.
         * @param [in] what A copy of the source for the emotive response.
         */
    EmotiveResponse(entt::entity entity, float emotion, SourceWhat&& what);
};

    /**
     * @brief Stores the properties used to establish believable NPC emotions.
     * Secondary to the BDI for behaviour influence.
     */
struct Characteristics {
        /**
         * @brief A collection of short-term reactions to specific events and observations.
         * Ideally, the contents should be popped out from the queue over a period of time.
         */
    std::queue<EmotiveResponse> emotions_;

        /**
         * @brief A long-term emotive state, influenced by the generation of new emotions.
         * Higher values are positive, lower values are negative, zero is apathetic.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         */
    float mood_;

        /**
         * @brief A collection of modifiers for the emotive responses to specific Properties.
         * This value should be added to emotive responses using its mapped Property.
         * Higher values are positive, lower values are negative, zero has no effect.
         * Properties without a modifier should be treated as if they are assigned a modifier of 0.0f.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         */
    std::map<npc::Properties, float> traits_;

        /**
         * @brief A collection of modifiers for behaviour tendencies towards/against specific Actions.
         * This value should be used when establishing plans and determining Action priority.
         * Higher values are positive, lower values are negative, zero is the default.
         * Properties without a modifier should be treated as if they are assigned a modifier of 0.0f.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         * @warning Modifier meaning is established relative to other Action modifiers!
         */
    std::map<npc::Actions, float> personality_;

        /**
         * @brief Default object constructor, initialises Characteristics to default values.
         * Generates a Characteristic initially apathetic, with no emotions, traits, or personality.
         */
    Characteristics();

        /**
         * @brief Object constructor, initialises Characteristics via parameter.
         * Allows for all initial Characteristic properties to be set other than emotions.
         * @param [in] mood A long-term emotive state, influenced by the generation of new emotions.
         * @param [in] emotions A collection of short-term reactions to specific events and observations.
         * @param [in] traits A collection of modifiers for the emotive responses to specific Properties.
         * @param [in] personality A collection of modifiers for behaviour tendencies towards/against specific Actions.
         */
    explicit Characteristics(float mood,
                             std::initializer_list<EmotiveResponse> emotions = {},
                             std::initializer_list<std::pair<const npc::Properties, float>> traits = {},
                             std::initializer_list<std::pair<const npc::Actions, float>> personality = {});
};

} // namespace component
