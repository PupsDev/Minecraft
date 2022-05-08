#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H
//#include "common/GameObject.hpp"
#include <list>

class SceneGraphInterface
{
    /*
        Pattern Composite 
    */

    public:
        //virtual ~SceneGraphInterface();
        SceneGraphInterface()
        {

        }

        virtual void apply(Transform *t) = 0;
        void applyPhysics()
        {

        }


    
        GameObject* gameObject;

        SceneGraphInterface* parent;
        std::list< SceneGraphInterface* > children; 

       
};

class SceneGraphLeaf : public SceneGraphInterface
{
    public:
        SceneGraphLeaf()
        {

        }
        void apply(Transform *t)
        {
           
            gameObject->apply(t);
        }
                void applyPhysics()
        {
            
        }


};
class SceneGraphComposite : public SceneGraphInterface
{
    
    public:
        glm::vec3 BBsize;
        SceneGraphComposite()
        {

        }

        void apply(Transform *t)
        {
            // recursion papi

            gameObject->apply(t);
            

        }
        void setBoundingBox(GLuint BoxShader)
        {
            AABB* monkeyBB = new AABB(*gameObject);
            monkeyBB->loadOnGpu(BoxShader);
            BBsize = monkeyBB->size;
            SceneGraphLeaf* graphMonkeyBB = new SceneGraphLeaf();
            graphMonkeyBB->gameObject = monkeyBB;
            add(graphMonkeyBB);
            
        }
        void applyPhysics(float deltaTime)
        {
            if(abs(deltaTime)< 1.f)
            {
                glm::vec3 pos = gameObject->computePosition();
                glm::vec3 move =  gameObject->physic->vitesse + deltaTime * gameObject->physic->gravity;
                //move = glm::vec3( glm::vec3(0.,-9.8,0.));
                Transform * translation = new Transform(move);
                translation->model = translation->getMat4();
                apply(translation);

            }

        }


        void add(SceneGraphInterface* child)
        {
            
            child->gameObject->parentT = gameObject->t;
            children.push_back(child);
            
        }

};
#endif