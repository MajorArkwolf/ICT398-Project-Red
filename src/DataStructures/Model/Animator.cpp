#include "Animator.hpp"
#include "Engine/Engine.hpp"

void model::Animator::BoneTransform(double TimeInSeconds) {
    transforms.resize(100);
    animation_time += TimeInSeconds;
    if (animated_model != nullptr) {
        if (loaded_animation != nullptr) {
            if (animated_model->is_animated_) {
                double TicksPerSecond = loaded_animation->GetTicksPerSecond();
                double TimeInTicks    = animation_time * TicksPerSecond;
                if (end_when_completed) {
                    if (TimeInTicks >= loaded_animation->GetDuration()) {
                        clip_ended = true;
                        return;
                    }
                }
                glm::mat4 Identity(1.0f);

                double AnimationTime = fmod(TimeInTicks, loaded_animation->GetDuration());

                ReadNodeHeirarchy(AnimationTime, animated_model->root_joint_, Identity);
            }
        } else {
            for (unsigned i = 0; i < 100; i++) {
                transforms[i] = glm::mat4(1.0f);
            }
        }
    }
}
void model::Animator::ReadNodeHeirarchy(const double &AnimationTime, const JointsName& jN,
                                        const glm::mat4 &ParentTransform) {
    glm::mat4 NodeTransformation(jN.transform);

    const auto* pNodeAnim = loaded_animation->FindNodeAnim(jN.name);

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

    if (animated_model->bone_mapping_.find(jN.name) != animated_model->bone_mapping_.end()) {
        unsigned BoneIndex = animated_model->bone_mapping_[jN.name];
       transforms[BoneIndex] = animated_model->global_inverse_transform_ * GlobalTransformation *
                               animated_model->bone_info_[BoneIndex].bone_offset;
    }

    for (auto &child : jN.children) {
        ReadNodeHeirarchy(AnimationTime, child, GlobalTransformation);
    }
}

glm::quat model::Animator::CalcInterpolatedRotation(double AnimationTime, const AnimJointNode *pNodeAnim) {
    glm::quat Out;
    // we need at least two values to interpolate...
    if (pNodeAnim->num_rot_keys == 1) {
        Out = pNodeAnim->rot_key[0].second;
        return Out;
    }
    unsigned RotationIndex = loaded_animation->FindRotation(AnimationTime, pNodeAnim);
    unsigned NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->num_rot_keys);
    double DeltaTime = pNodeAnim->rot_key[NextRotationIndex].first - pNodeAnim->rot_key[RotationIndex].first;
    double Factor = (AnimationTime - pNodeAnim->rot_key[RotationIndex].first) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::quat& StartRotationQ = pNodeAnim->rot_key[RotationIndex].second;
        const glm::quat& EndRotationQ = pNodeAnim->rot_key[NextRotationIndex].second;
    Out = glm::slerp(StartRotationQ, EndRotationQ, static_cast<float>(Factor));
    Out = glm::normalize(Out);
    return Out;
}
glm::vec3 model::Animator::CalcInterpolatedPosition(double AnimationTime, const AnimJointNode *pNodeAnim) {
    glm::vec3 Out;
    if (pNodeAnim->num_pos_keys == 1) {
        Out = pNodeAnim->pos_key[0].second;
        return Out;
    }

    unsigned PositionIndex = loaded_animation->FindPosition(AnimationTime, pNodeAnim);
    unsigned NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->num_pos_keys);
    double DeltaTime = pNodeAnim->pos_key[NextPositionIndex].first - pNodeAnim->pos_key[PositionIndex].first;
    double Factor = (AnimationTime - pNodeAnim->pos_key[PositionIndex].first) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& Start = pNodeAnim->pos_key[PositionIndex].second;
    const glm::vec3& End = pNodeAnim->pos_key[NextPositionIndex].second;
    glm::vec3 Delta = End - Start;
    Out = Start + static_cast<float>(Factor) * Delta;
    return Out;
}

//ALL CHECKS WITH THE STRING MUST BE UPPERCASE
void model::Animator::LoadAnimation(const std::string& newAnim, bool endWhenCompletedFlag) {
    size_t index = 0;
    if (loaded_animation != nullptr) {
        index = loaded_animation->GetName().find(newAnim);
    } else {
        index = std::string::npos;
    }
    if (index == std::string::npos) {
        animation_time = 0.0;
        loaded_animation = animated_model->GetAnimation(newAnim);
        end_when_completed = endWhenCompletedFlag;
        clip_ended = false;
    }
}

void model::Animator::ResetAnimationTime() {
    animation_time = 0.0;
}
bool model::Animator::IsAnimationedEnded() const {
    return clip_ended;
}
void model::Animator::LinkToModel(size_t modelID) {
    animated_model = redengine::Engine::get().model_manager_.getModel(modelID);
}
