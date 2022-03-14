
ostream& operator<<(ostream& os, const glm::vec3& v){
    return(os<<v[0]<<", "<<v[1]<<", "<<v[2]);
}

class Mesh
{
public:
	std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short>> triangles;
    std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec3> normals;

	std::vector<glm::vec2> uvs;

	GLuint programID;
    GLuint vertexbuffer;
	GLuint normalsBuffer;
    GLuint elementbuffer;

	GLuint uvBufferPlane;

	GLuint texture;
	GLuint textureLocation0;

    GLuint modelMatrix_uniform;

	glm::vec3 AABBMin;
	glm::vec3 AABBMax;

    void loadFromFile(std::string name){
    	loadOFF(name, indexed_vertices, indices, triangles );		
    }

	void loadTexture(const char* name){
		texture = loadBMP_custom(name,false);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glGenerateMipmap(GL_TEXTURE_2D);
	}

    void loadOnGpu(GLuint id){
    	// Load it into a VBO
    	programID = id;
    	modelMatrix_uniform = glGetUniformLocation(programID,"model");
    	

    	glGenBuffers(1, &vertexbuffer);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalsBuffer);
	    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	    // Generate a buffer for the indices as well
	    
	    glGenBuffers(1, &elementbuffer);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

		glGenBuffers(1, &uvBufferPlane);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uvBufferPlane);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0] , GL_STATIC_DRAW);

		textureLocation0 = glGetUniformLocation(programID,"myTextureSampler[0]");
    }

    void draw(Transform mat){


		// cout<<"indices : "<<indices.size()<<endl;
		// cout<<"indexed_vertices : "<<indexed_vertices.size()<<endl;
		// cout<<"normals : "<<normals.size()<<endl;
		// cout<<"uvs : "<<uvs.size()<<endl;

		// for(int i = 0 ; i < uvs.size() ; i++){
		// 	cout<<"uvs : "<<uvs[i][0]<<" "<<uvs[i][1]<<endl;
		// }

    	//cout<<"modelMatrix_uniform"<<" : "<<glGetUniformLocation(programID,"model")<<endl;
		

    	glEnableVertexAttribArray(0);
    	glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

			

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glUniformMatrix4fv(modelMatrix_uniform      , 1, false, glm::value_ptr(mat.getMatrix()));

		glBindBuffer(GL_ARRAY_BUFFER, uvBufferPlane);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );


					

		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
        glVertexAttribPointer(
			2,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
			
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureLocation0,0);

		
        // Draw the triangles !
        glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
			);
		

		glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

    }
	
	void computeAABB(){

		AABBMin = indexed_vertices[0];
		AABBMax = indexed_vertices[0];

		for(int i = 0 ; i < indexed_vertices.size() ; i ++){
			for(int dim = 0 ; dim < 3 ; dim ++){
				//cout<<i<<" "<<dim<<endl;

				if(indexed_vertices[i][dim] > AABBMax[dim] ) AABBMax[dim] = indexed_vertices[i][dim];
				if(indexed_vertices[i][dim] < AABBMin[dim] ) AABBMin[dim] = indexed_vertices[i][dim];
			}
		}

		cout<<AABBMin<<endl;
        cout<<AABBMax<<endl;
		
	}   

/*
	Mesh(){
		indices = auto();
		triangles = auto();
		indexed_vertices = auto();
	}
	~Mesh();*/
	
};

