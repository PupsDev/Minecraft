#include <map>
#include <time.h> 
#include <unistd.h>

class GodCraft : public Scene
{
    public :



        SceneGraphComposite* graphMonkey;
        SceneGraphComposite* graphHand;
        SceneGraphComposite* graphMonkey2;
        SceneGraphComposite *graphmapObj;

        glm::vec3 vitesse;

        bool changeDirection;

        int frameCount ;
        
        bool handFull = false;
        int grabedObject=-1;
        GameObject * mapObj;
        glm::vec3 diff;
        glm::vec3 olddiff;
        float kdistance = 5.;

        //Game * game;
        GodCraft()
        {
            BoxShader = LoadShaders( "bounding_box_vertex_shader.glsl", "bounding_box_fragment_shader.glsl" );
            
            Transform * translation = new Transform(glm::vec3(2.,20.,2.));
            Transform * translation2 = new Transform(glm::vec3(8.,20.,2.));
            translation->model = translation->getMat4();
            translation2->model = translation2->getMat4();

            vitesse = glm::vec3(0.,0.,0.);
            diff = glm::vec3(0.,0.,0.);
            olddiff = glm::vec3(0.,0.,0.);
            
            srand (time(NULL));
            frameCount =0;
            deltaTime = 0;
            generatedTrees = false;

            LoaderObj loaderTree = LoaderObj("tree2.obj");
            treeMesh = Mesh();
            treeMesh.indexed_vertices = loaderTree.vertices;
            treeMesh.indices =loaderTree.indices;
            treeMesh.normals =loaderTree.normals;
            treeMesh.uvs =loaderTree.textures;
            
            treeMesh.loadTexture2("tree.DDS");
            
           

            loadSky();
            
            GameObject * Hand = new GameObject();
            Hand->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
            Hand->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
            Hand->hand = true;
            LoaderObj loader = LoaderObj("hand.obj");

            Hand->mesh.indexed_vertices = loader.vertices;
            Hand->mesh.indices =loader.indices;
            Hand->mesh.normals =loader.normals;
            Hand->mesh.uvs =loader.textures;
            Hand->HUD = true;
            Hand->mesh.loadTexture2("hand.DDS");


            Hand->loadOnGpu(BoxShader);
                Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(1.)));
            scaling->model = scaling->getMat4();
            

            graphHand = new SceneGraphComposite();
            graphHand->gameObject = Hand;
            graphHand->apply(scaling);
            graphHand->setBoundingBox(BoxShader);


            Hand->physic->size = graphHand->BBsize;

                Transform * translation3 = new Transform(glm::vec3( (float) 5,5.,4.));
            translation3->model = translation3->getMat4();
            
            
            graphHand->apply(translation);
            graphHand->apply(translation3);

            add(graphHand);

            cout<<"SizeObj :"<<objects.size()<<endl;
            loadBuilding();

            for(int i = 0 ; i < 10 ; i++)
            {
                GameObject * simpleMonkey = new GameObject();
                simpleMonkey->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
                simpleMonkey->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
                
                LoaderObj loader = LoaderObj("kevin.obj");

                simpleMonkey->mesh.indexed_vertices = loader.vertices;
                simpleMonkey->mesh.indices =loader.indices;
                simpleMonkey->mesh.normals =loader.normals;
                simpleMonkey->mesh.uvs =loader.textures;
                simpleMonkey->HUD = true;
                //loadOBJ2("dorian2.obj", simpleMonkey->mesh.indexed_vertices,  simpleMonkey->mesh.uvs,simpleMonkey->mesh.normals);
	
                simpleMonkey->mesh.loadTexture2("citrus.DDS");


                simpleMonkey->loadOnGpu(BoxShader);
                 Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(2.,2.,2.)));
                scaling->model = scaling->getMat4();
                

                graphMonkey = new SceneGraphComposite();
                graphMonkey->gameObject = simpleMonkey;
                graphMonkey->apply(scaling);
                graphMonkey->setBoundingBox(BoxShader);


                simpleMonkey->physic->size = graphMonkey->BBsize;

                  Transform * translation3 = new Transform(glm::vec3( (float) i,15.,2.));
                translation3->model = translation3->getMat4();
                
               
                graphMonkey->apply(translation);
                graphMonkey->apply(translation3);

                add(graphMonkey);
            }

            mapObj = new GameObject();
            {        
               
                GLuint HUDShader = LoadShaders( "HUD_vertex_shader.glsl", "HUD_fragment_shader.glsl" );

                LoaderObj loader = LoaderObj("simplePlanCloseUp.obj");

                mapObj->mesh.indexed_vertices = loader.vertices;
                mapObj->mesh.indices =loader.indices;
                mapObj->mesh.normals =loader.normals;
                mapObj->mesh.uvs =loader.textures;
                mapObj->HUD = true;
                mapObj->mesh.generateTexture();
                //mapObj->mesh.loadTexture2("citrus.DDS");

                mapObj->loadOnGpu(HUDShader);
                
                Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(1.)));
                scaling->model = scaling->getMat4();
            

                graphmapObj = new SceneGraphComposite();
                graphmapObj->gameObject = mapObj;
                graphmapObj->apply(scaling);
                graphmapObj->setBoundingBox(HUDShader);


                mapObj->physic->size = graphmapObj->BBsize;
                                Transform * translation3 = new Transform(glm::vec3( (float) 0,0.,0.));
                translation3->model = translation3->getMat4();
                
            
                graphmapObj->apply(translation3);

                add(mapObj);


            }
            {
                // DUMMY FOR DISPLAY MAP WTF
                GameObject * Hand = new GameObject();
                Hand->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
                Hand->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
                Hand->hand = true;
                LoaderObj loader = LoaderObj("hand.obj");

                Hand->mesh.indexed_vertices = loader.vertices;
                Hand->mesh.indices =loader.indices;
                Hand->mesh.normals =loader.normals;
                Hand->mesh.uvs =loader.textures;
                Hand->HUD = true;
                Hand->mesh.loadTexture2("hand.DDS");


                Hand->loadOnGpu(BoxShader);
                    Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(1.)));
                scaling->model = scaling->getMat4();
                

                SceneGraphComposite *gh = new SceneGraphComposite();
                gh->gameObject = Hand;
                gh->apply(scaling);
                gh->setBoundingBox(BoxShader);


                Hand->physic->size = gh->BBsize;

                    Transform * translation3 = new Transform(glm::vec3( (float) 0,50.,0.));
                translation3->model = translation3->getMat4();
                
                
                gh->apply(translation);
                gh->apply(translation3);

                add(gh);
            }
           
            
        }
                void loadSky()
        {
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
        void loadBuilding()
        {
            
            GameObject * Temple = new GameObject();
            
            LoaderObj loader = LoaderObj("temple.obj");

            Temple->mesh.indexed_vertices = loader.vertices;
            Temple->mesh.indices =loader.indices;
            Temple->mesh.normals =loader.normals;
            Temple->mesh.uvs =loader.textures;
            
            Temple->mesh.loadTexture2("checker.DDS");


            Temple->loadOnGpu(BoxShader);
            
            Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(5.)));
            scaling->model = scaling->getMat4();
        

            SceneGraphComposite *graphTemple = new SceneGraphComposite();
            graphTemple->gameObject = Temple;
            graphTemple->apply(scaling);
            graphTemple->setBoundingBox(BoxShader);


            Temple->physic->size = graphTemple->BBsize;
            Temple->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
            Temple->physic->fixed = true;
            Temple->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);

            Transform * translation3 = new Transform(glm::vec3( (float) -12,10.,-10.));
            translation3->model = translation3->getMat4();
            
        
            graphTemple->apply(translation3);

            add(graphTemple);
                

        }
        void setCamera(Camera* m_camera)
        {
            camera = m_camera;
        }

        glm::vec3 mouseToSpace(double x, double y)
        {
            //cout<<"1: "<<x<<" && 2: "<<y<<endl;
            float new_x = 2*(x / (double)camera->width) - 1.0;
            float new_y = 1. - 2*(y / (double)camera->height);


            glm::vec4 m1 = glm::vec4(new_x,new_y,0.1,1.);

            glm::mat4 iProjection = glm::inverse(camera->projectionMatrix);
            glm::vec4 m1_eye = iProjection * m1;

            m1_eye = m1_eye * 1.f/m1_eye[3];
        
            glm::mat4 iView = glm::inverse(camera->viewMatrix);

            glm::vec4 m1_space = glm::vec4(m1_eye.x,m1_eye.y,m1_eye.z,0.);

            glm::vec4 ray4 = iView * m1_space;

            glm::vec3 ray = glm::vec3(ray4.x,ray4.y,ray4.z);
            glm::vec3 mouse_ray = normalize(ray);

            return mouse_ray;
        }
        void dragMouse(MOUSE mdrag)
        {
                       double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            glm::vec3 mvec0 = mouseToSpace(xpos ,ypos);
            //glm::vec3 mvec1 = mouseToSpace2(xpos,ypos);


            glm::vec3 pos = objects[1]->computePosition();
            //cout<<mdrag.pressed<<endl;
            //cout<<mdrag.pressedRight<<endl;

            glm::vec3 pos2 = glm::vec3(8.,20.,2.);


            glm::vec3 cube = glm::vec3(0.,0.,0.); 
            glm::vec3 cube2 = glm::vec3(0.,0.,0.); 

            bool isIntersect = intersect(camera->position, mvec0, cube);
            
            if(isIntersect)
            {

                glm::vec3 trans = (cube + glm::vec3(0.,1.,0.) ) -pos;
                Transform * translation3 = new Transform(trans);
                translation3->model = translation3->getMat4();
                graphHand->apply(translation3);
                

            }
            else
            {
                glm::vec3 trans = (camera->position + kdistance*mvec0) -pos;
                Transform * translation3 = new Transform(trans);
                translation3->model = translation3->getMat4();
                graphHand->apply(translation3);
            }
            //diff = glm::vec3(0.,0.,0.);
            if(mdrag.pressed)
            {
                
                glm::vec3 mvec = mouseToSpace(mdrag.lastMouse.x ,mdrag.lastMouse.y);
                //cout<<"MOUSE OLD -> "<<mvec<<endl;
                //cout<<"MOUSE NEW -> "<<mvec0<<endl;
                
                bool isIntersect2 = intersect(camera->position, mvec, cube2);
                //cout<<"DIFF "<<cube - cube2<<endl;
                diff =  cube2-cube;
                diff[1]=0.;
                camera->position += 4.f*deltaTime *diff;
                olddiff += diff;
                camera->set(camera->position,camera->direction,glm::vec3(0.0f, 1.0f,  0.0f));
                camera->giveItToMe();
                //cout<<"CAMERA "<<camera->position<<endl;




            }
            if(mdrag.pressedRight && !handFull)
            {
                bool isIntersect2=false;
                for(int i = 1; i < graphs.size(); i++)
                {

                    Intersection inter; 
                    isIntersect2 = graphs[i]->gameObject->mesh.intersect(camera->position, mvec0, inter,graphs[i]->gameObject->t->model);
                    if(isIntersect2 )
                    {
                        //cout<<isIntersect2<<endl;
                        cout<<"Object "<<i<<endl;
                        grabedObject =i;

                       
                        graphs[i]->gameObject->t = graphHand->gameObject->t;
                        graphs[i]->children.back()->gameObject->t = graphHand->gameObject->t;
                        graphs[i]->children.back()->gameObject->parentT->model =  glm::mat4(1.f);
                         //graphs[i]->apply(graphHand->gameObject->t);
                        handFull = true;


                
                        break;
                    }
                    

                }

            }
            if(!mdrag.pressedRight && grabedObject!=-1)
            {
                handFull = false;
                cout<<"rels"<<endl;
                glm::vec3 pos = graphs[grabedObject]->gameObject->computePosition();
                glm::vec3 posHand = graphHand->gameObject->computePosition();

                if(mdrag.push)
                {
                    graphs[grabedObject]->gameObject->physic->vitesse =  camera->direction;    
                }
                graphs[grabedObject]->gameObject->t = new Transform(posHand+glm::vec3(0.,-1.,0.));
                graphs[grabedObject]->gameObject->t->model = graphs[grabedObject]->gameObject->t->getMat4();
                
                graphs[grabedObject]->children.back()->gameObject->t = graphs[grabedObject]->gameObject->t;
                //graphs[grabedObject]->children.back()->gameObject->parentT =  graphs[grabedObject]->gameObject->t;

                grabedObject=-1;
            }




        }
        // Updated each frame
        void getInput(MOUSE mdrag)
        {
            dragMouse(mdrag);

        }
        void update()
        {   

            //graphMonkey->gameObject->physic->display();
            /*
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
            */
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
                
                for(auto & graph2 : graphs)
                {
                    glm::vec3 collisionNormal;
                    float collisiontime = graph->gameObject->physic->SweptAABB(graph2->gameObject->physic, collisionNormal); 
                    graph->gameObject->physic->position += graph->gameObject->physic->vitesse * collisiontime; 
                    
        
                    float remainingtime = 1.0f - collisiontime;
                    //cout<<collisiontime<<endl;
                    //cout<<graph->gameObject->physic->position<<endl;
                    if(collisiontime < 0.01f)
                    {
                        //cout<<"collide"<<endl;
                        graph->gameObject->physic->vitesse *= remainingtime;
                        graph2->gameObject->physic->vitesse *= remainingtime;
                        for(int i = 0 ; i < 3 ; i++)
                        {
                            if(abs(collisionNormal[i])>0.0001f)
                            {
                                graph->gameObject->physic->vitesse[i] *= -1.f;
                                graph2->gameObject->physic->vitesse[i] *= -1.f;
                            }
                        }
                    }
                }
                
                int frame = rand() % 20 + 10;

                if(changeDirection && !graph->gameObject->physic->fixed && frameCount > frame  )
                {
                    int direction = rand() % 4 + 1;

                    graph->gameObject->physic->vitesse =
                    deltaTime*(direction == 1  ? vec3(1,0,0) : direction == 2 ? vec3(-1,0,0) : direction== 3 ? vec3(0,0,1)  :vec3(0,0,-1));
                    


                }
    
            }

            changeDirection = false;
            frameCount ++;
            
            
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);


            glm::vec3 mvec0 = mouseToSpace(xpos ,ypos);
            //glm::vec3 mvec1 = mouseToSpace2(xpos,ypos);

            //cout<<mvec0<<endl;
            //glm::vec3 mvec = mvec0;
            //cout<<"PRESSED -> "<<drag.pressed<<endl;
            //cout<<"MOUSE -> "<<drag.lastMouse.x<<endl;
            //cout<<"MOUSE -> "<<drag.lastMouse.y<<endl;

            glm::vec3 pos = objects[1]->computePosition();
            //cout<<pos<<endl;
            glm::vec3 pos2 = glm::vec3(8.,20.,2.);


            glm::vec3 cube = glm::vec3(0.,0.,0.); 
            glm::vec3 cube2 = glm::vec3(0.,0.,0.); 

            bool isIntersect = intersect(camera->position, mvec0, cube);
            
            if(isIntersect)
            {

                glm::vec3 trans = (cube + glm::vec3(0.,1.,0.) ) -pos;
                Transform * translation3 = new Transform(trans);
                translation3->model = translation3->getMat4();
                graphHand->apply(translation3);
                

            }
            else
            {
                glm::vec3 trans = (camera->position + kdistance*mvec0) -pos;
                Transform * translation3 = new Transform(trans);
                translation3->model = translation3->getMat4();
                graphHand->apply(translation3);
            }

           
            glm::vec3 posMap = mapObj->computePosition();
            //cout<<posMap<<endl;
            
            glm::mat3 view3 =  Transform::convertMat4(camera->viewMatrix);
            glm::mat3 iview3 = glm::inverse(view3);


            int sizex = 1;
            int sizey =sizex;
            int default_value = 1;
            std::vector<int> v(sizex, default_value);
            std::vector<std::vector<int>> shapeCamera(sizey, v);

            int squareSize = 16;
            int borderSize = 1;
            std::vector<int> v2(squareSize+borderSize, default_value);
            std::vector<std::vector<int>> shapeBorder(squareSize+borderSize, v2);
            
            for(int i = borderSize ; i < squareSize-borderSize;i++)
            {
                for(int j = borderSize ; j < squareSize-borderSize;j++)
                {
                    shapeBorder[i][j]=0;
                }
                
            }
            //sizex=17;
            //sizey=17;
            
            int offsetx = (int)camera->position[0]%16;
            int offsety = (int)camera->position[2]%16;
            for(int s=-sizey/2 ; s<sizey/2 +1 ; s++)
            {
                for(int t=-sizex/2 ; t<sizex/2+1 ; t++)
                {   
                    if(shapeCamera[s+sizey/2][t+sizex/2])
                    {
                        // DRAW CAMERA
                        map->imageMap[160+offsetx+s][160+offsety+t]= glm::vec3(255,0.,0.);
                       
                    }
                    
                }

            }
            
            
            std::vector<int> v3(9, default_value);
            std::vector<std::vector<int>> shapeChunk(9, v3);
            int x = (camera->position[0])/16;
            int y = (camera->position[2])/16;
            std::vector<pair<int,int>> chunksIds = map->findChunks( camera->position);
            
            for(int s=-9/2 ; s<9/2 +1 ; s++)
            {
                for(int t=-9/2 ; t<9/2+1 ; t++)
                {   
                    if(shapeChunk[s+9/2][t+9/2])
                    {
                        // DRAW CAMERA
                        for(auto & chunk : chunksIds)
                            map->imageMap[160+chunk.first*16 +8 ][160+chunk.second*16 +8]= glm::vec3(255,255.,0.);
                       
                    }
                    
                }

            }
       
            // DRAW CUBE
            /*int x = (cube[0])/16;
            int y = (cube[2])/16;*/
            //cout<<"CUBE --> "<<cube<<endl;

            /*for(int s=-9/2 ; s<9/2 +1 ; s++)
            {
                for(int t=-9/2 ; t<9/2+1 ; t++)
                {   
                    if(shapeChunk[s+9/2][t+9/2])
                    {
    
                        map->imageMap[160+cube[0]+s][160+cube[2]+t]= glm::vec3(255,255.,255.);
                    
                    }
                    
                }

            }*/
            

                       
 



            /*
            for(int i = 8 ; i < map->imageMap.size()-8;i+=16)
            {
                for(int j = 8  ; j < map->imageMap[i].size()-8 ; j+=16)
                {
                    for(int s=-(squareSize+borderSize)/2 ; s<(squareSize+borderSize)/2 +1 ; s++)
                    {
                        for(int t=-(squareSize+borderSize)/2 ; t<(squareSize+borderSize)/2+1 ; t++)
                        {   
                            if(shapeBorder[s+(squareSize+borderSize)/2][t+(squareSize+borderSize)/2])
                            {
                                // DRAW CUNK
                                map->imageMap[i+s][j+t]= glm::vec3(255,0.,0.);
                            }
                            
                        }

                    }

                }
            }*/

            mapObj->mesh.reloadTexture(map->imageMap);
            

        }


    
        void printBendel(std::map<int, std::map<int, std::map<int, int>>> bendel)
        {
            std::map<int, std::map<int, std::map<int, int>>> ::iterator it;

            for (it = bendel.begin(); it != bendel.end(); it++)
            {
                std::cout << it->first    // string (key)
                        << ':'
                        << "it->second"   // string's value 
                        << std::endl;
                std::map<int, std::map<int, int>> ::iterator it2;
                for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << it2->first <<endl;
                }
            }
        }
        void printMap(std::map<int, std::map<int, Chunk>> chunks)
        {
            std::map<int, std::map<int, Chunk>> ::iterator it;

            for (it = chunks.begin(); it != chunks.end(); it++)
            {
                std::cout << it->first    // string (key)
                        << ':'
                        << "it->second:"   // string's value 
                        << std::endl;
                std::map<int, Chunk> ::iterator it2;
                for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << it2->first <<endl;
                }

            }
        }
        
    void draw()
    {

        for(auto & object : objects)
        {   
            //cout<<"cpt : "<<cpt++<<endl;

            Chunk * chonky = map->getChunk(object->chunksIds[0],object->chunksIds[1]);
            if(chonky || object->HUD)
            {
                if(chonky->drawn || object->HUD)
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

            
        }
    }



    bool intersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 &cube)
    {
        
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