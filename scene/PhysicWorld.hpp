class PhysicWorld 
{
    public :
    vector< SceneGraphComposite*> m_objects;

    void add( SceneGraphComposite* o)
    {
        m_objects.push_back(o);
    }
    void step(float deltatime)
    {
        resolve(deltatime);
        for(auto obj : m_objects)
        {
            obj->gameObject->physic->force += obj->gameObject->physic->mass * obj->gameObject->physic->gravity;
            obj->gameObject->physic->vitesse = obj->gameObject->physic->force / obj->gameObject->physic->mass *deltatime;
            glm::vec3 newposition = obj->gameObject->physic->vitesse * deltatime + obj->gameObject->computePositions();
            obj->gameObject->t = Transform(newposition);
            obj->gameObject->physic->force = glm::vec3(0.,0.,0.);

        }

    }
    void resolve(float deltatime)
    {
        vector<Collision> collisions;
        for(auto a : m_objects)
        {
            for(auto b : m_objects)
            {
                if (a==b)break;
                if(!a->Collider || !b->Collider) continue;


            }
        }
    }

}