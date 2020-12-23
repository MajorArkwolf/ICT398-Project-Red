#pragma once

#include "ECS/Entity.hpp"
#include "Engine/EulerCamera.hpp"

namespace demo {
class Player {
   public:
    /**
         * Default Constructor
         */
    Player();

    /**
         * Defaulted Destructor
         */
    ~Player() = default;

    /**
         * Sets the main player also known as the big player.
         * @param player the Entity of the main player.
         */
    void SetBigPlayer(Entity player);

    /**
         * Sets the little player in the world.
         * @param player the Entity of the little player.
         */
    void SetLittlePlayer(Entity player);

    /**
         * Returns the camera that is active based off  the entity the player is controlling.
         * @return the active camera by reference.
         */
    engine::Camera &GetActiveCamera();

    /**
         * Returns the current active player.
         * @return the entity object by reference.
         */
    Entity &GetActivePlayer();

    /**
         * Swaps the active player between the little and the big.
         */
    void TogglePlayer();

    /**
         * Updates the positions and sub functions.
         * @param t time since engine started.
         * @param dt time since last update.
         */
    void Update(double t, double dt);

    /**
         * Interface function that passes the input down into
         * @param forward
         * @param backward
         * @param left
         * @param right
         * @param delta_time
         */
    void ProcessKeyboardInput(bool forward, bool backward, bool left, bool right, double delta_time);

    void GrabObject(entt::entity grabbed_object);
    void ThrowObject(glm::vec3 force);
    void DropObject();
    void SetRegistry(entt::registry &the_registry);
    bool GetIsHoldingObject();

   private:
    /// The camera offset from the model.
    float little_player_offset_ = 2.8f;
    float big_player_offset_ = 150.0f;
    /// The models offset on the yaw rotation from the camera.
    float model_rotation_offset_ = 90.f;
    /// The big player entity
    Entity big_player_{};
    /// The little player entity
    Entity little_player_{};
    /// The flag to switch between players.
    bool big_player_active_ = true;

    /// Get the active offset of the camera to the model.
    float GetActiveOffset();

    //Is the player holding an object
    entt::entity grabbed_object_ = {};
    bool holding_object_ = false;
    entt::registry *registry_ = nullptr;
};
}// namespace demo
