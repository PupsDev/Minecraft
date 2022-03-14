#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normalIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D myTextureSampler[1];

uniform float seaLevel;


//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out float debug;
out vec3 normal;
out vec3 pos;


void main(){
        gl_Position = projection * view * model * vec4(vertices_position_modelspace,1) ;
        UV = vertexUV;
        normal = normalIn;
        pos = (model * vec4(vertices_position_modelspace,1)).xyz;
}       

