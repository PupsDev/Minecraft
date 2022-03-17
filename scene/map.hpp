
# include <cassert>
# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
#include <thread> 

# include "scene/PerlinNoise.hpp"
# include "scene/Chunk.hpp"


bool ivec3egale(const ivec3& a, const ivec3& b){
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

class Map {
    public:
    vector<ivec3> cubes;
    //vector<vector<Chunk>> chunks;

    map<int, map<int, Chunk>> chunks;

    //vector<Cube> c_types;
    Cube c;

    int size = 100;
    int renderDistance = 15; //chunks

    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{ seed };

    GLuint programID;
    GLuint gigaTexture;

    int maxThread = 10;
    int nbThread = 0;

    Map(GLuint GameObjectShader, int size=100, int renderDistance =15){

        gigaTexture = loadBMP_custom("../textures/minecraft/gigaTexture.bmp",false);
        size=size;
        renderDistance = renderDistance;

        programID = GameObjectShader;
        
        //c_types = vector<Cube>(5,Cube());

        c = Cube();

        // c_types[0].loadOnGpu(GameObjectShader); 
        // c_types[1].loadOnGpu(GameObjectShader); 
        // c_types[2].loadOnGpu(GameObjectShader); 
        // c_types[3].loadOnGpu(GameObjectShader); 
        // c_types[4].loadOnGpu(GameObjectShader); 

        // c_types[0].setTexture("../textures/minecraft/grass.bmp");
        // c_types[1].setTexture("../textures/minecraft/dirt.bmp");
        // c_types[2].setTexture("../textures/minecraft/stone.bmp");
        // c_types[3].setTexture("../textures/minecraft/sand.bmp");
        // c_types[4].setTexture("../textures/minecraft/water.bmp");

        //chunks = vector<vector<Chunk>>(1,vector<Chunk>(1,Chunk()));
        chunks = map<int, map<int, Chunk>>();
    }

    void draw(Camera camera){
        ivec2 playerChunk = vec2(camera.position[0]/16,camera.position[2]/16);

        //glUseProgram(programID);
		//camera.giveItToMe(programID);

        //cout<<"nbThread "<<nbThread<<endl;

        for(int x = playerChunk[0]-renderDistance ; x < playerChunk[0]+renderDistance ; x++ ){
            for(int y = playerChunk[1]-renderDistance ; y < playerChunk[1]+renderDistance; y ++ ){
                int ix = (x<0 ? x*-2 +1 : x*2);
                int iy = (x<0 ? y*-2 +1 : y*2);

                if(chunks.find(ix) == chunks.end())
                    chunks[ix] = map<int, Chunk>();

                if(chunks[ix].find(iy) == chunks[ix].end()){
                    chunks[ix][iy] = Chunk();
                }
                
                //camera.giveItToMe(programID);
                if(chunks[ix][iy].status == 0){
                    if(nbThread < maxThread){
                        //nbThread ++;
                        chunks[ix][iy].status = 1;
                        chunks[ix][iy].myOwnThread = new thread(constructChunk, &chunks[ix][iy], perlin, x*16, y*16, &c, &nbThread);
                        
                        
                    }
                }
                if(chunks[ix][iy].status == 2){
                    chunks[ix][iy].loadOnGpu(programID, gigaTexture);
                    //nbThread --;
                }
                if(chunks[ix][iy].status == 3){
                    vec3 pos = vec3(chunks[ix][iy].startX+8,camera.position[1],chunks[ix][iy].startY+8);
                    float dist = (length(pos-camera.position));
                    float ang = dot(pos-camera.position,camera.direction) / ((dist)*length(camera.direction));
                    if(dist<64 || ang>cos(camera.fov)){
                        chunks[ix][iy].draw(camera, programID);
                    }
                }
            }
        }
    }
};