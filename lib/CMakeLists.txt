# SET(CMAKE_BUILD_TYPE "RelWithDebInfo")

# Used to remove warnings from lib
function(target_ignore_header_warnings target scope)
    get_target_property(lib_include_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${target} SYSTEM ${scope} ${lib_include_dirs})
endfunction()

# Library build options.
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(OPTION_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(RP3D_COMPILE_TESTBED OFF)
option(RP3D_COMPILE_TESTS OFF)

# Build libraries.
string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Disable compiler warnings.
add_compile_options(
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-everything>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-w>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
)
add_subdirectory(react)
add_subdirectory(glm)
add_subdirectory(assimp)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(stb_image)
add_subdirectory(entt)
add_subdirectory(json)
add_subdirectory(cpplocate)

# Puts a stop to the endlessly irritating practise postfixing 'd' onto debug libraries.
set_target_properties(reactphysics3d PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(reactphysics3d PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
set_target_properties(assimp PROPERTIES DEBUG_POSTFIX "")
set_target_properties(glad PROPERTIES DEBUG_POSTFIX "")
set_target_properties(glfw PROPERTIES DEBUG_POSTFIX "")
set_target_properties(reactphysics3d PROPERTIES DEBUG_POSTFIX "")


# Ignore warnings from library headers.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    glfw/include
    glfw/src
    assimp/include
    glad/include
    glad/src
    react/src
    stb_image/
    entt/src
    cpplocate/source
)

# Ignore header warnings in our libraries.
target_ignore_header_warnings(EnTT INTERFACE)
target_ignore_header_warnings(assimp INTERFACE)
target_ignore_header_warnings(reactphysics3d INTERFACE)
target_ignore_header_warnings(cpplocate INTERFACE)
target_ignore_header_warnings(glm INTERFACE)
target_ignore_header_warnings(glfw INTERFACE)
target_ignore_header_warnings(glad INTERFACE)
target_ignore_header_warnings(imgui INTERFACE)
target_ignore_header_warnings(stb_image INTERFACE)
target_ignore_header_warnings(cpplocate INTERFACE)
