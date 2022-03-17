class GameObject
{
public:

	Transform t;
	Mesh mesh;
	GLuint programID;

	GLuint viewMatrix_uniform ;
    GLuint projectionMatrix_uniform ;

	GLuint viewPosUniform;

    vector<GameObject*> childs;

	void draw(Camera camera, Transform parent = Transform()){ 

		glUseProgram(programID);

		glUniformMatrix4fv(viewMatrix_uniform       , 1, false, glm::value_ptr(camera.viewMatrix));
        glUniformMatrix4fv(projectionMatrix_uniform , 1, false, glm::value_ptr(camera.projectionMatrix));
		glUniform3fv(viewPosUniform, 1, &camera.position[0]);

		glEnableVertexAttribArray(0);
    	glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		

        glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexbuffer);
        glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

			

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.elementbuffer);
        glUniformMatrix4fv(mesh.modelMatrix_uniform      , 1, false, glm::value_ptr(t.getMat4()));

		glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBufferPlane);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );


					

		glBindBuffer(GL_ARRAY_BUFFER, mesh.normalsBuffer);
        glVertexAttribPointer(
			2,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
			
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.texture);
        glUniform1i(mesh.textureLocation0,0);

		
        // Draw the triangles !
        glDrawElements(
			GL_TRIANGLES,      // mode
			mesh.indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
			);
		

		glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		//mesh.draw(t);
	
		//cout<<"miaou"<<endl;


	}
	void apply(Transform parent)
	{
		Transform tmp = t;

		//tmp.applyTransformation(parent);
		t = parent;
	}
	void loadMesh(std::string name){
		mesh.loadFromFile(name);
	}

	void loadOnGpu(GLuint id){

		programID = id;
		glUseProgram(programID);
		mesh.loadOnGpu(programID);

		viewMatrix_uniform = glGetUniformLocation(programID,"view");
		projectionMatrix_uniform = glGetUniformLocation(programID,"projection");
		viewPosUniform = glGetUniformLocation(programID , "viewPos");
	}


	GameObject(){
		t = Transform();
		mesh = Mesh();
		childs = vector<GameObject*>(0);
	}
	GameObject(Mesh m_mesh){
		t = Transform();
		mesh = m_mesh;
		childs = vector<GameObject*>(0);
	}


	//~GameObject();
	
};