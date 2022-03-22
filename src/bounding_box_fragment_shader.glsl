#version 330 core

in vec2 UV;
in vec3 normal;
in vec3 pos;
//in float debug;

// Ouput data
out vec4 color;


void main(){       

        

        color = vec4(1.,0.,0.,0.5);

        color.a = 0.3;

}
