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
            Transform * translation = new Transform(gameObject->physic->vitesse + 0.2f*deltaTime * gameObject->physic->gravity );
            translation->model = translation->getMat4();
            apply(translation);
        }


        void add(SceneGraphInterface* child)
        {
            
            child->gameObject->parentT = gameObject->t;
            children.push_back(child);
            
        }

};
#endif