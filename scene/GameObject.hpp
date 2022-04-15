

class GameObject
{
public:

	// parent Transform 
	Transform* parentT;
	// local transform
	Transform* t;

	Mesh mesh;
	vector<Mesh> meshLod;
	GLuint programID;

	GLuint viewMatrix_uniform ;
    GLuint projectionMatrix_uniform ;

	GLuint viewPosUniform;

	int vis;
	bool doesLOD = false;	
	double maxDist = 10;

	bool wireframe = false;
	
	PhysicComponent* physic;

	void draw(Camera camera, Transform* parent = new Transform()){ 

		double cameraDistance = camera.getDistance(t->apply(vec3(0,0,0)));
		//cameraDistance-=40;
	
		//cout<<"cameraDstance : "<<cameraDistance<<endl;

		Mesh * tmpMesh;	
			//t->printmat4();


		if(doesLOD){
			//cout<<"cameraDistance :"<<cameraDistance<<endl;
			//cout<<"affiched "<<fmin(cameraDistance/maxDist,1.0) * (meshLod.size() - 1)<<endl;
			tmpMesh = &meshLod[fmin(cameraDistance/maxDist,1.0) * (meshLod.size() - 1)];
		}else{
			tmpMesh = &mesh;
		}
		if(!doesLOD || cameraDistance<maxDist*5){	
		
			glUseProgram(programID);

			glUniformMatrix4fv(viewMatrix_uniform       , 1, false, glm::value_ptr(camera.viewMatrix));
			glUniformMatrix4fv(projectionMatrix_uniform , 1, false, glm::value_ptr(camera.projectionMatrix));
			glUniform3fv(viewPosUniform, 1, &camera.position[0]);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);

		

			glBindBuffer(GL_ARRAY_BUFFER, tmpMesh->vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

				

			// Index buffer
			//t->printmat4();
			//t->printmat4();
			glm::mat4 newmodel = parentT->model * t->model;
			Transform *model = new Transform(newmodel); 
			physic->position = model->apply(glm::vec3(0.));
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpMesh->elementbuffer);
			glUniformMatrix4fv(tmpMesh->modelMatrix_uniform      , 1, false, glm::value_ptr(newmodel));

			glBindBuffer(GL_ARRAY_BUFFER, tmpMesh->uvBufferPlane);
			glVertexAttribPointer(
						1,                  // attribute
						2,                  // size
						GL_FLOAT,           // type
						GL_FALSE,           // normalized?
						0,                  // stride
						(void*)0            // array buffer offset
						);


						

			glBindBuffer(GL_ARRAY_BUFFER, tmpMesh->normalsBuffer);
			glVertexAttribPointer(
				2,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);
				
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tmpMesh->texture);
			glUniform1i(tmpMesh->textureLocation0,0);

			
			// Draw the triangles !
			glDrawElements(
				GL_TRIANGLES,      // mode
				tmpMesh->indices.size(),    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
				);
			

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}
		//mesh.draw(t);
	
		//cout<<"miaou"<<endl;


	}
	void apply(Transform* parent)
	{


		//tmp.applyTransformation(parent);
		
		t->multiply(parent);
		

	}
	glm::vec3 computePosition()
	{
		Transform *model = new Transform(parentT->model * t->model); 
		return model->apply(glm::vec3(0.));
		

	}
	void setLodMesh(std::string name, int index){
		if(index >= meshLod.size()){
			meshLod.resize(index+1);
			cout<<"Size : "<<meshLod.size()<<endl;
		}

		meshLod[index].loadFromFile(name);
	}

	void loadMesh(std::string name){
		mesh.loadFromFile(name);
	}

	void loadOnGpu(GLuint id){

		programID = id;
		glUseProgram(programID);

		if(doesLOD){
			for(int i=0 ; i < meshLod.size() ; i++){
				meshLod[i].loadOnGpu(programID);
				//cout<<"loaded : "<<i<<endl;
			}
		}
		else
			mesh.loadOnGpu(programID);

		viewMatrix_uniform = glGetUniformLocation(programID,"view");
		projectionMatrix_uniform = glGetUniformLocation(programID,"projection");
		viewPosUniform = glGetUniformLocation(programID , "viewPos");
	}


	GameObject(){
		t = new  Transform();
		parentT = new  Transform();
		physic = new PhysicComponent();
		mesh = Mesh();
		vis =1.;
	}
	GameObject(Mesh m_mesh){
		t = new Transform();
		parentT = new Transform();
		physic = new PhysicComponent();
		mesh = m_mesh;
		vis =1.;
	}


	//~GameObject();
	
};