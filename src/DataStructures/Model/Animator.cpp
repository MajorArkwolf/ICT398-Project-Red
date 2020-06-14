#include "Animator.hpp"
#include "Engine/Engine.hpp"

void Model::Animator::BoneTransform(double TimeInSeconds) {
    Transforms.resize(100);
    animationTime += TimeInSeconds;
    if (animatedModel != nullptr) {
        if (loadedAnimation != nullptr) {
            if (animatedModel->isAnimated) {
                double TicksPerSecond = loadedAnimation->getTicksPerSecond();
                double TimeInTicks    = animationTime * TicksPerSecond;
                if (endWhenCompleted) {
                    if (TimeInTicks >= loadedAnimation->getDuration()) {
                        clipEnded = true;
                        return;
                    }
                }
                glm::mat4 Identity(1.0f);

                double AnimationTime = fmod(TimeInTicks, loadedAnimation->getDuration());

                ReadNodeHeirarchy(AnimationTime, animatedModel->rootJoint, Identity);
            }
        } else {
            for (unsigned i = 0; i < 100; i++) {
                Transforms[i] = glm::mat4(1.0f);
            }
        }
    }
}
void Model::Animator::ReadNodeHeirarchy(const double &AnimationTime, const JointsName& jN,
                       const glm::mat4 &ParentTransform) {
    glm::mat4 NodeTransformation(jN.Transform);

    const auto* pNodeAnim = loadedAnimation->FindNodeAnim(jN.name);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
//        aiVector3D Scaling;
//        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
//        glm::mat4 ScalingM(1.0f);
//        ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

        // Interpolate rotation and generate rotation transformation matrix
        glm::mat4 RotationM = glm::mat4(CalcInterpolatedRotation(AnimationTime, pNodeAnim));

        // Interpolate translation and generate translation transformation matrix
        //TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);
        glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), CalcInterpolatedPosition(AnimationTime, pNodeAnim));

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM /** ScalingM*/;
    }

    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    if (animatedModel->boneMapping.find(jN.name) != animatedModel->boneMapping.end()) {
        unsigned BoneIndex = animatedModel->boneMapping[jN.name];
       Transforms[BoneIndex] = animatedModel->globalInverseTransform * GlobalTransformation *
                                                                 animatedModel->boneInfo[BoneIndex].BoneOffset;
    }

    for (auto &child : jN.children) {
        ReadNodeHeirarchy(AnimationTime, child, GlobalTransformation);
    }
}

glm::quat Model::Animator::CalcInterpolatedRotation(double AnimationTime, const AnimJointNode *pNodeAnim) {
    glm::quat Out;
    // we need at least two values to interpolate...
    if (pNodeAnim->numRotKeys == 1) {
        Out = pNodeAnim->rotKey[0].second;
        return Out;
    }
    unsigned RotationIndex = loadedAnimation->FindRotation(AnimationTime, pNodeAnim);
    unsigned NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->numRotKeys);
    double DeltaTime = pNodeAnim->rotKey[NextRotationIndex].first - pNodeAnim->rotKey[RotationIndex].first;
    double Factor = (AnimationTime - pNodeAnim->rotKey[RotationIndex].first) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::quat& StartRotationQ = pNodeAnim->rotKey[RotationIndex].second;
        const glm::quat& EndRotationQ = pNodeAnim->rotKey[NextRotationIndex].second;
    Out = glm::slerp(StartRotationQ, EndRotationQ, static_cast<float>(Factor));
    Out = glm::normalize(Out);
    return Out;
}
glm::vec3 Model::Animator::CalcInterpolatedPosition(double AnimationTime, const AnimJointNode *pNodeAnim) {
    glm::vec3 Out;
    if (pNodeAnim->numPosKeys == 1) {
        Out = pNodeAnim->posKey[0].second;
        return Out;
    }

    unsigned PositionIndex = loadedAnimation->FindPosition(AnimationTime, pNodeAnim);
    unsigned NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->numPosKeys);
    double DeltaTime = pNodeAnim->posKey[NextPositionIndex].first - pNodeAnim->posKey[PositionIndex].first;
    double Factor = (AnimationTime - pNodeAnim->posKey[PositionIndex].first) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& Start = pNodeAnim->posKey[PositionIndex].second;
    const glm::vec3& End = pNodeAnim->posKey[NextPositionIndex].second;
    glm::vec3 Delta = End - Start;
    Out = Start + static_cast<float>(Factor) * Delta;
    return Out;
}

//ALL CHECKS WITH THE STRING MUST BE UPPERCASE
void Model::Animator::LoadAnimation(const std::string& newAnim, bool endWhenCompletedFlag) {
    size_t index = 0;
    if (loadedAnimation != nullptr) {
        index = loadedAnimation->getName().find(newAnim);
    } else {
        index = std::string::npos;
    }
    if (index == std::string::npos) {
        animationTime = 0.0;
        loadedAnimation = animatedModel->getAnimation(newAnim);
        endWhenCompleted = endWhenCompletedFlag;
        clipEnded = false;
    }
}

void Model::Animator::ResetAnimationTime() {
    animationTime = 0.0;
}
bool Model::Animator::IsAnimationedEnded() const {
    return clipEnded;
}
void Model::Animator::LinkToModel(size_t modelID) {
    animatedModel = RedEngine::Engine::get().modelManager.getModel(modelID);
}
