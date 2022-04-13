

class PhysicComponent
{
    public:
        // Position centre
        glm::vec3 position;
        glm::vec3 vitesse;
        // Taille x,y,z
        glm::vec3 size; 

        glm::vec3 gravity;
        PhysicComponent()
        {

        }
        PhysicComponent(glm::vec3 gravity)
        {
            this->gravity = gravity;
        }
        void display()
        {
            cout<<"position:\n"<<position<<endl;
            cout<<"vitesse:\n"<<vitesse<<endl;
            cout<<"gravity:\n"<<gravity<<endl;

        }

};