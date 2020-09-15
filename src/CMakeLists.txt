
# Add source files.
target_sources(${PROJECT_NAME} PRIVATE
    #This thing goes brrrrrooooooooommmmmmmm
    main.cpp

    #Engine
    Engine/Engine.cpp
    Engine/EulerCamera.cpp
    #--Renderer
    Engine/Renderer/OpenGL.cpp
    Engine/Renderer/Shader.cpp
    Engine/Renderer/Skybox.cpp
    #--Input
    Engine/SubModules/Input/GLFWInputWrangler.cpp
    Engine/SubModules/Input/InputManager.cpp

    #--SubModules
    Engine/SubModules/ModelManager.cpp
    Engine/SubModules/GUIManager.cpp
    Engine/SubModules/JsonLoader.cpp
    Engine/SubModules/PrefabRepo.cpp
    Engine/SubModules/ShaderRepo.cpp
    Engine/SubModules/ConsoleLog.cpp
    #--Physics
    Engine/Physics/PhysicsEngine.cpp
    Engine/Physics/PhysicsWorld.cpp
    Engine/Physics/PhysicsListener.cpp
    Engine/Physics/CollisionDetection.cpp
    Engine/Physics/CollisionResolution.cpp
    Engine/Physics/PhysicsShape.cpp
    Engine/Physics/Logger.cpp

    #Game
    #--MainMenu
    Game/MainMenu/MainMenu.cpp
    #--Demo Scene
    Game/Demo/Demo.cpp
    #--Physics Demo Scene
        Game/PhysicsDemo/PhysicsDemo.cpp
    #--PrefabEditor
    Game/PrefabEditor/PrefabEditor.cpp

    #DataStructures
    #--Model
    DataStructures/Model/Animation.cpp
    DataStructures/Model/Mesh.cpp
    DataStructures/Model/Model.cpp
    DataStructures/Model/Animator.cpp

    #ECS
    ECS/ECS.cpp
    ECS/Entity.cpp
    #--Components
    ECS/Component/Model.cpp
    ECS/Component/Basic.cpp
    #--Systems
    ECS/System/System.cpp
)

# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
