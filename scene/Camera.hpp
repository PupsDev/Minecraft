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
		glUseProgram(programID);

		glUniformMatrix4fv(viewMatrix_uniform       , 1, false, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrix_uniform , 1, false, glm::value_ptr(projectionMatrix));
        glUniform3fv(viewPosUniform, 1, &position[0]);
	}
    
	Camera(GLuint programID){
		viewMatrix_uniform = glGetUniformLocation(programID,"view");
		projectionMatrix_uniform = glGetUniformLocation(programID,"projection");
		viewPosUniform = glGetUniformLocation(programID , "viewPos");

	}
	//~Camera();
};