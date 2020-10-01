#pragma once

#include <set>

#include <entt/entt.hpp>
#include "DataStructures/NPC/AI.hpp"

namespace component {
    struct Plan {
            /**
             * @brief The action that the NPC has planned to perform.
             * @warning Not guaranteed to be possible for the NPC to perform!
             */
        npc::Actions action_;

            /**
             * @brief The identifier of the entity the NPC has planned to target.
             * @note Can be used to set the NPC itself as its target.
             * @warning Not guaranteed to be a valid entity identifier!
             */
        entt::entity entity_;

            /**
             * @brief The identifier of the desire that the NPC is planning to resolve.
             * @warning This should NEVER be 0, that identifier is reserved for the desire root.
             */
        int desire_;

            /**
             * @brief The identifier of the goals that the NPC is planning to resolve.
             * @note Can include 0 due to its referential nature as an iterator.
             */
        std::set<int> goals_;

            /**
             * @brief The previous outcome of attempting this plan.
             * @note Assumes that kUnknown indicates no prior attempts have been made.
             */
        npc::Outcomes history_;

            /**
             * @brief Object constructor, initializes contents of a new Plan.
             * Additionally sets history_ to npc::Outcomes::kUnknown.
             * @param action The action that the NPC has planned to perform.
             * @param entity The identifier of the entity the NPC has planned to target.
             * @param desire The identifier of the desire that the NPC is planning to resolve.
             * @param goals The identifier of the goals that the NPC is planning to resolve.
             */
        Plan(npc::Actions action, entt::entity entity, int desire, std::set<int> goals);

            /// The default constructor is not permitted for use.
        Plan() = delete;
    };
}