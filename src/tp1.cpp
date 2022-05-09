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
#include <utility>

using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include <common/texture.hpp>


#include "scene/Transform.hpp"

#include "scene/LoaderObj.hpp"
#include "scene/Camera.hpp"
#include "scene/Triangle.hpp"
#include "scene/Mesh.hpp"
#include "scene/Physic.hpp"
#include "scene/GameObject.hpp"
#include "scene/BoundingBox.hpp"
#include "common/sceneGraph.hpp"
#define MINIAUDIO_IMPLEMENTATION
#include "scene/miniaudio.h"

#include "scene/ForestGenerator.hpp"
#include "scene/Voronoi.hpp"
#include "Terrain.hpp"
#include "scene/Cube.hpp"
#include "scene/map.hpp"
#include "scene/Scene.hpp"
#include "scene/GodCraft.hpp"

#include "scene/Plane.hpp"



void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 900;

// camera
glm::vec3 camera_position   = glm::vec3(0.0, 20.0, 0.0);
//vec3(8.16655, 0.26985, -9.64445), vec3(7.8268, -0.136002, -8.79601)
glm::vec3 camera_target = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

//Suzie
glm::vec3 suzie_transform = glm::vec3(0.0, 0.0, 0.0); 
// timing
float deltaTime = 0.0f;	// time between current frame and last frame 
float lastFrame = 0.0f;

//input 
glm::vec3 mouse_ray    = glm::vec3(0.0f, 0.f,0.f);
bool selectClick = false;
bool mousePressed = false;

Camera *camera = new Camera();
double  mouse_x;
double  mouse_y;
//rotation
float chairAngle = 0.0f;
float seaLevel = 1.0f;
float angle = 0.;
float zoom = 1.;

float terrainAngle = 0.0f;

//resolution
float resolution = 64.0;
float  kdistance = 8.;
float rotationSpeed = 1.0f;
bool cameraMode = true;
bool pvp = false;
bool drawMap = false;
bool keyPressed = false;
bool makeMeGravitate = false;



COORD lastMouse;


MOUSE drag;

void mouse_cursor_callback( GLFWwindow * window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
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
    

    //window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Title",  glfwGetPrimaryMonitor(), NULL);

    window = glfwCreateWindow( SCR_WIDTH  ,SCR_HEIGHT, "GodCraft", NULL, NULL);
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
        glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_cursor_callback);

    // Hide the mouse and enable unlimited mouvemezanne0.off cannot be opened
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, SCR_WIDTH/2, SCR_HEIGHT/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);


    // hide mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


    return 1;
}


int gameLoop(Scene* scene, GLuint GameObjectShader)
{

    
    bool displayFPS = false;

    glm::vec3 vitesse = glm::vec3(0.,0.,0.);
    scene->startFrame = glfwGetTime()+2.;
    ForestGenerator fg = ForestGenerator();
    do{
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(1/deltaTime<60 && displayFPS)
            cout<<"fps : "<<1/deltaTime<<endl;
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        scene->camera->set(camera_position,camera_target,camera_up);
        scene->camera->giveItToMe();
        
        scene->deltaTime = deltaTime;
        scene->getInput(drag);
        camera_position = scene->camera->position;
        scene->map->draw(scene->camera);
        if(drawMap)
            scene->map->drawMap(); 
        
        if(scene->map->drawn)
        {
            scene->GenerateTrees(fg.treeMap,fg.countTree);
            
        }
        

        scene->update();
        scene->draw();
        

        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    return 1;
}


int main( void )
{
    init();
        // mini audio player

    ma_result result;
    ma_engine engine;

    std::string input = "test.mp3";
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }

    ma_engine_play_sound(&engine, &input[0], NULL);
    
    //Voronoi v = Voronoi();
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
  

    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );
    GLuint GameObjectShader = LoadShaders( "../scene/object_vertex_shader.glsl", "../scene/object_fragment_shader.glsl" );

    camera->setProgramId(GameObjectShader);
    camera->width = SCR_WIDTH;
    camera->height = SCR_HEIGHT;
 
    
    Scene* scene = new GodCraft();
    scene->map = new Map(GameObjectShader,100,10);
    scene->window = window;
    scene->camera = camera;

    //map.draw(camera);
    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    gameLoop(scene,GameObjectShader);


    glDeleteProgram(programID);
    glDeleteProgram(GameObjectShader);

    glfwTerminate();
    ma_engine_uninit(&engine);

    return 0;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        //cout<<xpos<<" : "<<ypos<<endl;
        drag.lastMouse.x = xpos;
        drag.lastMouse.y = ypos;
        drag.push = false;
        
        
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {

        drag.pressed = false;
        
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        drag.pressedRight = true;
    }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {

        drag.pressedRight = false;
        
    }

        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        drag.pick = true;
    }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {

        drag.pick = false;
        
    }
    
}
void mouse_cursor_callback( GLFWwindow * window, double xpos, double ypos)  
{

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) 
  {
    //cout<<"RELEASE\n";
    //drag.pressed = false;
    return;
  }
   /*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
  {
     cout<<"PRESS\n";
  }
  */
  drag.pressed = true;
 
 // cout<<"DRAG"<<endl;
  //cout<<xpos<<" : "<<ypos<<endl;
  double diffx = lastMouse.x - xpos; 
  double diffy = lastMouse.y - ypos;


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
        

        if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        {
            suzie_transform =  glm::vec3(0.,1.,0.);          
        }
        if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        {
             suzie_transform =  glm::vec3(0.,-1.,0.);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        {
            suzie_transform =  glm::vec3(1.,0.,0.);          
        }
        if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        {
             suzie_transform =  glm::vec3(-1.,0.,0.);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        {
            suzie_transform =  glm::vec3(0.,0.,1.);          
        }
        if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        {
             suzie_transform =  glm::vec3(0.,0.,-1.);
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


     if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        drag.pick = true;
        //cout<<"seaLevel"<<seaLevel<<endl;
    }
    
     if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE){
        drag.pick = false;
        //cout<<"seaLevel"<<seaLevel<<endl;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        seaLevel += deltaTime*0.5;
        //cout<<"seaLevel"<<seaLevel<<endl;
    }


    if (glfwGetKey(window, GLFW_KEY_G) ==  GLFW_PRESS &&
    glfwGetKey(window, GLFW_KEY_L) ==  GLFW_PRESS ){
        makeMeGravitate=!makeMeGravitate;
        //cout<<"seaLevel"<<seaLevel<<endl;
    }
    
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
        //mymap.drawMap();
        drag.push = true;
        //cout<<"seaLevel"<<seaLevel<<endl;
    }
    


    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        cameraMode = !pvp ? !cameraMode : cameraMode;
        pvp = true;
    }

    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE){
        pvp = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        kdistance-=0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
        kdistance+=0.1;
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
