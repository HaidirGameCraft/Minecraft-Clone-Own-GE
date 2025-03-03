#version 320 es
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 uvs;

uniform mat4 projection_matrix;
uniform mat4 world_matrix;
uniform mat4 modelView_matrix;

out vec3 Fragnormal;
out mat4 model_matrix;
out vec4 FragVertices;
out vec2 FragTexCoords;

void main(void)
{
    gl_Position = projection_matrix * world_matrix * modelView_matrix * position;

    model_matrix = modelView_matrix;
    Fragnormal = normals;
    FragVertices = modelView_matrix * position;
    FragTexCoords = uvs;
}