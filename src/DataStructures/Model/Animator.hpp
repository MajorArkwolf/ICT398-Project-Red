#pragma once

#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "DataStructures/Model/Animation.hpp"
#include "DataStructures/Model/DataTypes.hpp"

namespace model {
    class Model;
    class Animator {
      public:
        /**
         * Default constructor
         */
        Animator() = default;
        /// animation that will be played.
        Animation* loaded_animation = nullptr;
        /// The transforms after they have been updated.
        std::vector<glm::mat4> transforms = {};
        /// Animated model.
        Model* animated_model = nullptr;

        /**
         * Function that will update bones based on time.
         * @param time_seconds Delta time variable should be passed in.
         */
        void BoneTransform(double time_seconds);

        /**
         * Find the node that will
         * @param animation_time animation time since start
         * @param jN joint being animated
         * @param parent_transform the current transform of the parent
         */
        void ReadNodeHeirarchy(const double &animation_time, const JointsName& jN,
                               const glm::mat4 &parent_transform);

        /**
         * Calculate the position between two points.
         * @param animation_time time passed between key 1 and key 2.
         * @param pNodeAnim next animation.
         * @return the resultant vector.
         */
        glm::vec3 CalcInterpolatedPosition(double animation_time, const AnimJointNode *pNodeAnim);

        /**
         * Calculate the position between two points.
         * @param animation_time time passed between key 1 and key 2.
         * @param pNodeAnim next animation.
         * @return the resultant quat.
         */
        glm::quat CalcInterpolatedRotation(double animation_time, const AnimJointNode *pNodeAnim);

        /**
         * Load animation
         * @param new_anim string name being loaded
         * @param end_when_completed_flag should the animation end of completion
         */
        void LoadAnimation(const std::string& new_anim, bool end_when_completed_flag = false);

        /**
         * load the model based of the models id.
         * @param model_ID model ID it should be linked to
         */
        void LinkToModel(size_t model_ID);

        /**
         * Reset the animation time back to zero.
         */
        void ResetAnimationTime();

        /**
         * Checks to see if an animation has ended
         * @return true if it has
         */
        bool IsAnimationedEnded() const;
      private:
        /// Animation time
        double animation_time = 0.0;
        /// Checks to see if the animation should loop or end
        bool end_when_completed = false;
        /// True if the clip has ended.
        bool clip_ended = false;
    };
}

