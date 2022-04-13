class Scene
{
    public :
        vector<GameObject> objects;
        Camera camera;

        SceneGraphComposite* graphMonkey;
        glm::vec3 vitesse;

        Scene()
        {
            GLuint BoxShader = LoadShaders( "bounding_box_vertex_shader.glsl", "bounding_box_fragment_shader.glsl" );
            Transform * translation = new Transform(glm::vec3(2.,20.,2.));
            translation->model = translation->getMat4();
            vitesse = glm::vec3(0.,0.,0.);


            

            GameObject * simpleMonkey = new GameObject();
            simpleMonkey->physic = new PhysicComponent(glm::vec3(0.,-9.82,0.));
            simpleMonkey->physic->vitesse = 0.01f*glm::vec3(0.,0.,10.);
            
            simpleMonkey->loadMesh("suzanne.off");
            simpleMonkey->loadOnGpu(BoxShader);

            graphMonkey = new SceneGraphComposite();
            graphMonkey->gameObject = simpleMonkey;
            graphMonkey->setBoundingBox(BoxShader);

            simpleMonkey->physic->position = simpleMonkey->computePosition();
            simpleMonkey->physic->size = graphMonkey->BBsize;

      
            graphMonkey->apply(translation);

            add(graphMonkey);
           
            


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
            graphMonkey->gameObject->physic->position = graphMonkey->gameObject->computePosition();
            graphMonkey->gameObject->physic->display();
            graphMonkey->applyPhysics();


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