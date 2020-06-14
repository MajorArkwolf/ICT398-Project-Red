#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <string>
#include <vector>
/// Holds the relevent information about transforms
struct AnimJointNode {
    unsigned numPosKeys = 0;
    unsigned numRotKeys = 0;
    //unsigned numScaleKeys = 0;
    std::vector<std::pair<double, glm::vec3>> posKey = {};
    std::vector<std::pair<double, glm::quat>> rotKey = {};
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
     * @param newDuration duration of the animation
     * @param TPS ticks per second
     */
    Animation(double newDuration, double TPS);

    /**
     * Main constructor.
     * @param newDuration duration of the animation
     * @param TPS ticks per second
     */
    Animation(const std::string& newName, double newDuration, double TPS);

    /// The animation map of a model.
    std::map<std::string, AnimJointNode> animMap = {};

    /**
     * Find the rotation
     * @param AnimationTime time completed
     * @param pNodeAnim animation node to check
     * @return the key value of the keyframe.
     */
    unsigned int FindRotation(double AnimationTime, const AnimJointNode *pNodeAnim);

    /**
     * Find the position
     * @param AnimationTime time completed
     * @param pNodeAnim animation node to check
     * @return the key value of the keyframe.
     */
    unsigned int FindPosition(double AnimationTime, const AnimJointNode *pNodeAnim);

    /**
     * Find the Scaling
     * @param AnimationTime time completed
     * @param pNodeAnim animation node to check
     * @return the key value of the keyframe.
     */
    unsigned int FindScaling(double AnimationTime, const AnimJointNode *pNodeAnim);

    /**
     * Find the animation of a given joint.
     * @param NodeName name of the node.
     * @return the transforms of that node.
     */
    AnimJointNode *FindNodeAnim(const std::string &NodeName);

    /**
     * Gets the ticks per second.
     * @return ticks per second.
     */
    double getTicksPerSecond() const;

    /**
     * Get the duration.
     * @return the ticks per second
     */
    double getDuration() const;

    /**
     * Getter for name.
     * @return name of the string of animation
     */
    const std::string &getName() const;

  private:
    /// Ticks per second.
    double ticksPerSecond = 25.0;
    /// Duration of the animation
    double duration = 0.0;
    /// name of the animation.
    std::string name = "";
};
