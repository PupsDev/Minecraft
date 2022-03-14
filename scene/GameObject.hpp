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

		
		

		Transform tmp = t;

		tmp.applyTransformation(parent);


		glUseProgram(programID);

		glUniformMatrix4fv(viewMatrix_uniform       , 1, false, glm::value_ptr(camera.viewMatrix));
        glUniformMatrix4fv(projectionMatrix_uniform , 1, false, glm::value_ptr(camera.projectionMatrix));
		glUniform3fv(viewPosUniform, 1, &camera.position[0]);
	

		mesh.draw(tmp);

		for(int i = 0 ; i < childs.size() ; i ++){
			childs[i]->draw(camera,tmp);
		}
	}

	void addChild(GameObject * obj ){
		childs.push_back(obj);
	}

	void setTransform(Transform tr){
		t = tr;
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

	//~GameObject();
	
};