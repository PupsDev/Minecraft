class Plane : public  GameObject
{
    public:
    
    vec3 modelDirection;
    vec3 modelUp;

    vec3 up;
    vec3 direction;
    vec3 position;

    vec3 speed;

    vec3 drag;
    vec3 thrust;
    vec3 gravity;
    vec3 lift;

    float thrustFactor;
    float baseDrag;

    Plane():GameObject(){

        modelDirection = vec3(0.0f, 0.0f, -1.0f);
        modelUp = vec3(0.0f, 1.0f, 0.0f);

        
        baseDrag = 0.1;
        
        up = vec3(0., 1., 0.);
        direction = vec3(1,0.0,0);
        position = vec3(0.,10.,0.);

        speed = vec3(20,0,0);

        drag = vec3(0,0,0);
        thrust = vec3(0,0,0);
        gravity = vec3(0,-9.81,0);
    } 

    void draw(Camera *camera, Transform* parent = new Transform()){ 
        GameObject::draw(camera,parent);

        affStats();
    }

    void controlesUpdate(GLFWwindow *window, double deltaTime){
        if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS){
            up = normalize(up);
            direction = normalize(direction);
            
            auto tmpQuat = glm::angleAxis((float)glm::radians(90.f*deltaTime), up);

            up = tmpQuat * up;
            direction = tmpQuat * direction;
        }
        if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS){
            up = normalize(up);
            direction = normalize(direction);
            
            auto tmpQuat = glm::angleAxis((float)glm::radians(-90.f*deltaTime), up);

            up = tmpQuat * up;
            direction = tmpQuat * direction;
        }
        if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS){
            up = normalize(up);
            direction = normalize(direction);
            
            auto tmpQuat = glm::angleAxis((float)glm::radians(90.f*deltaTime), direction);

            up = tmpQuat * up;
            direction = tmpQuat * direction;
        }
        if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS){
            up = normalize(up);
            direction = normalize(direction);
            
            auto tmpQuat = glm::angleAxis((float)glm::radians(-90.f*deltaTime), direction);

            up = tmpQuat * up;
            direction = tmpQuat * direction;

        }
        if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS){
            up = normalize(up);
            direction = normalize(direction);
            
            vec3 right = normalize(cross(up,direction));
            auto tmpQuat = glm::angleAxis((float)glm::radians(90.f*deltaTime), right);

            up = tmpQuat * up;
            direction = tmpQuat * direction;
        }
         if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS){
             up = normalize(up);
            direction = normalize(direction);
            
            vec3 right = normalize(cross(up,direction));
            auto tmpQuat = glm::angleAxis((float)glm::radians(-90.f*deltaTime), right);

            up = tmpQuat * up;
            direction = tmpQuat * direction;

        }
        if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
            thrustFactor += 10*deltaTime;          
        }
        if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
             thrustFactor -=  10*deltaTime;
        }
    }

    float getLiftCoeficient(){
        float tmp = dot(speed, direction);
        return 0.1;
        //return (tmp-1)*(tmp-1);
    }

    Transform * update(float deltaTime){
        //vec3 up = vec3(-direction[1],direction[0],0);

        
        up = normalize(up);
        direction = normalize(direction);

        vec3 right = normalize(cross(up,direction));
        

        //thrustFactor = 50.0;

        //lift = dot(speed, direction) * up;

        vec3 liftDirection =up;
        lift = liftDirection*getLiftCoeficient()*length(speed)*length(speed)*0.5f; 

        thrust = direction * thrustFactor;

        float a = 0.01*dot(speed, direction) + 0.0005 * dot(speed, right); 
        float b = 0;
        float c = 0;

        drag = -normalize(speed) * (a*length(speed)*length(speed) + b*length(speed) + c);

        drag = vec3(0);
        thrust = vec3(0);
        //lift = vec3(0);

        speed+=(drag+thrust+gravity+lift)*deltaTime;

        position += speed*deltaTime;

       


        quat rot = quatLookAt(direction, up);

        glm:mat4 planeMat = (mat4)rot;
        planeMat = glm::translate(planeMat,inverse(rot)*position);

        Transform * translation = new Transform(planeMat);
        return translation;
    }

    void affStats(){
        double vSpeed = speed[1];
        
        cout<<endl<<endl;

        cout<<"Speed   : "<<length(vec2(speed[0],speed[2]))<<endl;
        cout<<"Vspeed  : "<<vSpeed<<endl;

        cout<<"drag    : "<<length(drag)<<endl;
        cout<<"thrust  : "<<length(thrust)<<endl;
        cout<<"lift    : "<<length(lift)<<endl;
        cout<<"gravity : "<<length(gravity)<<endl;
    }




};