#include "ECS/Component/NPC.hpp"

namespace component {

Plan::Plan(npc::Actions action_in, entt::entity entity_in, int desire_in,
           std::initializer_list<int> goals_in, npc::Outcomes history_in)
        : action(action_in),
          entity(entity_in),
          desire(desire_in),
          goals(goals_in),
          history(history_in) {}

Goal::Goal(entt::entity entity_in, npc::Properties property_in,
           npc::Components element_in, float range_min_in,
           float range_max_in, npc::Conditions condition_in,
           npc::Outcomes history_in)
        : entity(entity_in),
          property(property_in),
          element(element_in),
          range_min(range_min_in),
          range_max(range_max_in),
          condition(condition_in),
          history(history_in) {}

Desire::Desire(int parent_in, std::initializer_list<Goal> goals_in,
               npc::Outcomes history_in)
        : parent(parent_in),
          goals(goals_in),
          history(history_in) {}

BDI::BDI(entt::entity entity_in,
         std::initializer_list<npc::Properties> beliefs_properties_in,
         std::initializer_list<npc::Actions> beliefs_affordances_in,
         std::initializer_list<std::pair<const int, Desire>> desires_in,
         std::initializer_list<std::pair<const int, std::set<Plan>>> intentions_in)
        : beliefs_properties({{entity_in, beliefs_properties_in}}),
          beliefs_affordances({{entity_in, beliefs_affordances_in}}),
          desires(desires_in),
          intentions(intentions_in) {
    // Guarantee an initial belief in the existence of the identified entity
    beliefs_properties[entity_in].insert(npc::Properties::kExists);
}

EmotiveResponse::EmotiveResponse(entt::entity entity_in, float emotion_in,
                                 SourceWhat&& what_in)
        : entity(entity_in),
          emotion(emotion_in),
          what(what_in) {}

Characteristics::Characteristics() : mood(0.0f) {}

Characteristics::Characteristics(float mood_in,
                                 std::initializer_list<EmotiveResponse> emotions_in,
                                 std::initializer_list<std::pair<const npc::Properties, float>> traits_in,
                                 std::initializer_list<std::pair<const npc::Actions, float>> personality_in)
        : mood(mood_in),
          emotions(emotions_in),
          traits(traits_in),
          personality(personality_in) {}

BehaviourState::BehaviourState(npc::Stages current_in, int steps_current_in,
                               int steps_accumulated_in, npc::Stages prior_in,
                               int steps_prior_in)
        : current(current_in),
          steps_current(steps_current_in),
          steps_accumulated(steps_accumulated_in),
          prior(prior_in),
          steps_prior(steps_prior_in) {}

} // namespace component
