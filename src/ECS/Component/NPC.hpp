#pragma once

#include <map>
#include <set>
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
             *
             * Initialises history_ to npc::Outcomes::kUnknown.
             * @param action The action that the NPC has planned to perform.
             * @param entity The identifier of the target entity for the planned action.
             * @param desire The identifier of the Desire that the NPC has planned to resolve.
             * @param goals The identifier(s) of the Desire's Goal(s) that the NPC has planned to resolve.
             */
        Plan(npc::Actions action, entt::entity entity, int desire, std::initializer_list<int> goals);
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
             *
             * Initialises history_ to npc::Outcomes::kUnknown.
             * @param entity The identifier of the target entity to check.
             * @param property The identifier of the target entity's property to check.
             * @param component The identifier of the target property's component to check.
             * @param range_min The lower bounds of the conditional range.
             * @param range_max The upper bounds of the conditional range.
             * @param condition The identifier of the Goal's conditional requirement.
             */
        Goal(entt::entity entity, npc::Properties property, npc::Components component,
             float range_min, float range_max, npc::Conditions condition);
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
             *
             * Initialises history_ to npc::Outcomes::kUnknown.
             * @param parent The identifier of the parent Desire.
             * @param goals The Desire's collection of conditional requirements.
             */
        Desire(int parent, std::initializer_list<Goal> goals);
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
             * @brief Object constructor, initialises BDI to default values.
             * Generates an NPC's BDI with no initial knowledge.
             */
        BDI();

            /**
             * @brief Object constructor, initializes BDI via parameter.
             * Establishes an NPC's BDI with an initial knowledge of itself.
             * @note Always generates npc::Beliefs::kExists for itself, even if not provided.
             * @param self_identifier The identifier of the NPC.
             * @param beliefs_properties_self The NPC's properties that it believes it initially knows about.
             * @param beliefs_affordances_self The NPC's actions that it believes it can initially perform by itself.
             */
        BDI(entt::entity self_identifier,
            std::initializer_list<npc::Properties> beliefs_properties_self = {},
            std::initializer_list<npc::Actions> beliefs_affordances_self = {});
    };

        /// An emotional response to a specific NPC perception, construct, or circumstance.
    struct EmotiveResponse {
            /**
             * @brief The identifier of the target entity to check.
             * @note Can be used to set the NPC itself as its target.
             */
        entt::entity entity_;

            /**
             * @brief The emotional response to the NPC perception, construct, or circumstance.
             * Higher values are positive, lower values are negative.
             * @note Values are expected to be within a range of 1.0f and -1.0f.
             */
        float emotion_;

            /**
             * @brief The source of the emotional response.
             * @warning Requires use of std::holds_alternative and std::visit to test & access!
             */
        std::variant<std::monostate, std::tuple<npc::Properties, npc::Components>,
                     std::tuple<int, Plan>, std::tuple<int, int, Goal>,
                     std::tuple<int, Desire>, npc::Events> what_;
    };
}
