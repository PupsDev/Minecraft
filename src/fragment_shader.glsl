#version 330 core

in vec2 UV;
in float debug;

// Ouput data
out vec3 color;

uniform sampler2D myTextureSampler[5];
uniform float seaLevel;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
float clamp(float value, float min, float max){
        return value<min?min:value>max?max:value;
}

void main(){
        color = vec3(0.2, 0.2,0.9);

        float height = texture( myTextureSampler[0], UV).r;

        vec3 t1 =  texture( myTextureSampler[1], UV).rgb;
        vec3 t2 =  texture( myTextureSampler[2], UV).rgb;
        vec3 t3 =  texture( myTextureSampler[3], UV).rgb;

        if(height<seaLevel-0.001){
                height = height;
                if(height < 0.33f){
                        color = mix(t3,t2,clamp(map(height, 0.0, 0.3,-10,1),0,1));
                }else if(height < 0.66f){
                        color = mix(t2,t1,clamp(map(height, 0.3, 0.66,-10,1),0,1));;
                }else{
                        color = t1;       
                }
        }else{
                color = texture( myTextureSampler[4], UV).rgb; 
        }

        //color = vec3(seaLevel);

        //color =vec3(0.2, 0.2,0.4);
        //color = vec3(debug);

        //color = texture( myTextureSampler[1], UV).rgb;
        //color = vec3(UV,0.0);
        //color = vec3(1.0, 0.0, 0.0);

}
