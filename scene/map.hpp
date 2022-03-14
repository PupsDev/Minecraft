
# include <cassert>
# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include "scene/PerlinNoise.hpp"


bool ivec3egale(const ivec3& a, const ivec3& b){
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

class Chunk{
    public:
        int startX = 0;
        int startY = 0;

        vector<ivec3> cubes;
        vector<int> type;
        vector<int> visibility;

        map<int, map<int, map<int, int>>> bendel;

    Chunk(const siv::PerlinNoise &perlin, int sx, int sy){
        //cout<<"GENERATING !!"<<BENDEL;
        startX = sx;
        startY = sy;

        int i = 0;

        for(int x = startX ; x < startX+16 ; x++){
            for(int y = startY ; y < startY+16 ; y ++){
                const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
                //std::cout << noise << '\t';
                double seaLevel = 0.25;

                for(int k = 0 ; k < 2 ; k ++){
                    ivec3 pos = vec3(x,y,(noise>seaLevel?noise:seaLevel)*100-100-k);
                    cubes.push_back(pos);
                    type.push_back(noise<seaLevel?4:noise<0.3?3:noise<0.7?0:2);
                    bendel[pos[0]][pos[1]][pos[2]] = i++;
                }

                
                // pos = vec3(x,y,(noise>seaLevel?noise:seaLevel)*100-100-1);
                // cubes.push_back(pos);
                // type.push_back(noise<seaLevel?4:noise<0.3?3:noise<0.7?0:2);

                
                // bendel[pos[0]][pos[1]][pos[2]] = i++;                
            }
        }

        for(int j = 0 ; j < cubes.size();j++){
            int tmpVis = 63;
            //for(int i = 0 ; i < cubes.size() ; i++){
                //if(bendel.find(cubes[j][0]-1) && bendel[cubes[j][0]-1].find(cubes[j][1]) && bendel[cubes[j][0]-1][cubes[j][1]].find(cubes[j][2]))
                //if(ivec3egale(cubes[bendel[(cubes[j][0]-1)][cube[j][1]][cubes[j][2]]], cubes[j] + ivec3(-1,0,0))){ 
                if(
                    bendel.find(cubes[j][0]-1) != bendel.end() &&
                    bendel[cubes[j][0]-1].find(cubes[j][1]) != bendel[cubes[j][0]-1].end() &&
                    bendel[cubes[j][0]-1][cubes[j][1]].find(cubes[j][2])!=bendel[cubes[j][0]-1][cubes[j][1]].end()){
                    tmpVis = tmpVis & (~1);
                }

                if(
                    bendel.find(cubes[j][0]+1) != bendel.end() &&
                    bendel[cubes[j][0]+1].find(cubes[j][1]) != bendel[cubes[j][0]+1].end() &&
                    bendel[cubes[j][0]+1][cubes[j][1]].find(cubes[j][2])!=bendel[cubes[j][0]+1][cubes[j][1]].end()){
                    tmpVis = tmpVis & (~16);
                }

                // if(ivec3egale(cubes[i], cubes[j] + ivec3(1,0,0))){
                //     tmpVis = tmpVis & (~16);
                // }

                if(
                    bendel.find(cubes[j][0]) != bendel.end() &&
                    bendel[cubes[j][0]].find(cubes[j][1]+1) != bendel[cubes[j][0]].end() &&
                    bendel[cubes[j][0]][cubes[j][1]+1].find(cubes[j][2])!=bendel[cubes[j][0]][cubes[j][1]+1].end()){
                    tmpVis = tmpVis & (~8);
                }

                

                // if(ivec3egale(cubes[i], cubes[j] + ivec3(0,1,0))){
                //     tmpVis = tmpVis & (~8);
                // }

                if(
                    bendel.find(cubes[j][0]) != bendel.end() &&
                    bendel[cubes[j][0]].find(cubes[j][1]-1) != bendel[cubes[j][0]].end() &&
                    bendel[cubes[j][0]][cubes[j][1]-1].find(cubes[j][2])!=bendel[cubes[j][0]][cubes[j][1]-1].end()){
                    tmpVis = tmpVis & (~4);
                }

                // if(ivec3egale(cubes[i], cubes[j] + ivec3(0,-1,0))){
                //     tmpVis = tmpVis & (~4);
                // }

                if(
                    bendel.find(cubes[j][0]) != bendel.end() &&
                    bendel[cubes[j][0]].find(cubes[j][1]) != bendel[cubes[j][0]].end() &&
                    bendel[cubes[j][0]][cubes[j][1]].find(cubes[j][2]+1)!=bendel[cubes[j][0]][cubes[j][1]].end()){
                    tmpVis = tmpVis & (~32);
                }

                // if(ivec3egale(cubes[i], cubes[j] + ivec3(0,0,1))){
                //     tmpVis = tmpVis & (~32);
                // }

                if(
                    bendel.find(cubes[j][0]) != bendel.end() &&
                    bendel[cubes[j][0]].find(cubes[j][1]) != bendel[cubes[j][0]].end() &&
                    bendel[cubes[j][0]][cubes[j][1]].find(cubes[j][2]-1)!=bendel[cubes[j][0]][cubes[j][1]].end()){
                    tmpVis = tmpVis & (~2);
                }

                // if(ivec3egale(cubes[i], cubes[j] + ivec3(0,0,-1))){
                //     tmpVis = tmpVis & (~2);
                // }
            //}

            visibility.push_back(tmpVis);
        }
    }

    Chunk(){

    }
};

class Map {
    public:
    vector<ivec3> cubes;
    //vector<vector<Chunk>> chunks;

    map<int, map<int, Chunk>> chunks;

    vector<Cube> c_types;

    int size = 100;
    int renderDistance = 3; //chunks

    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{ seed };

    GLuint programID;



    Map(GLuint GameObjectShader){
        programID = GameObjectShader;
        c_types = vector<Cube>(5,Cube());

        c_types[0].loadOnGpu(GameObjectShader); 
        c_types[1].loadOnGpu(GameObjectShader); 
        c_types[2].loadOnGpu(GameObjectShader); 
        c_types[3].loadOnGpu(GameObjectShader); 
        c_types[4].loadOnGpu(GameObjectShader); 

        c_types[0].setTexture("../textures/minecraft/grass.bmp");
        c_types[1].setTexture("../textures/minecraft/dirt.bmp");
        c_types[2].setTexture("../textures/minecraft/stone.bmp");
        c_types[3].setTexture("../textures/minecraft/sand.bmp");
        c_types[4].setTexture("../textures/minecraft/water.bmp");

        //chunks = vector<vector<Chunk>>(1,vector<Chunk>(1,Chunk()));
        chunks = map<int, map<int, Chunk>>();
    }

    void draw(Camera camera){
        ivec2 playerChunk = vec2(camera.position[0]/16,camera.position[2]/16);

        glUseProgram(programID);

		camera.giveItToMe(programID);

        for(int x = playerChunk[0]-renderDistance ; x < playerChunk[0]+renderDistance ; x++ ){
            for(int y = playerChunk[1]-renderDistance ; y < playerChunk[1]+renderDistance; y ++ ){
                int ix = (x<0 ? x*-2 +1 : x*2);
                int iy = (x<0 ? y*-2 +1 : y*2);

                if(chunks.find(ix) == chunks.end())
                    chunks[ix] = map<int, Chunk>();

                if(chunks[ix].find(iy) == chunks[ix].end()){
                    chunks[ix][iy] = Chunk(perlin, x*16, y*16);
                    //cout<<x<<" "<<y<<endl;
                }

                for(int i = 0 ; i < chunks[ix][iy].cubes.size() ; i++){
                    vec3 pos = vec3(chunks[ix][iy].cubes[i][0],chunks[ix][iy].cubes[i][2],chunks[ix][iy].cubes[i][1]);
                    float dist = (length(pos-camera.position));
                    float ang = dot(pos-camera.position,camera.direction) / ((dist)*length(camera.direction));

                    //if(dist < (renderDistance-1)*16 &&  ang>cos(camera.fov)){
                    if(ang>cos(camera.fov)){
                        c_types[chunks[ix][iy].type[i]].pos = pos;
                        c_types[chunks[ix][iy].type[i]].visibility = chunks[ix][iy].visibility[i];
                        c_types[chunks[ix][iy].type[i]].draw(camera);
                    }
                }
            }
        }
    }
};