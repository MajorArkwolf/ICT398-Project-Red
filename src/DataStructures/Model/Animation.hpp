#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <string>
#include <vector>
/// Holds the relevent information about transforms
struct AnimJointNode {
    unsigned num_pos_keys = 0;
    unsigned num_rot_keys = 0;
    //unsigned numScaleKeys = 0;
    std::vector<std::pair<double, glm::vec3>> pos_key = {};
    std::vector<std::pair<double, glm::quat>> rot_key = {};
    //std::vector<std::pair<double, glm::vec3>> scaleKey = {};
};
/**
 * Information about a given animation.
 */
class Animation {
  public:
    /**
     * Default constructor
     */
    Animation() = default;

    /**
     * Constructor if no name is given.
     * @param new_duration duration_ of the animation
     * @param TPS ticks per second
     */
    Animation(double new_duration, double TPS);

    /**
     * Main constructor.
     * @param new_duration duration_ of the animation
     * @param TPS ticks per second
     */
    Animation(const std::string& new_name, double new_duration, double TPS);

    /// The animation map of a model.
    std::map<std::string, AnimJointNode> anim_map_ = {};

    /**
     * Find the rotation
     * @param animation_time time completed
     * @param anim_joint animation node to check
     * @return the key value of the keyframe.
     */
    unsigned int FindRotation(double animation_time, const AnimJointNode *anim_joint);

    /**
     * Find the position
     * @param animation_time time completed
     * @param anim_joint animation node to check
     * @return the key value of the keyframe.
     */
    unsigned int FindPosition(double animation_time, const AnimJointNode *anim_joint);

    /**
     * Find the Scaling
     * @param animation_time time completed
     * @param anim_joint animation node to check
     * @return the key value of the keyframe.
     */
    unsigned int FindScaling(double animation_time, const AnimJointNode *anim_joint);

    /**
     * Find the animation of a given joint.
     * @param node_name name of the node.
     * @return the transforms of that node.
     */
    AnimJointNode *FindNodeAnim(const std::string &node_name);

    /**
     * Gets the ticks per second.
     * @return ticks per second.
     */
    double GetTicksPerSecond() const;

    /**
     * Get the duration_.
     * @return the ticks per second
     */
    double GetDuration() const;

    /**
     * Getter for name.
     * @return name of the string of animation
     */
    const std::string &GetName() const;

  private:
    /// Ticks per second.
    double ticks_per_second_ = 25.0;
    /// Duration of the animation
    double duration_ = 0.0;
    /// name of the animation.
    std::string name_ = "";
};
