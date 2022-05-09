class PlaneCraft : public Scene{
    public :

        Plane *avion;
        BoundingBox* planeBB;

        SceneGraphComposite* graphPlane;
        //SceneGraphLeaf* graphPlaneBB;

        Transform * translation;

        //Game * game;
        PlaneCraft(){
            GLuint GameObjectShader = LoadShaders( "../scene/object_vertex_shader.glsl", "../scene/object_fragment_shader.glsl" );

            avion = new Plane();

            avion->loadMesh("avions/m2000V2.off");
            avion->mesh.computeNormals();
            avion->loadOnGpu(GameObjectShader);

            planeBB = new BoundingBox();
            planeBB->loadOnGpu(BoxShader);

            SceneGraphComposite* graphPlane = new SceneGraphComposite();
            //SceneGraphLeaf* graphPlaneBB = new SceneGraphLeaf();

            graphPlane->gameObject = avion;
            //graphPlaneBB->gameObject = planeBB;

            //graphPlane->add(graphPlaneBB);

            Transform * translation = new Transform(glm::vec3(0.,50.,0.));
            //translation->model = translation->getMat4()*glm::mat4(5.);
            translation->model = translation->getMat4();
            //graphPlane->gameObject->apply(translation);
            cout<<"MATELAS"<<endl;
            graphPlane->gameObject->t->model = translation->model;
            avion->position = graphPlane->gameObject->computePosition();

            graphPlane->gameObject->t->printmat4();
            
            add(avion);

            loadSky();

            // mapObj = new GameObject();
            // {        
               
            //     GLuint HUDShader = LoadShaders( "HUD_vertex_shader.glsl", "HUD_fragment_shader.glsl" );

            //     LoaderObj loader = LoaderObj("simplePlanCloseUp.obj");

            //     mapObj->mesh.indexed_vertices = loader.vertices;
            //     mapObj->mesh.indices =loader.indices;
            //     mapObj->mesh.normals =loader.normals;
            //     mapObj->mesh.uvs =loader.textures;
            //     mapObj->HUD = true;
            //     mapObj->mesh.generateTexture();
            //     //mapObj->mesh.loadTexture2("citrus.DDS");

            //     mapObj->loadOnGpu(HUDShader);
                
            //     Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(1.)));
            //     scaling->model = scaling->getMat4();
            

            //     graphmapObj = new SceneGraphComposite();
            //     graphmapObj->gameObject = mapObj;
            //     graphmapObj->apply(scaling);
            //     graphmapObj->setBoundingBox(HUDShader);


            //     mapObj->physic->size = graphmapObj->BBsize;
            //                     Transform * translation3 = new Transform(glm::vec3( (float) 0,0.,0.));
            //     translation3->model = translation3->getMat4();
                
            
            //     graphmapObj->apply(translation3);

            //     add(mapObj);


            // }
            // {
            //     // DUMMY FOR DISPLAY MAP WTF
            //     GameObject * Hand = new GameObject();
            //     Hand->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
            //     Hand->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
            //     Hand->hand = true;
            //     LoaderObj loader = LoaderObj("hand.obj");

            //     Hand->mesh.indexed_vertices = loader.vertices;
            //     Hand->mesh.indices =loader.indices;
            //     Hand->mesh.normals =loader.normals;
            //     Hand->mesh.uvs =loader.textures;
            //     Hand->HUD = true;
            //     Hand->mesh.loadTexture2("hand.DDS");


            //     Hand->loadOnGpu(BoxShader);
            //         Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(1.)));
            //     scaling->model = scaling->getMat4();
                

            //     SceneGraphComposite *gh = new SceneGraphComposite();
            //     gh->gameObject = Hand;
            //     gh->apply(scaling);
            //     gh->setBoundingBox(BoxShader);


            //     Hand->physic->size = gh->BBsize;

            //         Transform * translation3 = new Transform(glm::vec3( (float) 0,50.,0.));
            //     translation3->model = translation3->getMat4();
                
                
            //     gh->apply(translation);
            //     gh->apply(translation3);

            //     add(gh);
            // }
           
        }    
    void loadSky(){
        GameObject * Sky = new GameObject();           
        LoaderObj loader = LoaderObj("skyDome.obj");

        Sky->mesh.indexed_vertices = loader.vertices;
        Sky->mesh.indices =loader.indices;
        Sky->mesh.normals =loader.normals;
        Sky->mesh.uvs =loader.textures;
        Sky->HUD = true;
        
        Sky->mesh.loadTexture2("skydomeEarly.DDS");


        Sky->loadOnGpu(BoxShader);
            Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(80.)));
        scaling->model = scaling->getMat4();
        

        SceneGraphComposite *graphSky = new SceneGraphComposite();
        graphSky->gameObject = Sky;
        graphSky->apply(scaling);
        graphSky->setBoundingBox(BoxShader);


        Sky->physic->size = graphSky->BBsize;

        Transform * translation3 = new Transform(glm::vec3( (float) 0,-80.,0.));
        translation3->model = translation3->getMat4();
        
        
        graphSky->apply(translation3);

        add(Sky);  
    }

    void setCamera(Camera* m_camera){
        camera = m_camera;
    }
    void update(){   

        //camera->position = avion->position  - normalize(avion->speed) * 5.0f;
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //deltaTime = 0.02f;

        if( (currentFrame - startFrame) > 5 )
        {
            startFrame = currentFrame;
        }

        
        Plane * avion = (Plane*)objects[0];
        avion->controlesUpdate(window, deltaTime);

        auto tmpTransform = avion->update(deltaTime);


        
        //camera->position = tmpTransform->applyToPoint(vec3(0,0,0)) + vec3(0,5,-5);
        //camera->position = tmpTransform->getTranslation();// + vec3(0,5,-5);
        //camera->position = tmpTransform->applyToPoint(vec3(0, 0, 0))+vec3(-5,5,0);
        camera->position = avion->position  - normalize(avion->speed) * 5.0f;// + avion->up * 1.0f;
        //camera->up = avion->up;
        //camera->direction =  (avion->position+ avion->up * 0.0f) - camera->position;
    
        //cout<<"camera_position : "<< camera->position<<endl;
        //tmpTransform->printmat4(tmpTransform->getMat4());

        objects[0]->t = tmpTransform;
        
    /*
        if(graphPlane)
        {
            cout<<"MATELAS"<<endl;

        }*/
            //graphPlane->gameObject->t->printmat4();
        //graphPlane->gameObject->t = trans;
        
        //graphPlaneBB->gameObject->t = tmpTransform;
      
        //camera.set(camera_position,camera_target,camera_up);
    }            

    void draw(){
        //avion->draw(camera);
         for(auto & object : objects)
        {   
            if(object->wireframe)
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                object->draw(camera);
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            }
            else
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                object->draw(camera);
            }
            
        }
    }

    bool intersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 &cube){
        
        std::vector<pair<int,int>> chunksIds = map->findChunks(origin);
        cube = glm::vec3(0.,0.,0.);
        bool select = false;
        //cout<<"UN TEST\n";
        for(auto& chunkId : chunksIds)
        {
            
            float k = 0.; 
           
            glm::vec3 pos = glm::vec3(0.,0.,0.);
            while(k<100. && pos[1]>=0 && !select)
            {
                glm::vec3 pos = origin + k*direction;
                int x = pos[0]/16;
                int y = pos[2]/16;
                int resti = (int)pos[0]%16;
                int restj = (int)pos[2]%16;
                Chunk* chonky = map->getChunk(chunkId.first, chunkId.second);
                if(chonky!=NULL)
                {
                    int hight = map->findHighest(chonky, ivec2(pos[0],pos[2]));
                    if (hight != -1 && abs(hight-pos[1]) < 1 )
                    {
                        cube = glm::vec3(pos[0],hight,pos[2]);
                        //chonky->hideCube(ivec3((int)chonky->startX +resti,hight,(int)chonky->startY+restj));
                        //chonky->hideCube(cube);
                        select = true;
                        return select;

                    }
                }
                k+=0.1;
            }
        }

        return select;
    }
    
  

    bool collide(Map *map, SceneGraphInterface* object)
    {
        // 4 chunks
        glm::vec3 pos = object->gameObject->t->apply(glm::vec3(0.,1.,0.));
        std::vector<pair<int,int>> chunks = map->findChunks(pos);
        glm::vec3 newPos ;
        int hauteurMax = - INT32_MAX;

        for(auto pairChunk : chunks)
        {

            Chunk *currentChunk = map->getChunk(pairChunk.first,pairChunk.second);
            // Status of creation is chunk not loaded
            if( currentChunk!=NULL && currentChunk->status !=3)
            {
                return false;
            }
        
        // Coordonnée de la boite de suzanne 
            int x = pos[0];
            int y = pos[2];

            int h = map->findHighest(currentChunk,ivec2(x,y));
            hauteurMax = std::max(hauteurMax,h);

            //if(h != -1 && pos[1]-hauteurMax < 2.5)
            //currentChunk->hideCube(ivec3(x,y,h));

            
        }
        //cout<<"Position : "<<pos[1]<<endl<<endl;

        //cout<<"hauteur : "<<hauteurMax<<endl<<endl;

        // 1 rayon sphere suzanne + 0.5 cube et 1 offset
        if( (pos[1]-hauteurMax) <  10)
        {
            newPos = glm::vec3(pos[0],hauteurMax+3.5f,pos[2]);
            glm::vec3 translate = newPos-pos;
            Transform * translation = new Transform(translate);


            translation->model = translation->getMat4();
            object->gameObject->apply(translation);
            //cout<<"Collide : "<<endl;
      
            return true;
        }

        return false;
    }
};