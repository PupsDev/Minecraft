#include <map>
#include <time.h> 
#include "common/testObj.hpp"
#include <unistd.h>

typedef struct COORD {
    double x,y;
}COORD;
typedef struct MOUSE{
    COORD lastMouse;
    bool pressed;
    bool pressedRight;

    bool push;
    int pick = 0;
}MOUSE;
class Scene
{
    public :
        Camera *camera;
        Map *map;
        GLFWwindow * window;

        // Forest
        vector<vector<glm::vec3>> treeMapPosition;
        bool generatedTrees;
        int treeCount = 0;
        Mesh treeMesh;

        // frames
        float startFrame;
        float lastFrame;
        float deltaTime;

        //default shader
        GLuint BoxShader;

        vector<GameObject*> objects;
        vector<SceneGraphComposite*> graphs;

        MOUSE drag;

        Scene(){}
       
        virtual void  update()
        {

        }
        void add(GameObject* object)
        {
            objects.push_back(object);
        }

        void add(SceneGraphComposite * graph)
        {
            objects.push_back(graph->gameObject);
            for(auto & node : graph->children)
            {
                objects.push_back(node->gameObject);

            }
           
            graphs.push_back(graph);

            
        }
        virtual void getInput(MOUSE mdrag)
        {

        }

        void GenerateTrees(vector<vector<int>> treeMap,int count)
        {

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

                            Chunk *chonky = map->getChunk(x, y);

                            
                            if(chonky!=NULL && chonky->drawn)
                            {
   
                                int hight = map->findHighest(chonky, ivec2(chonky->startX +resti,chonky->startY+restj));
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
                                
                                Tree->physic = new PhysicComponent(glm::vec3(0.,-9.8f,0.));
                                Tree->physic->fixed = true;
                                Tree->physic->vitesse = 0.01f*glm::vec3(0.,0.,0.);

                                SceneGraphComposite *graphTree = new SceneGraphComposite();
                                graphTree->gameObject = Tree;
                                graphTree->apply(scaling);
                                graphTree->setBoundingBox(BoxShader);


                                Tree->physic->size = graphTree->BBsize;
                                //pos+=glm::vec3(0,graphTree->BBsize[1]*0.5f,0);
                                Transform * translation3 = new Transform(pos);
                                translation3->model = translation3->getMat4();
                                
                                graphTree->apply(translation3);
                                
                                add(graphTree);

                                
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


        virtual void  draw()
        {

        }
        

   
};