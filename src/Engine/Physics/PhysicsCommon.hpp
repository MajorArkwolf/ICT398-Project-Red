#pragma once
#include <reactphysics3d/reactphysics3d.h>

class PhysicsCommon{
private:
    PhysicsCommon() = default; // Disallow instantiation outside of the class.
public:
    PhysicsCommon(const PhysicsCommon&) = delete;
    PhysicsCommon& operator=(const PhysicsCommon &) = delete;
    PhysicsCommon(PhysicsCommon &&) = delete;
    PhysicsCommon & operator=(PhysicsCommon &&) = delete;

    reactphysics3d::PhysicsCommon physics_common_{};

    static auto& GetInstance(){
        static auto instance = PhysicsCommon{};
        return instance;
    }
};
