#version 410 core
// Uniform variables
uniform bool isGlobalVertexColorEnabled;          // True if we need to use the global vertex color
uniform vec4 globalVertexColor;                   // Vertex color

// In variables
in vec4 vertexColorOut;

// Out variables
out vec4 FragColor;                        // Output color

void main() {

    // Compute the final color
    if (isGlobalVertexColorEnabled) {
        FragColor = globalVertexColor;
    }
    else {
        FragColor = vertexColorOut;
    }
}
