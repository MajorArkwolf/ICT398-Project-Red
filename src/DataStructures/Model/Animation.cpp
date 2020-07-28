#include "Animation.hpp"

Animation::Animation(double newDuration, double TPS){
    duration = newDuration;
    ticksPerSecond = TPS;
}

Animation::Animation(const std::string& newName, double newDuration, double TPS){
    name = newName;
    duration = newDuration;
    ticksPerSecond = TPS;
}

AnimJointNode* Animation::FindNodeAnim(const std::string& NodeName)
{
    if (animMap.count(NodeName)) {
        return &animMap.at(NodeName);
    }
    return nullptr;
}

unsigned Animation::FindRotation(double AnimationTime, const AnimJointNode* pNodeAnim){
    for (unsigned i = 0 ; i < pNodeAnim->numRotKeys; ++i) {
        if (AnimationTime < pNodeAnim->rotKey[i + 1].first) {
            return i;
        }
    }
    //assert(0);
    return 0;
}

unsigned Animation::FindPosition(double AnimationTime, const AnimJointNode* pNodeAnim)
{
    for (unsigned i = 0 ; i < pNodeAnim->numPosKeys ; ++i) {
        if (AnimationTime < pNodeAnim->posKey[i + 1].first) {
            return i;
        }
    }
    //assert(0);
    return 0;
}

unsigned Animation::FindScaling(double AnimationTime, const AnimJointNode* pNodeAnim)
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

double Animation::getTicksPerSecond() const {
    return ticksPerSecond;
}

double Animation::getDuration() const {
    return duration;
}
const std::string &Animation::getName() const {
    return name;
}
