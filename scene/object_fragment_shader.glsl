#version 330 core

in vec2 UV;
in vec3 normal;
in vec3 pos;
//in float debug;

// Ouput data
out vec3 color;


uniform sampler2D myTextureSampler[1];
uniform vec3 viewPos;
//uniform float seaLevel;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
float clamp(float value, float min, float max){
        return value<min?min:value>max?max:value;
}

void main(){
        vec3 lightColor = vec3(0.9,0.9,0.9);
        vec3 lightDir   = normalize(vec3(0.5,1.0,0.2));
        vec3 objectColor = texture( myTextureSampler[0], UV).rgb;
        float specularStrength = 0.2;

        vec3 viewDir = normalize(viewPos - pos);
        vec3 reflectDir = reflect(-lightDir, normal); 

        float ambientStrength = 0.4;
        vec3 ambient = ambientStrength * lightColor;

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        

        vec3 result = (ambient + diffuse + specular) * objectColor;
        //vec3 result = (ambient + diffuse + specular) * vec3(0.5,0.5,0.5);
        
        //color = vec3(1.0, 0.0, 1.0);
        color = result;
        //color = normalize(viewPos);
        //color = normal;

        //color = vec3(1,0,0);
}
