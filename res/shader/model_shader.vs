#version 410 core
const int MAX_JOINTS = 100;
const int MAX_WEIGHTS = 4;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in ivec4 aJointID;
layout (location = 6) in vec4 aJointWeights;

out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;

uniform bool isAnimated;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 jointTransforms[MAX_JOINTS];

void main()
{
	mat4 bone_transform = jointTransforms[aJointID[0]] * aJointWeights[0];
		bone_transform += jointTransforms[aJointID[1]] * aJointWeights[1];
		bone_transform += jointTransforms[aJointID[2]] * aJointWeights[2];
		bone_transform += jointTransforms[aJointID[3]] * aJointWeights[3];

    vec4 boned_position;
    if (isAnimated == true) {
	    boned_position = bone_transform * vec4(aPos, 1.0);
	} else {
	    boned_position = vec4(aPos, 1.0);
	}

    Normals = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * boned_position);
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * boned_position;

}