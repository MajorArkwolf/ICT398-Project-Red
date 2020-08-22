#include "Animator.hpp"
#include "Engine/Engine.hpp"

void model::Animator::BoneTransform(double TimeInSeconds) {
    transforms_.resize(100);
    animation_time_ += TimeInSeconds;
    if (animated_model_ != nullptr) {
        if (loaded_animation_ != nullptr) {
            if (animated_model_->is_animated_) {
                double ticks_per_second = loaded_animation_->GetTicksPerSecond();
                double time_in_ticks    = animation_time_ * ticks_per_second;
                if (end_when_completed_) {
                    if (time_in_ticks >= loaded_animation_->GetDuration()) {
                        clip_ended_ = true;
                        return;
                    }
                }
                glm::mat4 identity(1.0f);

                double animation_time = fmod(time_in_ticks, loaded_animation_->GetDuration());

                ReadNodeHeirarchy(animation_time, animated_model_->root_joint_, identity);
            }
        } else {
            for (unsigned i = 0; i < 100; i++) {
                transforms_[i] = glm::mat4(1.0f);
            }
        }
    }
}
void model::Animator::ReadNodeHeirarchy(const double &AnimationTime, const JointsName& jN,
                                        const glm::mat4 &ParentTransform) {
    glm::mat4 node_transformation(jN.transform);

    const auto* pNodeAnim = loaded_animation_->FindNodeAnim(jN.name);

    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
//        aiVector3D Scaling;
//        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
//        glm::mat4 ScalingM(1.0f);
//        ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

        // Interpolate rotation and generate rotation transformation matrix
        glm::mat4 rotation_m = glm::mat4(CalcInterpolatedRotation(AnimationTime, pNodeAnim));

        // Interpolate translation and generate translation transformation matrix
        //TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);
        glm::mat4 translation_m = glm::translate(glm::mat4(1.0f), CalcInterpolatedPosition(AnimationTime, pNodeAnim));

        // Combine the above transformations
        node_transformation = translation_m * rotation_m /** ScalingM*/;
    }

    glm::mat4 global_transformation = ParentTransform * node_transformation;

    if (animated_model_->bone_mapping_.find(jN.name) != animated_model_->bone_mapping_.end()) {
        unsigned bone_index = animated_model_->bone_mapping_[jN.name];
       transforms_[bone_index] = animated_model_->global_inverse_transform_ * global_transformation *
                               animated_model_->bone_info_[bone_index].bone_offset;
    }

    for (auto &child : jN.children) {
        ReadNodeHeirarchy(AnimationTime, child, global_transformation);
    }
}

glm::quat model::Animator::CalcInterpolatedRotation(double AnimationTime, const AnimJointNode *pNodeAnim) {
    glm::quat out;
    // we need at least two values to interpolate...
    if (pNodeAnim->num_rot_keys == 1) {
        out = pNodeAnim->rot_key[0].second;
        return out;
    }
    unsigned rotation_index = loaded_animation_->FindRotation(AnimationTime, pNodeAnim);
    unsigned new_rotation_index = (rotation_index + 1);
    assert(new_rotation_index < pNodeAnim->num_rot_keys);
    double delta_time = pNodeAnim->rot_key[new_rotation_index].first - pNodeAnim->rot_key[rotation_index].first;
    double factor = (AnimationTime - pNodeAnim->rot_key[rotation_index].first) / delta_time;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::quat& start_rotation_q = pNodeAnim->rot_key[rotation_index].second;
        const glm::quat& end_rotation_q = pNodeAnim->rot_key[new_rotation_index].second;
    out = glm::slerp(start_rotation_q, end_rotation_q, static_cast<float>(factor));
    out = glm::normalize(out);
    return out;
}
glm::vec3 model::Animator::CalcInterpolatedPosition(double AnimationTime, const AnimJointNode *pNodeAnim) {
    glm::vec3 out;
    if (pNodeAnim->num_pos_keys == 1) {
        out = pNodeAnim->pos_key[0].second;
        return out;
    }

    unsigned position_index = loaded_animation_->FindPosition(AnimationTime, pNodeAnim);
    unsigned next_position_index = (position_index + 1);
    assert(next_position_index < pNodeAnim->num_pos_keys);
    double delta_time = pNodeAnim->pos_key[next_position_index].first - pNodeAnim->pos_key[position_index].first;
    double factor = (AnimationTime - pNodeAnim->pos_key[position_index].first) / delta_time;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& start = pNodeAnim->pos_key[position_index].second;
    const glm::vec3& end = pNodeAnim->pos_key[next_position_index].second;
    glm::vec3 delta = end - start;
    out = start + static_cast<float>(factor) * delta;
    return out;
}

//ALL CHECKS WITH THE STRING MUST BE UPPERCASE
void model::Animator::LoadAnimation(const std::string& newAnim, bool endWhenCompletedFlag) {
    size_t index = 0;
    if (loaded_animation_ != nullptr) {
        index = loaded_animation_->GetName().find(newAnim);
    } else {
        index = std::string::npos;
    }
    if (index == std::string::npos) {
        animation_time_ = 0.0;
        loaded_animation_ = animated_model_->GetAnimation(newAnim);
        end_when_completed_ = endWhenCompletedFlag;
        clip_ended_ = false;
    }
}

void model::Animator::ResetAnimationTime() {
    animation_time_ = 0.0;
}
bool model::Animator::IsAnimationedEnded() const {
    return clip_ended_;
}
void model::Animator::LinkToModel(size_t modelID) {
    animated_model_ = redengine::Engine::get().model_manager_.getModel(modelID);
}
