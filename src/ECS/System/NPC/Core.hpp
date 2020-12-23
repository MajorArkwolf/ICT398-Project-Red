#include <entt/entt.hpp>

namespace System {

    /**
     * @author William Vlahos
     * @brief Sets an Entity's NPC components to values from an external file.
     * Also generates any NPC components the Entity is missing.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     * @param [in] path The filesystem path to the target external file.
     */
void NPCImport(entt::registry& registry, const entt::entity& entity, std::string path);

    /**
     * @brief Exports an Entity's NPC component values to an external file.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     * @param [in] path The filesystem path to the target external file.
     */
void NPCExport(entt::registry& registry, const entt::entity& entity, std::string path);

    /**
     * @brief Updates an NPC's Beliefs and Desires from its perception of its environment.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCObserve(entt::registry& registry, const entt::entity& entity);

    /**
     * @brief Updates an NPC's Intentions and Desires from its Beliefs and Desires.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCPrepare(entt::registry& registry, const entt::entity& entity);

    /**
     * @brief Tests an NPC's Intentions and responds to those triggered with Actions.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCRespond(entt::registry& registry, const entt::entity& entity);

    /**
     * @brief Keeps an NPC idle, relative to its characteristics.
     * An NPC with a neutral/apathetic mood will remain idle for longer.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCIdle(entt::registry& registry, const entt::entity& entity);

    /**
     * @brief Updates all NPC Entity components and behaviours.
     * Automatically manages calls to AI inference, observation, and response systems.
     * @param [in,out] registry The target Registry.
     * @param [in] t The current time.
     * @param [in] dt The delta time from the previous call.
     */
void NPCsUpdate(entt::registry& registry, double t, double dt);

    /**
     * @brief Initializes the EnTT configuration of NPC system and component dependencies.
     * This ensures that any construction or destruction of NPC components remains in sync.
     * @note Only required to be called once, but should be safe to call repeatedly.
     */
void NPCSystemInit(entt::registry& registry);

} // namespace System