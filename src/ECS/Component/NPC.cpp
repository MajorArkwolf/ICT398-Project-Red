#include "ECS/Component/NPC.hpp"

component::Plan::Plan(npc::Actions action, entt::entity entity,
                      int desire, std::initializer_list<int> goals)
        : action_(action),
          entity_(entity),
          desire_(desire),
          goals_(goals),
          history_(npc::Outcomes::kUnknown) {}

component::Goal::Goal(entt::entity entity, npc::Properties property,
                      npc::Components component, float range_min,
                      float range_max, npc::Conditions condition)
        : entity_(entity),
          property_(property),
          component_(component),
          range_min_(range_min),
          range_max_(range_max),
          condition_(condition),
          history_(npc::Outcomes::kUnknown) {}

component::Desire::Desire(int parent, std::initializer_list<Goal> goals)
        : parent_(parent),
          goals_(goals),
          history_(npc::Outcomes::kUnknown) {}

component::BDI::BDI() {}

component::BDI::BDI(entt::entity entity,
                    std::initializer_list<npc::Properties> beliefs_properties,
                    std::initializer_list<npc::Actions> beliefs_affordances)
        : beliefs_properties_({{entity, beliefs_properties}}),
          beliefs_affordances_({{entity, beliefs_affordances}}) {
    // Guarantee an initial belief in the existence of the identified entity
    beliefs_properties_[entity].insert(npc::Properties::kExists);
}

component::EmotiveResponse::EmotiveResponse(entt::entity entity, float emotion,
                                            SourceWhat what)
        : entity_(entity),
          emotion_(emotion),
          what_(what) {}

component::Characteristics::Characteristics() : mood_(0.0f) {}

component::Characteristics::Characteristics(
            float mood,
            std::initializer_list<std::pair<const npc::Properties, float>> traits,
            std::initializer_list<std::pair<const npc::Actions, float>> personality)
        : mood_(mood),
          traits_(traits),
          personality_(personality) {}
