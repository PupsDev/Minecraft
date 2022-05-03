
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
    int renderDistance; //chunks

    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{ seed };

    GLuint programID;
    GLuint gigaTexture;

    int maxThread = 10;
    int nbThread = 0;
    bool drawn;

    vector<vector<glm::vec3>> imageMap;
  /*  Map()
    {

    }
*/
    Map( int size=100, int renderDistance =15){

        gigaTexture = loadBMP_custom("../textures/minecraft/gigaTexture.bmp",false);
        drawn = false;
        size=size;
        this->renderDistance = renderDistance;

        int caseSize = 1;
        int lineSize = 16*2*renderDistance;

        imageMap.resize(caseSize*lineSize);
        for(auto &line : imageMap)
        {
            line.resize(caseSize*lineSize);
        }
        
        //c_types = vector<Cube>(5,Cube());

        c = Cube();

        chunks = map<int, map<int, Chunk>>();
    }
    Map(GLuint GameObjectShader, int size=100, int renderDistance =15){

        gigaTexture = loadBMP_custom("../textures/minecraft/gigaTexture.bmp",false);
        drawn = false;
        size=size;
        this->renderDistance = renderDistance;

        programID = GameObjectShader;

        int caseSize = 1;
        int lineSize = 16*2*renderDistance;

        imageMap.resize(caseSize*lineSize);
        for(auto &line : imageMap)
        {
            line.resize(caseSize*lineSize);
        }
        
        //c_types = vector<Cube>(5,Cube());

        c = Cube();

        chunks = map<int, map<int, Chunk>>();
    }
    Map& operator=(const Map& map2)
    {
        gigaTexture = map2.gigaTexture;
        drawn = map2.drawn;
        size = map2.size;
        renderDistance = map2.renderDistance;
        programID = map2.programID;
        c = map2.c;
        //chunks = map2.chunks;
        for (auto const& [key, val] : map2.chunks)
        {
            chunks[key] = map<int, Chunk>();
            for (auto const& [key2, val2] : val)
            {
                chunks[key][key2] = val2; 
            }
        }

    }

    void draw(Camera camera){
        ivec2 playerChunk = vec2(camera.position[0]/16,camera.position[2]/16);
        //cout<<playerChunk[0]<<" "<<playerChunk[1]<<endl;
        //cout<<renderDistance<<endl;
        // 0-0
        //glUseProgram(programID);
		//camera.giveItToMe(programID);

        //cout<<"nbThread "<<nbThread<<endl;

        for(int x = playerChunk[0]-renderDistance ; x < playerChunk[0]+renderDistance ; x++ ){
            for(int y = playerChunk[1]-renderDistance ; y < playerChunk[1]+renderDistance ; y ++ ){
                int ix = x;//(x<0 ? x*-2 +1 : x*2);
                int iy = y;//(x<0 ? y*-2 +1 : y*2);

                if(chunks.find(ix) == chunks.end())
                    chunks[ix] = map<int, Chunk>();

                if(chunks[ix].find(iy) == chunks[ix].end()){
                    chunks[ix][iy] = Chunk();
                    chunks[ix][iy].worldPos = ivec2(ix,iy);
                }
                
                //camera.giveItToMe(programID);
                if(chunks[ix][iy].status == 0){
                    if(nbThread < maxThread){
                        //nbThread ++;
                        chunks[ix][iy].status = 1;
                        chunks[ix][iy].worldPos = ivec2(x*16,y*16);
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
                    chunks[ix][iy].drawn = false;
                    if(dist<64 || ang>cos(camera.fov)){
                        chunks[ix][iy].draw(camera, programID);
                        chunks[ix][iy].drawn = true;
                    }
                    int nx = x+renderDistance -  playerChunk[0];
                    int ny = y+renderDistance -  playerChunk[1];
                    for(int i=0 ; i< 16 ; i++)
                    {
                        for(int j=0 ; j<16 ; j++)
                        {
                            imageMap[nx*16+i][ny*16+j]=chunks[ix][iy].imageChunk[i][j];
                        }
                    }
                }
            }
        }
        drawn = true;
    }
    void drawMap() 
    {

        ofstream myfile;


        std::string output = "../map/map.ppm";
        myfile.open (&output[0]);
        int width = 320;
        int height = 320;
        myfile << "P3\n";
        myfile << width<<" "<<height<<"\n255\n";
        /*
        for (auto const& line : chunks)
        {
            for(auto const& chunk: line.second)
            {
                for(int i=0 ; i< 16 ; i++)
                {
                    for(int j=0 ; j<16 ; j++)
                    {
                        for(int k=0 ; k<3 ; k++)
                            myfile << chunk.second.imageChunk[i][j][k]<<" ";
                        myfile<<"\n";
                    }
                }
            }
        }
        */
        for (auto const& line : imageMap)
        {
            for(auto const& chunk: line)
            {
                for(int k=0 ; k<3 ; k++)
                    myfile << chunk[k]<<" ";
                myfile<<"\n";
            }
        }

        myfile.close();
    }

};