

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
            this->position = glm::vec3(0.);
            this->size =     glm::vec3(0.);
            this->vitesse =  glm::vec3(0.);
        }
        void display()
        {
            cout<<"position:\n"<<position<<endl;
            cout<<"vitesse:\n"<<vitesse<<endl;
            cout<<"gravity:\n"<<gravity<<endl;

        }
        float SweptAABB(PhysicComponent *b2,glm::vec3 &normal )
        {
            glm::vec3 InvEntry;
            glm::vec3 InvExit;

            glm::vec3 Entry;
            glm::vec3 Exit;
            
            for(int i = 0 ; i < 3 ; i++)
            {
                if(vitesse[i] > 0.f)
                {
                    InvEntry[i] = b2->position[i] - (position[i]+size[i]);
                    InvExit[i] = (b2->position[i]+b2->size[i]) - position[i];

                }
                else if(vitesse[i] < 0.f)
                {
                    InvEntry[i] = (b2->position[i]+b2->size[i]) - position[i];
                    InvExit[i] = b2->position[i] - (position[i]+size[i]);

                }
                if(vitesse[i] == 0.f)
                { 
                    Entry[i] = -std::numeric_limits<float>::infinity(); 
                    Exit[i] = std::numeric_limits<float>::infinity(); 
                } 
                else 
                { 
                    Entry[i] = InvEntry[i] / position[i]; 
                    Exit[i] =  InvExit[i] / position[i]; 
                } 

            }

            float entryTime = std::max(std::max(Entry[0], Entry[1]),Entry[2]); 
            float exitTime =  std::min(std::min(Exit[0], Exit[1]),Exit[2]);

            // no collision
            if (entryTime > exitTime || 
                Entry[0] < 0.0f && Entry[1] < 0.0f && Entry[2] < 0.0f ||
                Entry[0] > 1.0f || Entry[1] > 1.0f || Entry[2] > 1.0f) 

            { 
                normal = glm::vec3(0.);
                return 1.0f; 
            }
            //collision
            else
            {
                if( Entry[0] > std::max(Entry[1],Entry[2]))
                {
                    if(InvExit[0]<0.f)
                    {
                        normal = glm::vec3(1.,0.,0.);
                    }
                    else
                    {
                        normal = glm::vec3(-1.,0.,0.);
                    }
                }
                else if( Entry[1] > std::max(Entry[0],Entry[2]))
                {
                    if(InvExit[1]<0.f)
                    {
                        normal = glm::vec3(0.,1.,0.);
                    }
                    else
                    {
                        normal = glm::vec3(0.,-1.,0.);
                    }
                }
                else if( Entry[2] > std::max(Entry[1],Entry[0]))
                {
                    if(InvExit[2]<0.f)
                    {
                        normal = glm::vec3(0.,0.,1.);
                    }
                    else
                    {
                        normal = glm::vec3(0.,0.,-1.);
                    }
                }
                

            }
            return entryTime; 

        }


};