class Chunk{
    public:
        int status; 

        // 0 : pas encore fabriqué
        // 1 : en cours de fabrication
        // 2 : fabriqué mais pas sur le gpu
        // 3 : tout bon

        thread * myOwnThread;

        int startX = 0;
        int startY = 0;

        vector<ivec3> cubes;
        vector<int> type;
        vector<int> visibility;

        map<int, map<int, map<int, int>>> bendel;

        Cube * cubeSample;

        Mesh gigaMesh;

        GLuint viewMatrix_uniform;
        GLuint projectionMatrix_uniform;
        GLuint viewPosUniform;


    Chunk(){
        status = 0;
    }


    void makeGigaMesh(){
        gigaMesh = Mesh();
        int ind = 0;
        for(int i = 0 ; i < cubes.size() ; i++){
            int p2 = 1;
            for(int j = 0 ; j< 6 ; j ++){
                if(visibility[i] & p2){
                    cubeSample->faces[j];

                    for(int k = 0 ; k < cubeSample->faces[j].indices.size() ; k ++){
                        gigaMesh.indices.push_back(cubeSample->faces[j].indices[k] + ind);
                    }

                    for(int k = 0 ; k < cubeSample->faces[j].indexed_vertices.size() ; k ++){
                        ind++;
                        vec3 pos = vec3(cubes[i][0], cubes[i][2], cubes[i][1]);  
                        vec2 uvShift = vec2(type[i]*1.0/5.0,0.0);
                        vec2 uv = cubeSample->faces[j].uvs[k];
                        uv.x = uv.x*0.2 + type[i]*0.2;

                        gigaMesh.indexed_vertices.push_back(cubeSample->faces[j].indexed_vertices[k]+pos);
                        gigaMesh.normals.push_back(cubeSample->faces[j].normals[k]);
                        gigaMesh.uvs.push_back(uv);
                    }
                }
                p2*=2;
            }
        }
    }

    //Chunk(const siv::PerlinNoise &perlin, int sx, int sy, Cube * c, GLuint programID, GLuint texture){
    void generate(const siv::PerlinNoise &perlin, int sx, int sy, Cube * c){
        cubeSample = c;

        //cout<<"GENERATING !!"<<endl;
        startX = sx;
        startY = sy;

        
        int i = 0;
        int levelDivisor = 100;
        
        for(int x = startX ; x < startX+16 ; x++){
            for(int y = startY ; y < startY+16 ; y ++){
                const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 4);
                //std::cout << noise << '\t';
                double seaLevel = 0.25;
                int z = (noise>seaLevel?noise:seaLevel)*100-seaLevel*100+5;
                //std::cout << z << '\t';
                
                int baseType = 
                    noise<seaLevel?4:
                    noise<0.3?2:
                    noise<0.7?0:
                    3;

                //cout<<z<<endl;
                z/=levelDivisor -1;
                for(int k = 0 ; k <= z ; k ++){
                    int realType =
                        k == z ? baseType :
                        baseType == 0 && k > z-3 ? 1 :
                        baseType == 2 && k > z-3 ? 2 :
                        baseType == 4 ? 4:
                        3;
                    ivec3 pos = vec3(x,y,k);
                    cubes.push_back(pos);
                    type.push_back(realType);
                    bendel[pos[0]][pos[1]][pos[2]] = i++;
                };                
            }
        }

        for(int j = 0 ; j < cubes.size();j++){
            int tmpVis = 63;
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
            if(
                bendel.find(cubes[j][0]) != bendel.end() &&
                bendel[cubes[j][0]].find(cubes[j][1]+1) != bendel[cubes[j][0]].end() &&
                bendel[cubes[j][0]][cubes[j][1]+1].find(cubes[j][2])!=bendel[cubes[j][0]][cubes[j][1]+1].end()){
                tmpVis = tmpVis & (~8);
            }
            if(
                bendel.find(cubes[j][0]) != bendel.end() &&
                bendel[cubes[j][0]].find(cubes[j][1]-1) != bendel[cubes[j][0]].end() &&
                bendel[cubes[j][0]][cubes[j][1]-1].find(cubes[j][2])!=bendel[cubes[j][0]][cubes[j][1]-1].end()){
                tmpVis = tmpVis & (~4);
            }
            if(
                bendel.find(cubes[j][0]) != bendel.end() &&
                bendel[cubes[j][0]].find(cubes[j][1]) != bendel[cubes[j][0]].end() &&
                bendel[cubes[j][0]][cubes[j][1]].find(cubes[j][2]+1)!=bendel[cubes[j][0]][cubes[j][1]].end()){
                tmpVis = tmpVis & (~32);
            }
            if(
                bendel.find(cubes[j][0]) != bendel.end() &&
                bendel[cubes[j][0]].find(cubes[j][1]) != bendel[cubes[j][0]].end() &&
                bendel[cubes[j][0]][cubes[j][1]].find(cubes[j][2]-1)!=bendel[cubes[j][0]][cubes[j][1]].end()){
                tmpVis = tmpVis & (~2);
            }

            visibility.push_back(tmpVis);
        }

        makeGigaMesh();

        status = 2;
    }

    void loadOnGpu(GLuint programID, GLuint texture){
        glUseProgram(programID);

        gigaMesh.texture = texture;
        gigaMesh.loadOnGpu(programID);

		viewMatrix_uniform = glGetUniformLocation(programID,"view");
		projectionMatrix_uniform = glGetUniformLocation(programID,"projection");
		viewPosUniform = glGetUniformLocation(programID , "viewPos");

        //cout<<"COUCOU"<<endl;
        status = 3;
    }

    void draw(Camera camera, GLuint programID){

        glUseProgram(programID);

		glUniformMatrix4fv(viewMatrix_uniform       , 1, false, glm::value_ptr(camera.viewMatrix));
        glUniformMatrix4fv(projectionMatrix_uniform , 1, false, glm::value_ptr(camera.projectionMatrix));
		glUniform3fv(viewPosUniform, 1, &camera.position[0]);
        
        Transform tmp = Transform();
        gigaMesh.draw(tmp);
    }
};

void constructChunk(Chunk * self, const siv::PerlinNoise &perlin, int sx, int sy, Cube * c, int * nbThread){
    (*nbThread)++;
    self->generate(perlin, sx, sy, c);
    (*nbThread)--;
}