#include <map>
#include <time.h> 

class Scene
{
    public :
        vector<GameObject> objects;
        vector<SceneGraphComposite*> graphs;
        Camera camera;

        SceneGraphComposite* graphMonkey;
        SceneGraphComposite* graphMonkey2;

        glm::vec3 vitesse;
        float startFrame;
        float lastFrame;
        float deltaTime;
        bool changeDirection;

        int frameCount ;
        Scene()
        {
            GLuint BoxShader = LoadShaders( "bounding_box_vertex_shader.glsl", "bounding_box_fragment_shader.glsl" );
            Transform * translation = new Transform(glm::vec3(2.,20.,2.));
            Transform * translation2 = new Transform(glm::vec3(8.,20.,2.));
            translation->model = translation->getMat4();
            translation2->model = translation2->getMat4();

            vitesse = glm::vec3(0.,0.,0.);
            srand (time(NULL));
            frameCount =0;


            for(int i = 0 ; i < 40 ; i++)
            {
                GameObject * simpleMonkey = new GameObject();
                simpleMonkey->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
                simpleMonkey->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
                
                LoaderObj loader = LoaderObj("kevin.obj");

                simpleMonkey->mesh.indexed_vertices = loader.vertices;
                simpleMonkey->mesh.indices =loader.indices;
                //simpleMonkey->mesh.normals =loader.normals;
                simpleMonkey->mesh.uvs =loader.textures;
                simpleMonkey->mesh.loadTexture("dorian.bmp");


                simpleMonkey->loadOnGpu(BoxShader);
                 Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(2.,2.,2.)));
                scaling->model = scaling->getMat4();
                

                graphMonkey = new SceneGraphComposite();
                graphMonkey->gameObject = simpleMonkey;
                graphMonkey->apply(scaling);
                graphMonkey->setBoundingBox(BoxShader);


                simpleMonkey->physic->size = graphMonkey->BBsize;

                  Transform * translation3 = new Transform(glm::vec3( (float) i,10.,2.));
                translation3->model = translation3->getMat4();
                
               
                graphMonkey->apply(translation);
                graphMonkey->apply(translation3);

                add(graphMonkey);
            }


            GameObject * simpleMonkey2 = new GameObject();
            simpleMonkey2->physic = new PhysicComponent(glm::vec3(0.,0.,0.));
            simpleMonkey2->physic->vitesse = 0.01f*glm::vec3(-1.,0.,0.);
            
            simpleMonkey2->loadMesh("suzanne.off");
            simpleMonkey2->loadOnGpu(BoxShader);

            graphMonkey2 = new SceneGraphComposite();
            graphMonkey2->gameObject = simpleMonkey2;
            graphMonkey2->setBoundingBox(BoxShader);


            simpleMonkey2->physic->size = graphMonkey2->BBsize;

      
            graphMonkey2->apply(translation2);

            add(graphMonkey2);
           
            
        }
        void setCamera(Camera m_camera)
        {
            camera = m_camera;
        }
        void add(GameObject object)
        {
            objects.push_back(object);
        }
        // To add Monkey wuth BB
        void add(SceneGraphComposite * graph)
        {
            objects.push_back(*graph->gameObject);
            for(auto & node : graph->children)
            {
                objects.push_back(*node->gameObject);

            }
           
            graphs.push_back(graph);

            
        }
        // Updated each frame
        void update(Map* map)
        {   

            //graphMonkey->gameObject->physic->display();
            glm::vec3 collisionNormal;
            float collisiontime = graphMonkey->gameObject->physic->SweptAABB(graphMonkey2->gameObject->physic, collisionNormal); 
            graphMonkey->gameObject->physic->position += graphMonkey->gameObject->physic->vitesse * collisiontime; 
            
  
            float remainingtime = 1.0f - collisiontime;
            //cout<<collisiontime<<endl;
            //cout<<graphMonkey->gameObject->physic->position<<endl;
            if(collisiontime < 0.01f)
            {
                graphMonkey->gameObject->physic->vitesse *= remainingtime;
                 graphMonkey2->gameObject->physic->vitesse *= remainingtime;
                for(int i = 0 ; i < 3 ; i++)
                {
                    if(abs(collisionNormal[i])>0.0001f)
                    {
                        graphMonkey->gameObject->physic->vitesse[i] *= -1.f;
                        graphMonkey2->gameObject->physic->vitesse[i] *= -1.f;
                    }
                }
            }

            //graphMonkey->applyPhysics();
            //graphMonkey2->applyPhysics();

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            if( (currentFrame - startFrame) > 5 )
            {
                startFrame = currentFrame;
                changeDirection = true;
            }
            for(auto & graph : graphs)
            {

                graph->applyPhysics(deltaTime);
                if(collide(map,graph))
                {
                    graph->gameObject->physic->vitesse[1] = 0.;
                    
                }
                if(changeDirection && frameCount >2 )
                {
                    int direction = rand() % 4 + 1;

                    graph->gameObject->physic->vitesse =
                    deltaTime*(direction == 1  ? vec3(1,0,0) : direction == 2 ? vec3(-1,0,0) : direction== 3 ? vec3(0,0,1)  : vec3(0,0,-1));
                    


                }
    
            }
            changeDirection = false;
            frameCount ++;


        }
        void draw()
        {
            for(auto & object : objects)
            {   
                if(object.wireframe)
                {
                    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                    object.draw(camera);
                     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                }
                else
                {
                     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                    object.draw(camera);
                }
                
            }
        }
    int mapToChunkId(int x, int renderDistance)
    {
        return (renderDistance*15 +x)/(renderDistance*15)-1;
    }

    Chunk* getChunk(Map *map,int ix, int iy)
    {
            auto chunkX = map->chunks.find(ix);
            if( chunkX != map->chunks.end())
            {
                    auto chunkY = chunkX->second.find(iy);
                    if( chunkY != chunkX->second.end())
                    {
                        return &chunkY->second;   
                    }
            }
            return NULL;
    }
    std::vector<pair<int,int>> findChunks(Map *map, glm::vec3 pos)
    {
        

        int x = mapToChunkId((int)pos[0],1);
        int y = mapToChunkId((int)pos[2],1);
        int renderDistance =1;
        std::vector<pair<int,int>> coord;

        ivec2 playerChunk = vec2(pos[0]/16,pos[2]/16);

        for(int x = playerChunk[0]-renderDistance ; x < playerChunk[0]+renderDistance ; x++ ){
            for(int y = playerChunk[1]-renderDistance ; y < playerChunk[1]+renderDistance; y ++ ){
                int ix = (x<0 ? x*-2 +1 : x*2);
                int iy = (x<0 ? y*-2 +1 : y*2);
        
                auto chunkX = map->chunks.find(ix);
                if( chunkX != map->chunks.end())
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
    int findHighest(Chunk *chonky,  ivec2 pos)
    {
        int hauteurMax = 200;
        map<int, map<int, map<int, int>>> bendel;
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
    int findCube(Chunk *chonky,  ivec3 pos)
    {
        int hauteurMax = 200;
        map<int, map<int, map<int, int>>> bendel;
        int ret =-1;
        auto cubeX = chonky->bendel.find(pos[0]);
            if( cubeX != chonky->bendel.end())
            {
                    auto cubeY = cubeX->second.find(pos[1]);
                    if( cubeY != cubeX->second.end())
                    {
                        auto cubeZ = cubeY->second.find(pos[2]);
                        if( cubeZ != cubeY->second.end())
                        {
                            ret = 1;
                            return ret;
                        }
                    }
            }
            return ret;
    }


    bool collide(Map *map, SceneGraphInterface* object)
    {
        // 4 chunks
        glm::vec3 pos = object->gameObject->t->apply(glm::vec3(0.,1.,0.));
        std::vector<pair<int,int>> chunks = findChunks(map, pos);
        glm::vec3 newPos ;
        int hauteurMax = - INT32_MAX;

        for(auto pairChunk : chunks)
        {

            Chunk *currentChunk = getChunk(map,pairChunk.first,pairChunk.second);
            // Status of creation is chunk not loaded
            if( currentChunk!=NULL && currentChunk->status !=3)
            {
                return false;
            }
        
        // Coordonnée de la boite de suzanne 
            int x = pos[0];
            int y = pos[2];

            int h = findHighest(currentChunk,ivec2(x,y));
            hauteurMax = std::max(hauteurMax,h);

            //if(h != -1 && pos[1]-hauteurMax < 2.5)
            //currentChunk->hideCube(ivec3(x,y,h));

            
        }
        //cout<<"Position : "<<pos[1]<<endl<<endl;

        //cout<<"hauteur : "<<hauteurMax<<endl<<endl;

        // 1 rayon sphere suzanne + 0.5 cube et 1 offset
        if( (pos[1]-hauteurMax) <  3.5)
        {
            newPos = glm::vec3(pos[0],hauteurMax+3.5f,pos[2]);
            glm::vec3 translate = newPos-pos;
            Transform * translation = new Transform(translate);


            translation->model = translation->getMat4();
            object->gameObject->apply(translation);
      
            return true;
        }

        return false;
    }
};