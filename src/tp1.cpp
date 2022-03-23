// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include <common/texture.hpp>


#include "scene/Transform.hpp"

#include "scene/Camera.hpp"
#include "scene/Mesh.hpp"
#include "scene/GameObject.hpp"
#include "scene/BoundingBox.hpp"
#include "common/sceneGraph.hpp"




#include "Terrain.hpp"
#include "scene/Cube.hpp"
#include "scene/map.hpp"

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(-10.0, 2.0, -10.0);
//vec3(8.16655, 0.26985, -9.64445), vec3(7.8268, -0.136002, -8.79601)
glm::vec3 camera_target = glm::vec3(1.0, 1.0, 1.0)-camera_position;
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

//Suzie
glm::vec3 suzie_transform = glm::vec3(0.0, 0.0, 0.0); 
// timing
float deltaTime = 0.0f;	// time between current frame and last frame 
float lastFrame = 0.0f;

//rotation
float chairAngle = 0.0f;
float seaLevel = 1.0f;
float angle = 0.;
float zoom = 1.;

float terrainAngle = 0.0f;

//resolution
float resolution = 64.0;

float rotationSpeed = 1.0f;
bool cameraMode = true;
bool pvp = false;

bool keyPressed = false;
float dist(glm::vec3 a,glm::vec3 b)
{
    return sqrt( (a[0]-b[0])*(a[0]-b[0]) +  (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2]) );
}
bool collide(Map *map, GameObject* box)
{
    glm::vec3 pos = box->t->apply(glm::vec3(0.,1.,0.));
    //cout<<glm::to_string(pos)<<endl;
    // to do do with renderdistance
    int x = ((int)pos[0]+15)/15;
    int y = ((int)pos[2]+15)/15;
    //cout<<x<<" "<<y<<endl;
    auto chunkX = map->chunks.find(x-1);
   if( chunkX != map->chunks.end())
   {
           cout<<"Ligne chunk trouvé"<<endl;

        auto chunkY = chunkX->second.find(y-1);
        if( chunkY != chunkX->second.end())
        {
             cout<<"Chunk trouvé"<<endl;
            for(auto& vert : chunkY->second.gigaMesh.indexed_vertices)
            {
                

                if(dist(vert,pos)< 2.)
                {
                    return true;
                }
            }
        }
   }



    return false;
}
int init()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( SCR_WIDTH  ,SCR_HEIGHT, "TP1 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

}

int gameLoop(Map map,GLuint GameObjectShader ,Camera camera)
{
    
    GLuint BoxShader = LoadShaders( "bounding_box_vertex_shader.glsl", "bounding_box_fragment_shader.glsl" );
    
    bool displayFPS = false;
    GameObject* suz = new GameObject();
    suz->loadMesh("suzanne.off");
    suz->loadOnGpu(GameObjectShader);

    //BoundingBox* bb = new BoundingBox();
    //bb->loadOnGpu(BoxShader);

    SceneGraphComposite* graphSuz = new SceneGraphComposite();
    SceneGraphLeaf* graphBB = new SceneGraphLeaf();
    graphSuz->gameObject = suz;
    //graphBB->gameObject = bb;
    //graphSuz->add(graphBB);

    Transform * translation = new Transform(glm::vec3(0.,10.,0.));
    translation->model = translation->getMat4();
    graphSuz->gameObject->apply(translation);
    //graphBB->gameObject->apply(translation);
    
    do{
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(1/deltaTime<60 && displayFPS)
            cout<<"fps : "<<1/deltaTime<<endl;

        // input
        // -----
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        
        /*if(!collide(&map,graphBB->gameObject))
        {
            translation = new Transform(suzie_transform);
            translation->model = translation->getMat4();
            graphSuz->gameObject->apply(translation);
           // graphBB->gameObject->apply(translation);
            suzie_transform = glm::vec3(0.,0.,0.);

        }
        else
        {
            translation = new Transform(-3*suzie_transform);
            translation->model = translation->getMat4();
            graphSuz->gameObject->apply(translation);
            //graphBB->gameObject->apply(translation);
        }*/



        //suzie_transform = glm::vec3(0.,0.,0.);
        //translation = new Transform(suzie_transform);
        //translation->model = translation->getMat4();
        //graphBB->gameObject->t->model = translation->getMat4();


        camera.fov = glm::radians(90.0f);
        viewMatrix = glm::lookAt(camera_position, camera_target+camera_position, camera_up);
        projectionMatrix = glm::perspective(camera.fov, 4.0f/3.0f, 0.1f, 1000.0f);

        //camera.fov = glm::radians(45.0f);
        camera.position = camera_position; // TODO faudra faire ça bien un jour et faire les calculs dans la classe camera
        camera.direction = camera_target;

        camera.viewMatrix = viewMatrix;
        camera.projectionMatrix = projectionMatrix;
        camera.giveItToMe();

       
        map.draw(camera);
        //suz->draw(camera);
        
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        
        //bb->draw(camera);
        glDisable(GL_BLEND);



        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
}
int main( void )
{
    init();

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    //GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );
    GLuint GameObjectShader = LoadShaders( "../scene/object_vertex_shader.glsl", "../scene/object_fragment_shader.glsl" );
    

    Camera camera(programID);

    Terrain myTerrain = Terrain(programID);

    myTerrain.setResolution(resolution);
    myTerrain.loadOnRam();
    myTerrain.loadOnGpu();


    Map map = Map(GameObjectShader,100,15);

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;


    gameLoop(map,GameObjectShader,camera);

    // Cleanup VBO and shader
    //glDeleteBuffers(1, &vertexbuffer);
    //glDeleteBuffers(1, &elementbuffer);

    glDeleteProgram(programID);
    glDeleteProgram(GameObjectShader);

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera zoom in and out
    float cameraSpeed = 5 * deltaTime;
    vec3 dir = normalize(vec3(camera_target[0],0.0,camera_target[2]));

    if(cameraMode){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            glm::vec3 rotateAxis = glm::cross(camera_target,camera_up); //rotateAxis
            camera_target = glm::rotate (camera_target,0.4f*cameraSpeed * 1.0f, rotateAxis);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            glm::vec3 rotateAxis = glm::cross(camera_target,camera_up); //rotateAxis
            camera_target = glm::rotate (camera_target,0.4f*cameraSpeed * -1.0f, rotateAxis);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera_position += cameraSpeed *2.0f* glm::vec3(0.0, 1.0, 0.0);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera_position -= cameraSpeed *2.0f* glm::vec3(0.0, 1.0, 0.0);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
          camera_target = glm::rotateY(camera_target, 0.4f*cameraSpeed * 1.0f);
        
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera_target = glm::rotateY(camera_target, 0.4f*cameraSpeed * -1.0f);



        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camera_position += glm::normalize(cameraSpeed * glm::cross(camera_target,camera_up)) * 0.1f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camera_position -= glm::normalize(cameraSpeed * glm::cross(camera_target,camera_up)) * 0.1f;


        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            camera_position += 4.0f*cameraSpeed * dir;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camera_position -= 4.0f*cameraSpeed * dir;
        

        if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        {
            // if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_RELEASE)
            // {
            //     keyPressed =!keyPressed;            
            //     suzie_transform =  glm::vec3(0.,1.,0.);
            // }        
            suzie_transform =  glm::vec3(0.,1.,0.);
                
        }
        if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        {
            //if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_RELEASE)
            // {
            //     keyPressed =!keyPressed;            
            //     suzie_transform =  glm::vec3(0.,-1.,0.);
            // }        
             suzie_transform =  glm::vec3(0.,-1.,0.);
        }

    }else{
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            rotationSpeed += 0.05;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            rotationSpeed -= 0.05;
    }


    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
        resolution += deltaTime*10;
        //cout<<"resolution : "<<(int)resolution<<endl;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
        resolution -= deltaTime*10;
        //cout<<"resolution : "<<(int)resolution<<endl;
    }


     if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        seaLevel -= deltaTime*0.5;
        //cout<<"seaLevel"<<seaLevel<<endl;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        seaLevel += deltaTime*0.5;
        //cout<<"seaLevel"<<seaLevel<<endl;
    }




    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        cameraMode = !pvp ? !cameraMode : cameraMode;
        pvp = true;
    }

    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE){
        pvp = false;
    }


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
