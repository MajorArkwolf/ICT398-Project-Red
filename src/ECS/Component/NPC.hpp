#pragma once

#include <set>

#include <entt/entt.hpp>
#include "DataStructures/NPC/Symbols.hpp"

namespace component {
        /**
         * @brief A declaration of an NPC's possible resolution for a Goal.
         * @note This is used as a component of the BDI's Intention.
         * @warning All identifiers stored must be verified before their use!
         */
    struct Plan {
            /**
             * @brief The action that the NPC has planned to perform.
             * @warning Not guaranteed to be possible for the NPC to perform!
             */
        npc::Actions action_;

            /**
             * @brief The identifier of the entity the NPC has targeted for its action.
             * @note Can be used to set the NPC itself as its target.
             */
        entt::entity entity_;

            /**
             * @brief The identifier of the Desire that the NPC is planning to resolve.
             * @warning This should NEVER be 0, that identifier is reserved for the desire root.
             */
        int desire_;

            /**
             * @brief The identifier of the Goals that the NPC is planning to resolve.
             * @note Can include 0 due to its referential nature as an iterator.
             */
        std::set<int> goals_;

            /**
             * @brief The previous outcome of attempting this Plan.
             * @note Assumes that kUnknown indicates no prior attempts have been made.
             */
        npc::Outcomes history_;

            /**
             * @brief Object constructor, initializes contents of a new Plan.
             * Additionally sets history_ to npc::Outcomes::kUnknown.
             * @param action The action that the NPC has planned to perform.
             * @param entity The identifier of the entity the NPC has targeted for its action.
             * @param desire The identifier of the Desire that the NPC is planning to resolve.
             * @param goals The identifier of the Goals that the NPC is planning to resolve.
             */
        Plan(npc::Actions action, entt::entity entity, int desire, std::set<int> goals);

            /// The default constructor is not permitted for use.
        Plan() = delete;
    };
}