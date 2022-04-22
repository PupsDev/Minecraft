#version 330 core

in vec2 UV;
in vec3 normal;
in vec3 pos;
//in float debug;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler[1];

void main(){       

        

        //color = vec4(1.,0.,0.,0.5);

        //color.a = 0.3;

        color = vec4(texture( myTextureSampler[0], UV).rgb,1);
        //color = vec4(vec2(UV.x, 1-UV.y),0,1);

}
