class Scene
{
    public :
        vector<GameObject> objects;
        Camera camera;

        SceneGraphComposite* graphMonkey;
        SceneGraphComposite* graphMonkey2;

        glm::vec3 vitesse;

        Scene()
        {
            GLuint BoxShader = LoadShaders( "bounding_box_vertex_shader.glsl", "bounding_box_fragment_shader.glsl" );
            Transform * translation = new Transform(glm::vec3(2.,20.,2.));
            Transform * translation2 = new Transform(glm::vec3(8.,20.,2.));
            translation->model = translation->getMat4();
            translation2->model = translation2->getMat4();

            vitesse = glm::vec3(0.,0.,0.);


            

            GameObject * simpleMonkey = new GameObject();
            simpleMonkey->physic = new PhysicComponent(glm::vec3(0.,0.,0.));
            simpleMonkey->physic->vitesse = 0.01f*glm::vec3(1.,0.,0.);
            
            simpleMonkey->loadMesh("suzanne.off");
            simpleMonkey->loadOnGpu(BoxShader);

            graphMonkey = new SceneGraphComposite();
            graphMonkey->gameObject = simpleMonkey;
            graphMonkey->setBoundingBox(BoxShader);


            simpleMonkey->physic->size = graphMonkey->BBsize;

      
            graphMonkey->apply(translation);

            add(graphMonkey);


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
        void add(SceneGraphInterface * graph)
        {
            objects.push_back(*graph->gameObject);
            for(auto & node : graph->children)
            {
                objects.push_back(*node->gameObject);

            }
        }
        // Updated each frame
        void update()
        {   

            //graphMonkey->gameObject->physic->display();
            glm::vec3 collisionNormal;
            float collisiontime = graphMonkey->gameObject->physic->SweptAABB(graphMonkey2->gameObject->physic, collisionNormal); 
            graphMonkey->gameObject->physic->position += graphMonkey->gameObject->physic->vitesse * collisiontime; 
            
  
            float remainingtime = 1.0f - collisiontime;
            cout<<collisiontime<<endl;
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

            graphMonkey->applyPhysics();
            graphMonkey2->applyPhysics();


        }
        void draw()
        {
            for(auto & object : objects)
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
};