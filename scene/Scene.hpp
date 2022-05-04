#include <map>
#include <time.h> 
#include "common/testObj.hpp"
#include <unistd.h>

class Scene
{
    public :
        vector<GameObject> objects;
        vector<SceneGraphComposite*> graphs;
        Camera camera;

        SceneGraphComposite* graphMonkey;
        SceneGraphComposite* graphHand;
        SceneGraphComposite* graphMonkey2;
         SceneGraphComposite *graphmapObj;

        glm::vec3 vitesse;
        float startFrame;
        float lastFrame;
        float deltaTime;
        bool changeDirection;

        int frameCount ;
        bool generatedTrees;
        GameObject * mapObj;

        vector<vector<glm::vec3>> treeMapPosition;
        GLuint BoxShader;
         int treeCount = 0;
         Mesh treeMesh;
        Scene()
        {
            BoxShader = LoadShaders( "bounding_box_vertex_shader.glsl", "bounding_box_fragment_shader.glsl" );
            Transform * translation = new Transform(glm::vec3(2.,20.,2.));
            Transform * translation2 = new Transform(glm::vec3(8.,20.,2.));
            translation->model = translation->getMat4();
            translation2->model = translation2->getMat4();

            vitesse = glm::vec3(0.,0.,0.);
            srand (time(NULL));
            frameCount =0;
            generatedTrees = false;

            LoaderObj loaderTree = LoaderObj("tree2.obj");
            treeMesh = Mesh();
            treeMesh.indexed_vertices = loaderTree.vertices;
            treeMesh.indices =loaderTree.indices;
            treeMesh.normals =loaderTree.normals;
            treeMesh.uvs =loaderTree.textures;
            
            treeMesh.loadTexture2("tree.DDS");

                       /* GameObject * Hand = new GameObject();
            Hand->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
            Hand->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
            
            LoaderObj loader2 = LoaderObj("hand.obj");

            Hand->mesh.indexed_vertices = loader2.vertices;
            Hand->mesh.indices =loader2.indices;
            Hand->mesh.normals =loader2.normals;
            Hand->mesh.uvs =loader2.textures;
            Hand->mesh.loadTexture("peppers.bmp");

            graphHand = new SceneGraphComposite();
            graphHand->gameObject = Hand;
            graphHand->setBoundingBox(BoxShader);

            Hand->physic->size = graphHand->BBsize;

            Transform * translation3 = new Transform(glm::vec3( (float) 7,7.,4.));
            translation3->model = translation3->getMat4();
            
        
            graphHand->apply(translation);
            graphHand->apply(translation3);
            add(graphHand);*/
           
                {
                    GameObject * Sky = new GameObject();
                Sky->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
                Sky->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
                
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
                
               
                graphSky->apply(translation);
                graphSky->apply(translation3);

                add(*Sky);
                }
                
                
                GameObject * Hand = new GameObject();
                Hand->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
                Hand->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);
                
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

                //cout<<"SizeObj :"<<objects.size()<<endl;
                //loadBuilding();

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

                mapObj->loadOnGpu(HUDShader);
                
                Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(0.25)));
                scaling->model = scaling->getMat4();
            

                graphmapObj = new SceneGraphComposite();
                graphmapObj->gameObject = mapObj;
                graphmapObj->apply(scaling);
                graphmapObj->setBoundingBox(HUDShader);


                mapObj->physic->size = graphmapObj->BBsize;

                Transform * translation3 = new Transform(glm::vec3( (float) 0,0.,0.));
                translation3->model = translation3->getMat4();
                
            
                graphmapObj->apply(translation3);

                add(*mapObj);
            }
           
            
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
        glm::vec3 mouseToSpace(double x, double y)
        {
            //cout<<"1: "<<x<<" && 2: "<<y<<endl;
            float new_x = 2*(x / (double)camera.width) - 1.0;
            float new_y = 1. - 2*(y / (double)camera.height);

            //cout<<"new_1: "<<new_x<<" && new_2: "<<new_y<<endl;
            //cout << "Cursor Position at (" << new_x << " : " << new_y << ")"<<endl;

            glm::vec4 m1 = glm::vec4(new_x,new_y,0.1,1.);

            glm::mat4 iProjection = glm::inverse(camera.projectionMatrix);
            glm::vec4 m1_eye = iProjection * m1;

            m1_eye = m1_eye * 1.f/m1_eye[3];
        
            glm::mat4 iView = glm::inverse(camera.viewMatrix);

            glm::vec4 m1_space = glm::vec4(m1_eye.x,m1_eye.y,m1_eye.z,0.);

            glm::vec4 ray4 = iView * m1_space;

            glm::vec3 ray = glm::vec3(ray4.x,ray4.y,ray4.z);
            glm::vec3 mouse_ray = normalize(ray);

            return mouse_ray;
        }

        // Updated each frame
        void update(Map* map, float kdistance)
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

            
            /*
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
                if(changeDirection && frameCount >2 )
                {
                    int direction = rand() % 4 + 1;

                    graph->gameObject->physic->vitesse =
                    deltaTime*(direction == 1  ? vec3(1,0,0) : direction == 2 ? vec3(-1,0,0) : direction== 3 ? vec3(0,0,1)  : vec3(0,0,-1));
                    


                }
    
            }

            changeDirection = false;
            frameCount ++;
            */
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);


            glm::vec3 mvec0 = mouseToSpace(xpos ,ypos);
            //glm::vec3 mvec1 = mouseToSpace2(xpos,ypos);

            //cout<<mvec0<<endl;
            glm::vec3 mvec = mvec0;
            
            glm::vec3 pos = objects[1].computePosition();
            //cout<<pos<<endl;
            glm::vec3 pos2 = glm::vec3(8.,20.,2.);


            glm::vec3 cube = glm::vec3(0.,0.,0.); 

            bool isIntersect = intersect(map, camera.position, mvec0, cube);
            
            if(isIntersect)
            {

                glm::vec3 trans = (cube + glm::vec3(0.,1.,0.) ) -pos;
                Transform * translation3 = new Transform(trans);
                translation3->model = translation3->getMat4();
                graphHand->apply(translation3);
                

            }
            else
            {
                glm::vec3 trans = (camera.position + kdistance*mvec0) -pos;
                Transform * translation3 = new Transform(trans);
                translation3->model = translation3->getMat4();
                graphHand->apply(translation3);
            }
            

           
            glm::vec3 posMap = mapObj->computePosition();

            
            glm::mat3 view3 =  Transform::convertMat4(camera.viewMatrix);
            glm::mat3 iview3 = glm::inverse(view3);



            //glm::vec3 transMap = glm::vec3(0.,0.,0.) -posMap;

            /*Transform * translationMap = new Transform(transMap);
            translationMap->model = translationMap->getMat4();
            graphmapObj->apply(translationMap);
            cout<<mapObj->computePosition()<<endl;
            */
            /*glm::mat4 viewMatrix = glm::lookAt(posMap, camera.direction+posMap, glm::vec3(0.0f, 1.0f,  0.0f));
            glm::vec4 np = viewMatrix*glm::vec4(mapObj->computePosition(),1); 
            if( abs(np[3]) >0.0001 )
                np *= 1.f/np[3];
            glm::vec3 deplace = glm::vec3(np[0],np[1],np[2])-posMap;
            */
            //glm::mat4 mat = camera.viewMatrix * 
           // glm::mat4 np = glm::inverse(camera.viewMatrix); 
            
            /*if( abs(np[3]) >0.0001 )
                np *= 1.f/np[3];
            */
            /*
            glm::vec3 pos3 = mapObj->computePosition();
            
            glm::vec3 directionMap = pos3 -camera.position;
            //glm::normalize(directionMap);
            glm::vec3 dir = camera.direction-camera.position;
            dir = glm::normalize(dir);
            //cout<<"LONGEUUR"<<glm::length(glm::cross(directionMap,dir))<<endl;
            //cout<<glm::cross(directionMap,dir)<<endl;

            Transform * translationMap = new Transform();
            translationMap->model = glm::mat4(Transform::convertMat4(camera.viewMatrix));
            
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)M_PI_2, glm::vec3(1., 0., 0.));
            
            glm::vec3 np = (camera.position +4.f*dir)-pos3;
            if(glm::length(np)>2.)
            {
                cout<<np<<endl;
                Transform * transMap = new Transform(np);
                mapObj->t->model = transMap->model *translationMap->model*rot;

            }
            */

            
            //cout<<pos3<<endl;

            
            
            /*else
            {
                Transform * noT = new Transform(-1.f*mapObj->computePosition());
                glm::vec3 np = (camera.position + 4.f*dir) -mapObj->computePosition();
                Transform * transMap = new Transform(np);
                mapObj->t->model = transMap->model * noT->model *mapObj->t->model;

            }*/

            /*
            translationMap = new Transform(transMap);
            translationMap->model = translationMap->getMat4();
            graphmapObj->apply(translationMap);*/

            
            

        

            //transMap = (camera.position + 2.f*camera.direction) -mapObj->computePosition();;
            //translationMap = new Transform(transMap);
            //translationMap->model = translationMap->getMat4();
            //graphmapObj->apply(translationMap);
            //cout<<mapObj->computePosition()<<endl;

        
            //cout<<hight<<endl;
            
            /*
            vector<vector<int>> shapeCamera;
            int sizex =5;
            int sizey =sizex;
            int i,j;
            shapeCamera.resize(sizey);

            for(i = 0; i < sizey; i++)
            {
                shapeCamera[i].resize(sizex);
                shapeCamera[i][j] = 1;

            }*/
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
            int offsetx = (int)camera.position[0]%16;
            int offsety = (int)camera.position[2]%16;
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
            /*
            std::vector<int> v3(9, default_value);
            std::vector<std::vector<int>> shapeChunk(9, v3);
            int x = (camera.position[0])/16;
            int y = (camera.position[2])/16;
            std::vector<pair<int,int>> chunksIds = findChunks(map, camera.position);
            
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

            }*/
       
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

            Transform * translation3 = new Transform(glm::vec3( (float) -12,10.,-10.));
            translation3->model = translation3->getMat4();
            
        
            graphTemple->apply(translation3);

            add(*Temple);
                

        }
        void GenerateTrees(vector<vector<int>> treeMap,int count,Map *map)
        {
            //cout<<"ok"<<endl;
            //printMap(map->chunks);

            //int x = -160/16;
            //int y = -160/16;



            //cout<<"ix:"<<ix<<"iy:"<<iy<<endl;
            //cout<<count<<endl;

            if(treeCount > 0.75*count)
            {
                
                generatedTrees = true;
            }
            if(!generatedTrees)
            {
               
                int size = treeMap.size();
                treeMapPosition.resize(size);
                for(auto& line : treeMapPosition)
                    line.resize(size);



                for(int i = 0 ; i < size ; i++)
                {

                    int resti = i%16;
                    for(int j = 0 ; j < size ; j++)
                    {
                        if(treeMap[i][j])
                        {
    
                            int restj = j%16;

                            int x = (i -size/2)/16;
                            int y = (j -size/2)/16;

                            Chunk *chonky = getChunk(map,x, y);

                            
                            if(chonky!=NULL && chonky->drawn)
                            {
   
                                int hight = findHighest(chonky, ivec2(chonky->startX +resti,chonky->startY+restj));
                                //cout<<mapi<<endl;
                                //cout<<hight<<endl;
                                glm::vec3 pos = glm::vec3(chonky->startX +resti,hight,chonky->startY+restj);
                                //cout<<pos<<endl;
                                treeMapPosition[i][j]=pos;

                                GameObject * Tree = new GameObject();
                                
                                Tree->mesh=treeMesh;
                                Tree->loadOnGpu(BoxShader);
                                Tree->chunksIds = ivec2(x,y);
                                Transform * scaling = new Transform(glm::scale(glm::mat4(1.f),glm::vec3(1.)));
                                scaling->model = scaling->getMat4();
                                

                                SceneGraphComposite *graphTree = new SceneGraphComposite();
                                graphTree->gameObject = Tree;
                                graphTree->apply(scaling);
                                graphTree->setBoundingBox(BoxShader);


                                Tree->physic->size = graphTree->BBsize;
                                //pos+=glm::vec3(0,graphTree->BBsize[1]*0.5f,0);
                                Transform * translation3 = new Transform(pos);
                                translation3->model = translation3->getMat4();
                                
                                graphTree->apply(translation3);
                                
                                add(*Tree);
                                treeCount++;
                                treeMap[i][j]=0;
                                //cout<<"generating .."<<100.*(float)treeCount/(float)count<<"\n";
                                //if(treeCount > 0.3*count)
                                //    cout<<"Done generating forest ! \n";



                            }
                            

                        }
                        
                    }
                }
                
                
            }
            

            
        }
        void printBendel(map<int, map<int, map<int, int>>> bendel)
        {
            map<int, map<int, map<int, int>>> ::iterator it;

            for (it = bendel.begin(); it != bendel.end(); it++)
            {
                std::cout << it->first    // string (key)
                        << ':'
                        << "it->second"   // string's value 
                        << std::endl;
                map<int, map<int, int>> ::iterator it2;
                for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << it2->first <<endl;
                }
            }
        }
        void printMap(map<int, map<int, Chunk>> chunks)
        {
            map<int, map<int, Chunk>> ::iterator it;

            for (it = chunks.begin(); it != chunks.end(); it++)
            {
                std::cout << it->first    // string (key)
                        << ':'
                        << "it->second:"   // string's value 
                        << std::endl;
                map<int, Chunk> ::iterator it2;
                for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << it2->first <<endl;
                }

            }
        }
        
        void draw(Map *map)
        {
            for(auto & object : objects)
            {   
                Chunk * chonky = getChunk(map,object.chunksIds[0],object.chunksIds[1]);
                if(chonky )
                {
                    if(chonky->drawn || object.HUD)
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
        int renderDistance =2;
        std::vector<pair<int,int>> coord;

        ivec2 playerChunk = vec2(pos[0]/16,pos[2]/16);

        for(int x = playerChunk[0]-renderDistance; x < playerChunk[0]+renderDistance ; x++ ){
            for(int y = playerChunk[1]-renderDistance ; y < playerChunk[1]+renderDistance; y ++ ){
                int ix = x;//(x<0 ? x*-2 +1 : x*2);
                int iy = y;//(x<0 ? y*-2 +1 : y*2);
        
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
    bool intersect(Map *map, glm::vec3 origin, glm::vec3 direction, glm::vec3 &cube)
    {
        
        std::vector<pair<int,int>> chunksIds = findChunks(map, origin);
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
                Chunk* chonky = getChunk(map,chunkId.first, chunkId.second);
                if(chonky!=NULL)
                {
                    int hight = findHighest(chonky, ivec2(pos[0],pos[2]));
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
    
    /*bool intersect(Map *map, glm::vec3 origin, glm::vec3 direction, glm::vec3 &cube)
    {
        bool select = false;

            
        float k = 0.; 
        glm::vec3 pos = glm::vec3(0.,0.,0.);
        while(k<100. && pos[1]>=0 && !select)
        {
            glm::vec3 pos = glm::vec3(0.,0.,0.) + k*direction;
            std::vector<pair<int,int>> chunksIds = findChunks(map, origin);
            for(auto& chunkId : chunksIds)
            {
                int x = pos[0]/16;
                int y = pos[2]/16;
                int resti = (int)pos[0]%16;
                int restj = (int)pos[2]%16;
                Chunk* chonky = getChunk(map,x, y);
                if(chonky!=NULL)
                {
                    int hight = findHighest(chonky, ivec2(pos[0],pos[2]));
                    if (hight != -1)
                    {
                        cube = glm::vec3(pos[0],hight,pos[2]);
                        //chonky->hideCube(ivec3((int)chonky->startX +resti,hight,(int)chonky->startY+restj));
                        select = true;
                        return select;
                    }
                }
            }
            k+=0.1;
        }
        

        return select;
    }*/
    /*
     bool intersect(Map *map, glm::vec3 origin, glm::vec3 direction, glm::vec3 &cube)
    {
        bool select = false;
        float k = 0.; 
        glm::vec3 pos = glm::vec3(0.,0.,0.);
        while(k<20. && pos[1]>=0 && !select)
        {
            glm::vec3 pos = origin + k*direction;
            int x = pos[0]/16;
            int y = pos[2]/16;
            int resti = (int)pos[0]%16;
            int restj = (int)pos[2]%16;

            Chunk* chonky = getChunk(map,x, y);
            if(chonky!=NULL)
            {
                int hight = findHighest(chonky, ivec2(pos[0],pos[2]));
                if (hight != -1)
                {
                    cube = glm::vec3(pos[0],hight,pos[2]);
                    //chonky->hideCube(ivec3((int)chonky->startX +resti,hight,(int)chonky->startY+restj));
                    select = true;
                }
            }
            else
            {
                cout<<"pas de chonk"<<endl;
            }
            k+=0.1;


        }
        return select;
    }*/

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