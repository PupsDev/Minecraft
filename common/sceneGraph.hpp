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


};
class SceneGraphComposite : public SceneGraphInterface
{
    public:
        SceneGraphComposite()
        {

        }

        void apply(Transform *t)
        {

            gameObject->apply(t);
            Transform *tlocal= gameObject->t;
            
            for( auto& child : children ) {
                
                child->apply(tlocal);
               

            }
        }

        void add(SceneGraphInterface* child)
        {
            children.push_back(child);
        }

};
#endif