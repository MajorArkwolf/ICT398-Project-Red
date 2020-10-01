#pragma once

#include <set>

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

            /**
             * @brief Object constructor, initializes Plan via parameter.
             *
             * Initialises history_ to npc::Outcomes::kUnknown.
             * @param action The action that the NPC has planned to perform.
             * @param entity The identifier of the target entity for the planned action.
             * @param desire The identifier of the Desire that the NPC has planned to resolve.
             * @param goals The identifier(s) of the Desire's Goal(s) that the NPC has planned to resolve.
             */
        Plan(npc::Actions action, entt::entity entity, int desire, std::set<int> goals);

            /// The default constructor is not permitted for use.
        Plan() = delete;
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
             * @brief The previous outcome of resolving this Goal.
             * @note Assumes that kUnknown indicates no prior attempts have been made.
             */
        npc::Outcomes history_;

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

            /// The default constructor is not permitted for use.
        Goal() = delete;
    };
}