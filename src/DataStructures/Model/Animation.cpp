#include "Animation.hpp"

Animation::Animation(double new_duration, double TPS){
    duration_ = new_duration;
    ticks_per_second_ = TPS;
}

Animation::Animation(const std::string& new_name, double new_duration, double TPS){
    name_ = new_name;
    duration_ = new_duration;
    ticks_per_second_ = TPS;
}

AnimJointNode* Animation::FindNodeAnim(const std::string& node_name)
{
    if (anim_map_.count(node_name)) {
        return &anim_map_.at(node_name);
    }
    return nullptr;
}

unsigned Animation::FindRotation(double animation_time, const AnimJointNode* anim_joint){
    for (unsigned i = 0 ; i < anim_joint->num_rot_keys; ++i) {
        if (animation_time < anim_joint->rot_key[i + 1].first) {
            return i;
        }
    }
    //assert(0);
    return 0;
}

unsigned Animation::FindPosition(double animation_time, const AnimJointNode* anim_joint)
{
    for (unsigned i = 0 ; i < anim_joint->num_pos_keys ; ++i) {
        if (animation_time < anim_joint->pos_key[i + 1].first) {
            return i;
        }
    }
    //assert(0);
    return 0;
}

unsigned Animation::FindScaling(double animation_time, const AnimJointNode* anim_joint)
{
//    assert(pNodeAnim->mNumScalingKeys > 0);
//    for (unsigned i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
//        if (AnimationTime < pNodeAnim->mScalingKeys[i + 1].mTime) {
//            return i;
//        }
//    }
//    assert(0);
    return 0;
}

double Animation::GetTicksPerSecond() const {
    return ticks_per_second_;
}

double Animation::GetDuration() const {
    return duration_;
}
const std::string &Animation::GetName() const {
    return name_;
}
