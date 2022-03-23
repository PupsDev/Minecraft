class Camera
{
public:
	glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

	vec3 position; // TODO faudra faire ça bien un jour et faire les calculs dans la classe camera (jai fais des truc)
    vec3 direction;

	GLuint programID;

	GLuint viewMatrix_uniform;
	GLuint projectionMatrix_uniform;
	GLuint viewPosUniform;

	float fov;


	void giveItToMe(){

		
		glUniformMatrix4fv(viewMatrix_uniform       , 1, false, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrix_uniform , 1, false, glm::value_ptr(projectionMatrix));
        glUniform3fv(viewPosUniform, 1, &position[0]);
	}
    void setProgramId(GLuint programID)
	{
		glUseProgram(programID);
		
		viewMatrix_uniform = glGetUniformLocation(programID,"view");
		projectionMatrix_uniform = glGetUniformLocation(programID,"projection");
		viewPosUniform = glGetUniformLocation(programID , "viewPos");
	}
	void set(vec3 camera_position,vec3 camera_target,vec3 camera_up)
	{
		position = camera_position; // TODO faudra faire ça bien un jour et faire les calculs dans la classe camera
        direction = camera_target;
        viewMatrix = glm::lookAt(position, direction+position, camera_up);
	}
	Camera(GLuint programID){

		fov = glm::radians(90.0f);
        projectionMatrix = glm::perspective(fov, 4.0f/3.0f, 0.1f, 1000.0f);

		viewMatrix_uniform = glGetUniformLocation(programID,"view");
		projectionMatrix_uniform = glGetUniformLocation(programID,"projection");
		viewPosUniform = glGetUniformLocation(programID , "viewPos");

	}

	
	double getDistance(vec3 pos){
		return length(pos-position);
	}

	//~Camera();
};