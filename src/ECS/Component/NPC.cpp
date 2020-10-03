#include "ECS/Component/NPC.hpp"

namespace component {

Plan::Plan(npc::Actions action, entt::entity entity, int desire,
           std::initializer_list<int> goals, npc::Outcomes history)
        : action_(action),
          entity_(entity),
          desire_(desire),
          goals_(goals),
          history_(history) {}

Goal::Goal(entt::entity entity, npc::Properties property,
           npc::Components component, float range_min,
           float range_max, npc::Conditions condition,
           npc::Outcomes history)
        : entity_(entity),
          property_(property),
          component_(component),
          range_min_(range_min),
          range_max_(range_max),
          condition_(condition),
          history_(history) {}

Desire::Desire(int parent, std::initializer_list<Goal> goals,
               npc::Outcomes history)
        : parent_(parent),
          goals_(goals),
          history_(history) {}

BDI::BDI(entt::entity entity,
         std::initializer_list<npc::Properties> beliefs_properties,
         std::initializer_list<npc::Actions> beliefs_affordances,
         std::initializer_list<std::pair<const int, Desire>> desires,
         std::initializer_list<std::pair<const int, std::set<Plan>>> intentions)
        : beliefs_properties_({{entity, beliefs_properties}}),
          beliefs_affordances_({{entity, beliefs_affordances}}),
          desires_(desires),
          intentions_(intentions) {
    // Guarantee an initial belief in the existence of the identified entity
    beliefs_properties_[entity].insert(npc::Properties::kExists);
}

EmotiveResponse::EmotiveResponse(entt::entity entity, float emotion,
                                 SourceWhat&& what)
        : entity_(entity),
          emotion_(emotion),
          what_(what) {}

Characteristics::Characteristics() : mood_(0.0f) {}

Characteristics::Characteristics(float mood,
                                 std::initializer_list<EmotiveResponse> emotions,
                                 std::initializer_list<std::pair<const npc::Properties, float>> traits,
                                 std::initializer_list<std::pair<const npc::Actions, float>> personality)
        : mood_(mood),
          emotions_(emotions),
          traits_(traits),
          personality_(personality) {}

} // namespace component
