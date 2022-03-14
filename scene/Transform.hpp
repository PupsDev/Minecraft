class Transform
{
public:
	glm::mat3 transformMat;
	glm::vec3 translationVec;

	Transform(){
		transformMat = glm::mat3(1.0);
		translationVec = glm::vec3(0.0);
	}
	//~Transform();

	void setTransform(glm::mat3 mat){
		transformMat = mat;
	}
	void setTranslation(glm::vec3 v){
		translationVec=v;
	}


	void applyTransformation(glm::mat3 mat){
		transformMat *= mat;
	}
	void applyTranslation(glm::vec3 v){
		translationVec+=v;
	}

	void applyTransformation(Transform t){
		translationVec += t.translationVec;
		transformMat = t.transformMat * transformMat;
	}



	glm::vec3 applyToPoint(glm::vec3 v){
		return transformMat*v+translationVec;
	}
	glm::vec3 applyToVector(glm::vec3 v){
		return transformMat*v;
	}


	glm::mat4 getMatrix(){
		return glm::translate(glm::mat4(transformMat),translationVec);
	}

	static glm::mat3 convertMat4(glm::mat4 mat){
	    const float *matrix = glm::value_ptr(mat);
	    float matrix2[9];
	    for(int i = 0 ; i < 3;i++)
	    {
	        for(int j=0;j<3;j++)
	        {
	            matrix2[i*3+j]=matrix[i*4+j];
	        }

	    }
	    return glm::make_mat3(matrix2);
	}
};