
# Add source files.
target_sources(${PROJECT_NAME} PRIVATE
    #This thing goes brrrrr
    main.cpp

    #Engine
    Engine/Engine.cpp
    Engine/EulerCamera.cpp
    #--Renderer
    Engine/Renderer/OpenGL.cpp
    Engine/Renderer/Shader.cpp
    Engine/Renderer/Skybox.cpp
    #--SubModules
    Engine/SubModules/ModelManager.cpp
    Engine/SubModules/GUIManager.cpp
    #- InputManagement
    Engine/Input/InputManager.cpp
    Engine/Input/GLFWInputWrangler.cpp
    Engine/Input/RedEventManager.cpp

    #Game
    #--MainMenu
    Game/MainMenu/MainMenu.cpp
    Game/MainMenu/StaticModel.cpp
    #--Demo Scene
    Game/Demo/Demo.cpp

    #DataStructures
    #--Model
    DataStructures/Model/Animation.cpp
    DataStructures/Model/Mesh.cpp
    DataStructures/Model/Model.cpp
    DataStructures/Model/Animator.cpp

    #ECS
    ECS/Ecs.cpp
    ECS/Entity.cpp
    #--Components
    ECS/Component/Model.cpp
    #--Systems
        ECS/System/System.cpp
)

# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})