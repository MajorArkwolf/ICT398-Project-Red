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
    npc::Actions action;

        /**
         * @brief The identifier of the target entity for the planned action.
         * @note Can be used to set the NPC itself as its target.
         */
    entt::entity entity;

        /**
         * @brief The identifier of the Desire that the NPC has planned to resolve.
         * @warning This should NEVER be 0, that identifier is reserved for the desire root.
         */
    int desire;

        /**
         * @brief The identifier(s) of the Desire's Goal(s) that the NPC has planned to resolve.
         * @note Can include 0 due to its referential nature as an iterator.
         */
    std::set<int> goals;

        /**
         * @brief The previous outcome of attempting this Plan.
         * @note Assumes that kUnknown indicates no prior attempts have been made.
         */
    npc::Outcomes history;

        /// The default constructor is not permitted for use.
    Plan() = delete;

        /**
         * @brief Object constructor, initializes Plan via parameter.
         * @param [in] action_in The action that the NPC has planned to perform.
         * @param [in] entity_in The identifier of the target entity for the planned action.
         * @param [in] desire_in The identifier of the Desire that the NPC has planned to resolve.
         * @param [in] goals_in The identifier(s) of the Desire's Goal(s) that the NPC has planned to resolve.
         * @param [in] history_in The previous outcome of attempting this Plan.
         */
    Plan(npc::Actions action_in, entt::entity entity_in, int desire_in,
         std::initializer_list<int> goals_in,
         npc::Outcomes history_in = npc::Outcomes::kUnknown);
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
    entt::entity entity;

        /// The identifier of the target entity's property to check.
    npc::Properties property;

        /**
         * @brief The identifier of the target property's component to check.
         * @note Invalid values should be ignored and replaced with kDefault.
         */
    npc::Components element;

        /**
         * @brief The lower bounds of the conditional range.
         * @warning Must be a smaller value than range_max_!
         */
    float range_min;

        /**
         * @brief The upper bounds of the conditional range.
         * @warning Must be a larger value than range_min_!
         */
    float range_max;

        /// The identifier of the Goal's conditional requirement.
    npc::Conditions condition;

        /**
         * @brief The previous outcome of attempting to resolve this Goal.
         * @note Assumes that kUnknown indicates no prior attempts have been made.
         */
    npc::Outcomes history;

        /// The default constructor is not permitted for use.
    Goal() = delete;

        /**
         * @brief Object constructor, initializes Goal via parameter.
         * @param [in] entity_in The identifier of the target entity to check.
         * @param [in] property_in The identifier of the target entity's property to check.
         * @param [in] element_in The identifier of the target property's component to check.
         * @param [in] range_min_in The lower bounds of the conditional range.
         * @param [in] range_max_in The upper bounds of the conditional range.
         * @param [in] condition_in The identifier of the Goal's conditional requirement.
         * @param [in] history_in The previous outcome of attempting to resolve this Goal.
         */
    Goal(entt::entity entity_in, npc::Properties property_in, npc::Components element_in,
         float range_min_in, float range_max_in, npc::Conditions condition_in,
         npc::Outcomes history_in = npc::Outcomes::kUnknown);
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
    int parent;

        /**
         * @brief The Desire's collection of conditional requirements.
         * The Desire is only resolved when all of its Goals have been met.
         * @warning This should never be empty!
         */
    std::vector<Goal> goals;

        /**
         * @brief The previous outcome of attempting to resolve this Desire.
         * Success only occurs when all of the Desire's Goals are met.
         * Failure only occurs when a Desire's Goal could not be met.
         * @note Assumes that kUnknown indicates no prior attempts have been made.
         */
    npc::Outcomes history;

        /// The default constructor is not permitted for use.
    Desire() = delete;

        /**
         * @brief Object constructor, initializes Goal via parameter.
         * @param [in] parent_in The identifier of the parent Desire.
         * @param [in] goals_in The Desire's collection of conditional requirements.
         * @param [in] history_in The previous outcome of attempting to resolve this Desire.
         */
    Desire(int parent_in, std::initializer_list<Goal> goals_in,
           npc::Outcomes history_in = npc::Outcomes::kUnknown);
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
    std::map<entt::entity, std::set<npc::Properties>> beliefs_properties;

        /**
         * @brief The NPC's collection of entity interactions it can perform.
         * Each entity is tracked individually, being mapped to a unique action set.
         * @note An NPC should only plan or perform Actions listed here.
         */
    std::map<entt::entity, std::set<npc::Actions>> beliefs_affordances;

        /**
         * @brief The NPC's hierarchical collection of Desires to resolve.
         * All Desires are mapped to their unique identifier.
         * @warning No Desire should be mapped to a value of 0 or less!
         */
    std::map<int, Desire> desires;

        /**
         * @brief The NPC's intended set of reactions to a Desire's resolution.
         * A set of one to many Plans is mapped to a trigger Desire's identifier.
         */
    std::map<int, std::set<Plan>> intentions;

        /**
         * @brief Default object constructor, initialises BDI to default values.
         * Generates an NPC's BDI with no initial knowledge or content.
         */
    BDI() = default;

        /**
         * @brief Object constructor, initializes BDI via parameter.
         * Establishes an NPC's BDI with an initial knowledge of an entity.
         * @note Always generates npc::Beliefs::kExists for itself, even if not provided.
         * @param [in] entity_in The identifier of an entity, including itself.
         * @param [in] beliefs_properties_in The initial property Beliefs for the specified entity.
         * @param [in] beliefs_affordances_in The initial affordance Beliefs for the specified entity.
         * @param [in] desires_in The initial set of Desires, paired with their identifying value.
         * @param [in] intentions_in The initial set of Intentions, paired with their identifying value.
         * @warning Any Desires/Intentions with matching identifiers will be overwritten/dropped!
         */
    explicit BDI(
        entt::entity entity_in,
        std::initializer_list<npc::Properties> beliefs_properties_in = {},
        std::initializer_list<npc::Actions> beliefs_affordances_in = {},
        std::initializer_list<std::pair<const int, Desire>> desires_in = {},
        std::initializer_list<std::pair<const int, std::set<Plan>>> intentions_in = {});
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
    entt::entity entity;

        /**
         * @brief The emotional response to the NPC perception, construct, or circumstance.
         * Higher values are positive, lower values are negative, zero is apathetic.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         */
    float emotion;

        /**
         * @brief The source of the emotive response, relative to the tracked entity.
         * @warning This is a type-safe union (std::variant), and must be treated with care!
         * @see EmotiveResponse::SourceWhat
         */
    SourceWhat what;

        /// The default constructor is not permitted for use.
    EmotiveResponse() = delete;

        /**
         * @brief Object constructor, initializes EmotiveResponse via parameter.
         * @param [in] entity_in The identifier of the entity causing the emotive response.
         * @param [in] emotion_in The emotional response to the NPC perception, construct, or circumstance.
         * @param [in] what_in A copy of the source for the emotive response.
         */
    EmotiveResponse(entt::entity entity_in, float emotion_in, SourceWhat&& what_in);
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
    std::queue<EmotiveResponse> emotions;

        /**
         * @brief A long-term emotive state, influenced by the generation of new emotions.
         * Higher values are positive, lower values are negative, zero is apathetic.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         */
    float mood;

        /**
         * @brief A collection of modifiers for the emotive responses to specific Properties.
         * This value should be added to emotive responses using its mapped Property.
         * Higher values are positive, lower values are negative, zero has no effect.
         * Properties without a modifier should be treated as if they are assigned a modifier of 0.0f.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         */
    std::map<npc::Properties, float> traits;

        /**
         * @brief A collection of modifiers for behaviour tendencies towards/against specific Actions.
         * This value should be used when establishing plans and determining Action priority.
         * Higher values are positive, lower values are negative, zero is the default.
         * Properties without a modifier should be treated as if they are assigned a modifier of 0.0f.
         * @note Values are expected to be within a range of 1.0f and -1.0f.
         * @warning Modifier meaning is established relative to other Action modifiers!
         */
    std::map<npc::Actions, float> personality;

        /**
         * @brief Default object constructor, initialises Characteristics to default values.
         * Generates a Characteristic initially apathetic, with no emotions, traits, or personality.
         */
    Characteristics();

        /**
         * @brief Object constructor, initialises Characteristics via parameter.
         * Allows for all initial Characteristic properties to be set other than emotions.
         * @param [in] mood_in A long-term emotive state, influenced by the generation of new emotions.
         * @param [in] emotions_in A collection of short-term reactions to specific events and observations.
         * @param [in] traits_in A collection of modifiers for the emotive responses to specific Properties.
         * @param [in] personality_in A collection of modifiers for behaviour tendencies towards/against specific Actions.
         */
    explicit Characteristics(float mood_in,
                             std::initializer_list<EmotiveResponse> emotions_in = {},
                             std::initializer_list<std::pair<const npc::Properties, float>> traits_in = {},
                             std::initializer_list<std::pair<const npc::Actions, float>> personality_in = {});
};

    /**
     * @brief Keeps track of an NPC's current stage of its behaviour.
     * @warning This is only responsible for tracking, not performing, behaviour!
     */
struct BehaviourState {
        /// The current stage of the NPC's behaviour.
    npc::Stages current;

        /// The amount of time the NPC behaviour has been in its current behaviour state.
    double current_dt;

        /// The prior stage of the NPC's behaviour before it was updated.
    npc::Stages prior;

        /// The amount of time the NPC behaviour has been in its prior behaviour state.
    double prior_dt;

        /// An accumulated amount of time since the prior turnover of NPC emotions.
    double emotion_turnover_dt;

        /**
         * @brief Default object constructor, optionally initialises BehaviourState via parameters.
         * @param current_in The current stage of the NPC's behaviour.
         * @param prior_in The prior stage of the NPC's behaviour before it was updated.
         */
    explicit BehaviourState(npc::Stages current_in = npc::Stages::kIdle,
                            double current_dt_in = 0.0f,
                            double emotion_turnover_dt_in = 0.0f,
                            npc::Stages prior_in = npc::Stages::kIdle,
                            double prior_dt_in = 0.0f);
};

} // namespace component
