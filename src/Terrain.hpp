#include <vector>
#include <iostream>
#include <common/texture.hpp>

using namespace std;

class Terrain : virtual public GameObject
{
public:

	GLuint programID;

	GLuint vertexbufferPlane;
	GLuint elementbufferPlane;
	GLuint uvBufferPlane;


	std::vector<unsigned short> indices_plane; //Triangles concaténés dans une liste 
    std::vector<glm::vec3> indexed_vertices_plane;
    std::vector<glm::vec2> uvs;

    int resolution = 4;
    //std::vector<float> heightMap;

    GLuint grass;
    GLuint rock;
    GLuint snow;
    GLuint heightMap;

    GLuint water;

    GLuint modelMatrix_uniform;


    GLuint textureLocation0;
	GLuint textureLocation1;
	GLuint textureLocation2;
	GLuint textureLocation3;
	GLuint textureLocation4;

	Terrain(GLuint id){
		programID = id;
		//super(id);
	}

	void setResolution(int n){
		resolution = n;
	}

    void generatePlane(std::vector<unsigned short> *indexed_vertices, std::vector<glm::vec3> *vertices, std::vector<glm::vec2> *uvs, int nbx, int nby){
    	indexed_vertices->clear();
    	vertices->clear();
    	uvs->clear();

	    for(int x = 0 ; x < nbx ; x++){
	        for(int y = 0 ; y  < nby ; y++){
	            vertices->push_back(vec3(((float)x)/nbx,((float)y)/nby,0.0));
	            uvs->push_back(vec2(((float)x)/(nbx-1),((float)y)/(nby-1)));
	            if(x!=nbx-1 && y != nby-1){
	                indexed_vertices->push_back(y*nbx + x);
	                indexed_vertices->push_back(y*nbx + x +1);
	                indexed_vertices->push_back((y+1)*nbx + x);

	                indexed_vertices->push_back(y*nbx + x +1);
	                indexed_vertices->push_back((y+1)*nbx + x+1);
	                indexed_vertices->push_back((y+1)*nbx + x);

	            }
	        }
	    }
	}

	void generateHeightMap(std::vector<float> *heightMap, int nbx, int nby){
		heightMap->clear();
	    for(int x = 0 ; x < nbx ; x++){
	        for(int y = 0 ; y < nby ; y ++){
	            heightMap->push_back(rand() % 10000 / 10000.0);
	        }
	    }
	}

	void recompute(){
		generatePlane(&indices_plane, &indexed_vertices_plane , &uvs, resolution, resolution);
		
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlane);
	    glBufferData(GL_ARRAY_BUFFER, indexed_vertices_plane.size() * sizeof(glm::vec3), &indexed_vertices_plane[0], GL_STATIC_DRAW);
	    	    
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferPlane);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_plane.size() * sizeof(unsigned short), &indices_plane[0] , GL_STATIC_DRAW);
	    
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uvBufferPlane);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0] , GL_STATIC_DRAW);
	}


	void loadOnRam(){

		grass = loadBMP_custom("../textures/tex_grass.bmp");
		rock = loadBMP_custom("../textures/tex_rock.bmp");
		snow = loadBMP_custom("../textures/tex_snowrock.bmp");

		water = loadBMP_custom("../textures/water1.bmp");



		heightMap = loadBMP_custom("../textures/hmap_rocky.bmp");

		generatePlane(&indices_plane, &indexed_vertices_plane , &uvs, resolution, resolution);
    	//generateHeightMap( &heightMap, 64, 64);
	}

	void loadOnGpu(){
		glUseProgram(programID);

	    glGenBuffers(1, &vertexbufferPlane);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlane);
	    glBufferData(GL_ARRAY_BUFFER, indexed_vertices_plane.size() * sizeof(glm::vec3), &indexed_vertices_plane[0], GL_STATIC_DRAW);
	    
	    glGenBuffers(1, &elementbufferPlane);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferPlane);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_plane.size() * sizeof(unsigned short), &indices_plane[0] , GL_STATIC_DRAW);

	    glGenBuffers(1, &uvBufferPlane);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uvBufferPlane);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0] , GL_STATIC_DRAW);

	    modelMatrix_uniform = glGetUniformLocation(programID,"model");

	    textureLocation0 = glGetUniformLocation(programID,"myTextureSampler[0]");
		textureLocation1 = glGetUniformLocation(programID,"myTextureSampler[1]");
		textureLocation2 = glGetUniformLocation(programID,"myTextureSampler[2]");
		textureLocation3 = glGetUniformLocation(programID,"myTextureSampler[3]");
		textureLocation4 = glGetUniformLocation(programID,"myTextureSampler[4]");

	    /*glGenBuffers(1, &heightMapBuffer);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, heightMapBuffer);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, heightMap.size() * sizeof(GL_FLOAT), &heightMap[0] , GL_STATIC_DRAW);*/
	}

	void draw(Camera camera, Transform parent = Transform()){
		

		glUseProgram(programID);

		Transform tmp = t;

		//tmp.applyTransformation(parent);

		glUniformMatrix4fv(modelMatrix_uniform, 1, false, glm::value_ptr(tmp.getMat4()));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlane);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_TRUE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

        // Index buffer

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferPlane);

        
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferPlane);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );
                    
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightMap);
        glUniform1i(textureLocation0,0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, grass);
        glUniform1i(textureLocation1,1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, rock);
        glUniform1i(textureLocation2,2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, snow);
        glUniform1i(textureLocation3,3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, water);
        glUniform1i(textureLocation4,4);



        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices_plane.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

		glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//GameObject::draw(camera, tmp);
	}
	
};