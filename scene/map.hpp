
# include <cassert>
# include <iostream>
# include <fstream>
# include <sstream>
# include <unordered_map>
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

    int nbBlockTypes = 5;

    vector<vector<glm::vec3>> imageMap;

    Voronoi voro;
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


    void draw(Camera* camera){
        ivec2 playerChunk = vec2(camera->position[0]/16,camera->position[2]/16);
        //cout<<playerChunk[0]<<" "<<playerChunk[1]<<endl;
        //cout<<renderDistance<<endl;
        // 0-0
        //glUseProgram(programID);
		//camera->giveItToMe(programID);

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
                
                //camera->giveItToMe(programID);
                if(chunks[ix][iy].status == 0){
                    if(nbThread < maxThread){
                        //nbThread ++;
                        chunks[ix][iy].status = 1;
                        chunks[ix][iy].worldPos = ivec2(x*16,y*16);

                        chunks[ix][iy].typeMap = voro.idImage;
                        chunks[ix][iy].heightMap = voro.heightMap;

                        chunks[ix][iy].myOwnThread = new thread(constructChunk, &chunks[ix][iy], perlin, x*16, y*16, &c, &nbThread);
                        
                        
                        
                    }
                }
                if(chunks[ix][iy].status == 2){
                    //Je me met à jours avec les copains déja là
                    chunks[ix][iy].updateGigaMesh(&chunks[x+1][iy]);
                    chunks[ix][iy].updateGigaMesh(&chunks[x-1][iy]);
                    chunks[ix][iy].updateGigaMesh(&chunks[ix][y+1]);
                    chunks[ix][iy].updateGigaMesh(&chunks[ix][y-1]);
                    //Je met à jours les copains qui étaient là avant moi, avec moi
                    chunks[x+1][iy].updateGigaMesh(&chunks[ix][iy]);
                    chunks[x-1][iy].updateGigaMesh(&chunks[ix][iy]);
                    chunks[ix][y+1].updateGigaMesh(&chunks[ix][iy]);
                    chunks[ix][y-1].updateGigaMesh(&chunks[ix][iy]);

                    chunks[ix][iy].loadOnGpu(programID, gigaTexture);
                }
                if(chunks[ix][iy].status == 3){
                    vec3 pos = vec3(chunks[ix][iy].startX+8,0,chunks[ix][iy].startY+8);
                    float dist = (length(pos-camera->position));
                    float ang = dot(pos-camera->position,camera->direction) / ((dist)*length(camera->direction));
                    chunks[ix][iy].drawn = false;

                    if(dist<64 || ang>cos(camera->fov)){
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
    Chunk* getChunk(int ix, int iy)
    {
            auto chunkX = chunks.find(ix);
            if( chunkX != chunks.end())
            {
                    auto chunkY = chunkX->second.find(iy);
                    if( chunkY != chunkX->second.end())
                    {
                        return &chunkY->second;   
                    }
            }
            return NULL;
    }
    std::vector<pair<int,int>> findChunks(glm::vec3 pos)
    {
        

        int x = mapToChunkId((int)pos[0],1);
        int y = mapToChunkId((int)pos[2],1);
        int renderDistance =2;
        std::vector<pair<int,int>> coord;

        ivec2 playerChunk = vec2(pos[0]/16,pos[2]/16);

        for(int x = playerChunk[0]-renderDistance; x < playerChunk[0]+renderDistance ; x++ ){
            for(int y = playerChunk[1]-renderDistance ; y < playerChunk[1]+renderDistance; y ++ ){
                int ix = x;//(x<0 ? x*-2 +1 : x*2);
                int iy = y;//(x<0 ? y*-2 +1 : y*2);
        
                auto chunkX = chunks.find(ix);
                if( chunkX != chunks.end())
                {
                        auto chunkY = chunkX->second.find(iy);
                        if( chunkY != chunkX->second.end())
                        {

                            coord.push_back(make_pair(ix,iy));
                            
                            //chunkY->second.gigaObject.vis = 0;     
                        }
                }
            }
        }

        return coord;
    }
        int mapToChunkId(int x, int renderDistance)
    {
        return (renderDistance*15 +x)/(renderDistance*15)-1;
    }
    int findHighest(Chunk *chonky,  ivec2 pos)
    {
        int hauteurMax = 200;
        std::map<int, std::map<int, std::map<int, int>>> bendel;
        int ret =-1;
        auto cubeX = chonky->bendel.find(pos[0]);
            if( cubeX != chonky->bendel.end())
            {
                    auto cubeY = cubeX->second.find(pos[1]);
                    if( cubeY != cubeX->second.end())
                    {
                        for( int z = hauteurMax;z>=0;z-- )
                        {
                                auto cubeZ = cubeY->second.find(z);
                            if( cubeZ != cubeY->second.end())
                            {
                                //cout<<"Z = "<<z<<endl;
                                ret = z;
                                
                                return ret;
                            }


                        }
                    }
                    
                            
            }
            
                            
            return ret;
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