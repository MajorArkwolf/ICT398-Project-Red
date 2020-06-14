
# Add source files.
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp

    #Engine
    Engine/Engine.cpp
    Engine/EulerCamera.cpp
    #Engine Renderer
    Engine/Renderer/OpenGL.cpp
    Engine/Renderer/Shader.cpp
    Engine/Renderer/Skybox.cpp

    #Game
    Game/MainMenu/MainMenu.cpp
    Game/MainMenu/StaticModel.cpp

)

# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})