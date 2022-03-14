#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D myTextureSampler[5];

uniform float seaLevel;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out float debug;


void main(){

        // TODO : Output position of the vertex, in clip space : MVP * position
        // //gl_Position = projection * view * model * vec4(vertices_position_modelspace+ //vec3(0.,0.,height/10),1) ;
        // //gl_Position = projection * view * model * vec4(vertices_position_modelspace,1) ;

        // float height;

        // if(texture( myTextureSampler[0], vertexUV).r > seaLevel){
        //         height = seaLevel;      
        // }else{
        //         height = texture( myTextureSampler[0], vertexUV).r ;
        // }

        // //height = seaLevel;


        // //height = texture( myTextureSampler[0], vertexUV).r / 5.0f - 0.1;



        // //gl_Position = projection * view * model * vec4(vertices_position_modelspace+ vec3(0.,0.,height),1) ;
        // vec3 newpos = vertices_position_modelspace;
        // newpos.z += height;
        //gl_Position = projection * view * model * vec4(newpos,1) ;
        gl_Position = projection * view * model * vec4(vertices_position_modelspace,1) ;

        // UV = vertexUV;
        // debug = height;
}

