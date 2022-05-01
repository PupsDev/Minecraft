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

        ivec2 worldPos;
        
        vector<bool> deletedCUbe;

        vector<ivec3> cubes;
        vector<int> type;
        vector<int> visibility;

        map<int, map<int, map<int, int>>> bendel;


        Cube * cubeSample;
        GameObject gigaObject;

        GLuint myProgId;
        GLuint viewMatrix_uniform;
        GLuint projectionMatrix_uniform;
        GLuint viewPosUniform;

        bool drawn;


    Chunk(){
        status = 0;
        gigaObject =  GameObject();
        gigaObject.mesh = Mesh();
        drawn = false;
    }


    void makeGigaMesh(){
        
        gigaObject.mesh.indices.clear();
        gigaObject.mesh.indexed_vertices.clear();
        gigaObject.mesh.normals.clear();
        gigaObject.mesh.uvs.clear();


        int ind = 0;
        for(int i = 0 ; i < cubes.size() ; i++)if(!deletedCUbe[i]){
            int p2 = 1;
            for(int j = 0 ; j< 6 ; j ++){
                if(visibility[i] & p2){
                    cubeSample->faces[j];

                    for(int k = 0 ; k < cubeSample->faces[j].indices.size() ; k ++){
                        gigaObject.mesh.indices.push_back(cubeSample->faces[j].indices[k] + ind);
                    }

                    for(int k = 0 ; k < cubeSample->faces[j].indexed_vertices.size() ; k ++){
                        ind++;
                        vec3 pos = vec3(cubes[i][0], cubes[i][2], cubes[i][1]);  
                        vec2 uvShift = vec2(type[i]*1.0/5.0,0.0);
                        vec2 uv = cubeSample->faces[j].uvs[k];
                        uv.x = uv.x*0.2 + type[i]*0.2;

                        gigaObject.mesh.indexed_vertices.push_back(cubeSample->faces[j].indexed_vertices[k]+pos);
                        gigaObject.mesh.normals.push_back(cubeSample->faces[j].normals[k]);
                        gigaObject.mesh.uvs.push_back(uv);
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
        int levelDivisor = 5;
        
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
                z= z /levelDivisor ;
                for(int k = 0 ; k <= z ; k ++){
                    int realType =
                        k == z ? baseType :
                        baseType == 0 && k > z-3 ? 1 :
                        baseType == 2 && k > z-3 ? 2 :
                        baseType == 4 ? 4:
                        3;
                    ivec3 pos = vec3(x,y,k);
                    deletedCUbe.push_back(false);
                    cubes.push_back(pos);
                    type.push_back(realType);
                    bendel[pos[0]][pos[1]][pos[2]] = i++;
                };                
            }
        }

        computeVisibility();
        makeGigaMesh();

        status = 2;
    }


    int getCube(ivec3 cubePos){
        if(
            bendel.find(cubePos[0]) != bendel.end() &&
            bendel[cubePos[0]].find(cubePos[1]) != bendel[cubePos[0]].end() &&
            bendel[cubePos[0]][cubePos[1]].find(cubePos[2])!=bendel[cubePos[0]][cubePos[1]].end()){
                return bendel[cubePos[0]][cubePos[1]][cubePos[2]];
        }else{
            return -1;
        }
    }


    void computeVisibility(){
        visibility.clear();
         for(int j = 0 ; j < cubes.size();j++){
            int tmpVis = 63;
            if(getCube(cubes[j]+ivec3(-1,0,0)) != -1){
                tmpVis = tmpVis & (~1);
            }
            if(getCube(cubes[j]+ivec3(1,0,0)) != -1){
                tmpVis = tmpVis & (~16);
            }
            if(getCube(cubes[j]+ivec3(0,1,0)) != -1){
                tmpVis = tmpVis & (~8);
            }
            if(getCube(cubes[j]+ivec3(0,-1,0)) != -1){
                tmpVis = tmpVis & (~4);
            }
            if(getCube(cubes[j]+ivec3(0,0,1)) != -1){
                tmpVis = tmpVis & (~32);
            }
            if(getCube(cubes[j]+ivec3(0,0,-1)) != -1){
                tmpVis = tmpVis & (~2);
            }
            visibility.push_back(tmpVis);
        }
    }

    void hideCube(ivec3 cubePos){
        if(status > 2){
            cout<<"hide cube : "<<cubePos<<endl;
            int cubeTohide = bendel[cubePos[0]][cubePos[1]][cubePos[2]];
            ///////////////
            
            // cubes.erase(cubes.begin()+cubeTohide);
            // type.erase(type.begin()+cubeTohide);
            // visibility.erase(visibility.begin()+cubeTohide);

            deletedCUbe[cubeTohide] = true;

            bendel[cubePos[0]][cubePos[1]].erase(cubePos[2]);
            

            ///////////////
            computeVisibility();
            makeGigaMesh();
            reloadOnGpu();
        }
    }

    void reloadOnGpu(){
        glUseProgram(myProgId);
        gigaObject.mesh.reloadOnGpu();
    }

    void loadOnGpu(GLuint programID, GLuint texture){
        myProgId    = programID;

        glUseProgram(programID);

        gigaObject.mesh.texture = texture;
        gigaObject.mesh.loadOnGpu(programID);

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
        
        Transform * tmp = new  Transform();
        gigaObject.apply(tmp);
        
         if(gigaObject.vis ==1)
        {
            gigaObject.draw(camera);
            drawn = true;


        }
        else
        {
             gigaObject.vis =1;
        }
        

    }
    bool intersect(float &t,glm::vec3 o, glm::vec3 d,ivec3 &cube)
    {
        std::vector<glm::vec3> normals;

        normals.push_back(glm::vec3(1.,0.,0.));
        normals.push_back(glm::vec3(-1.,0.,0.));

        normals.push_back(glm::vec3(0.,1.,0.));
        normals.push_back(glm::vec3(0.,-1.,0.));

        normals.push_back(glm::vec3(0.,0.,1.));
        normals.push_back(glm::vec3(0.,0.,-1.));

        float t_max = 1000.;
        bool ret = false;
        
        for(auto & pos : cubes)
        {
            for(auto &norm : normals)
            {
                float div =glm::dot(d,norm); 
                //cout<<div<<endl;
                if( div !=0)
                {
                   // cout<<"OUI"<<endl;
                    ret = true;
                    glm::vec3 point = glm::vec3(pos[0]+norm[0]/2,pos[1]+norm[1]/2,pos[2]+norm[2]/2);
                    glm::vec3 direc = glm::vec3(point-o);
                    float t2 = glm::dot( direc,norm) / div;

                    glm::vec3 intersection  = o + t2 * d;
                    float distance = length(intersection-point);

                    if(t2<t_max && distance < 0.5)
                    {
                        t_max = t2;
                        cube = pos;

                    }
                }
            }
        }
        t = t_max;
        hideCube(cube);
        return ret;

    }
};

void constructChunk(Chunk * self, const siv::PerlinNoise &perlin, int sx, int sy, Cube * c, int * nbThread){
    (*nbThread)++;
    self->generate(perlin, sx, sy, c);
    (*nbThread)--;
}