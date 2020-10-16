#include <entt/entt.hpp>

namespace System {

    /**
     * @brief Sets an Entity's NPC components to their default values.
     * Also generates any NPC components the Entity is missing.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCInit(entt::registry& registry, entt::entity& entity);

    /**
     * @brief Deletes all NPC components from an Entity.
     * Only deletes the NPC components, and does not delete the Entity itself.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCUnInit(entt::registry& registry, entt::entity& entity);

    /**
     * @brief Sets an Entity's NPC components to values from an external file.
     * Also generates any NPC components the Entity is missing.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     * @param [in] path The filesystem path to the target external file.
     */
void NPCImport(entt::registry& registry, entt::entity& entity, std::string path);

    /**
     * @brief Exports an Entity's NPC component values to an external file.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     * @param [in] path The filesystem path to the target external file.
     */
void NPCExport(entt::registry& registry, entt::entity& entity, std::string path);

    /**
     * @brief Updates an NPC's Beliefs and Desires from its perception of its environment.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCObserve(entt::registry& registry, entt::entity& entity);

    /**
     * @brief Updates an NPC's Intentions and Sub-Desires from its Beliefs and Desires.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCPrepare(entt::registry& registry, entt::entity& entity);

    /**
     * @brief Tests an NPC's Intentions and responds to those triggered with Actions.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCRespond(entt::registry& registry, entt::entity& entity);

    /**
     * @brief Keeps an NPC idle, relative to its characteristics.
     * An NPC with an apathetic mood will remain idle for longer.
     * @param [in,out] registry The target Registry.
     * @param [in,out] entity The target Entity.
     */
void NPCIdle(entt::registry& registry, entt::entity& entity);

    /**
     * @brief Updates all NPC Entity components and behaviours.
     * Automatically manages calls to AI inference, observation, and response systems.
     * @param [in,out] registry The target Registry.
     * @param [in] t The current time.
     * @param [in] dt The delta time from the previous call.
     */
void NPCsFixedUpdate(entt::registry& registry, double t, double dt);

    /**
     * @brief Initializes NPC system and component dependencies.
     * Ensures that any construction or destruction of NPC components remains in sync.
     * @note Only required to be called once, but should be safe to call repeatedly.
     */
void NPCSystemInit(entt::registry& registry);

} // namespace System