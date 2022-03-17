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
        SceneGraphInterface();

        virtual void compute(Transform *t) = 0;


    
        GameObject* gameObject;

        SceneGraphInterface* parent;
        std::list< SceneGraphInterface* > children; 

       
};

class SceneGraphLeaf : public SceneGraphInterface
{
    public:
        SceneGraphLeaf();
        void compute(Transform *t) override;


};
class SceneGraphComposite : public SceneGraphInterface
{
    public:
        SceneGraphComposite();

        void compute(Transform *t) override ;

        void add(SceneGraphInterface* child);

};
#endif