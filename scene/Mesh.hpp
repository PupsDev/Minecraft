
ostream& operator<<(ostream& os, const glm::vec3& v){
    return(os<<v[0]<<", "<<v[1]<<", "<<v[2]);
}

class Mesh
{
public:
	std::vector<unsigned short> indices; //Triangles concaténés dans une liste
	std::vector<glm::vec3> facesNormals;

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glGenerateMipmap(GL_TEXTURE_2D);
	}
	void loadTexture2(const char* name){
		texture = loadDDS(name);
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glGenerateMipmap(GL_TEXTURE_2D);*/
	}
	void generateTexture(/*vector<vector<glm::vec3>> imageArray*/)
	{

		// Create one OpenGL texture
		GLuint textureID;
		glGenTextures(1, &textureID);
		
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		//int size =imageArray.size();
		//int * data = (int*) malloc(size*size*3 * sizeof(int));

		GLubyte checkImage[320][320][3];

		int p=0;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for(int i =0; i< 320;i++)
		{
			for(int j = 0;j<320;j++)
			{
				for(int k = 0 ; k < 3 ; k++)
				{
					checkImage[i][j][k]=(GLubyte) std::min(127,i);
				}
			}
		}
		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 320, 320, 0, GL_RGB, GL_UNSIGNED_BYTE, checkImage);

		// OpenGL has now copied the data. Free our own version
		//delete [] checkImage;

		// Poor filtering, or ...
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

		// ... nice trilinear filtering ...
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		// ... which requires mipmaps. Generate them automatically.
		glGenerateMipmap(GL_TEXTURE_2D);

		// Return the ID of the texture we just created
		texture = textureID;
	}
	void reloadTexture(vector<vector<glm::vec3>> imageArray)
	{
				glBindTexture(GL_TEXTURE_2D, texture);

		int size =imageArray.size();
		//int * data = (int*) malloc(size*size*3 * sizeof(int));

		GLubyte checkImage[320][320][3];

		int p=0;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for(int i =0; i< size;i++)
			for(int j =0; j< size;j++)
			{
				for(int k = 0 ; k < 3 ; k++)
				{
				
					checkImage[i][j][k]=(GLubyte) imageArray[i][j][k];
				}
			}

		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 320, 320, 0, GL_RGB, GL_UNSIGNED_BYTE, checkImage);

		// OpenGL has now copied the data. Free our own version
		//delete [] checkImage;

		// Poor filtering, or ...
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

		// ... nice trilinear filtering ...
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		// ... which requires mipmaps. Generate them automatically.
		glGenerateMipmap(GL_TEXTURE_2D);

	}

	void reloadOnGpu(){
		if(normals.size()!=indexed_vertices.size())
		computeNormals();

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uvBufferPlane);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0] , GL_STATIC_DRAW);


	}

    void loadOnGpu(GLuint id){

		if(normals.size()!=indexed_vertices.size())
		computeNormals();


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

    void draw(Transform * mat){


    }

	void computeNormals(){

		vector<double> valence = vector<double>() ;// = vector<double>(indexed_vertices.size(), 0.0);
		valence.resize(indexed_vertices.size(), 0.0);

		normals.resize(indexed_vertices.size(),vec3(0,0,0));

		for(int i = 0; i < indices.size() ; i += 3){
			vec3 P1 = indexed_vertices[indices[i]];
			vec3 P2 = indexed_vertices[indices[i+1]];
			vec3 P3 = indexed_vertices[indices[i+2]];

			vec3 normal = normalize(cross(P2-P1,P3-P1));
			//cout<<normal<<endl;
			facesNormals.push_back(normal);

			normals[indices[i]] += normal;
			normals[indices[i+1]] += normal;
			normals[indices[i+2]] += normal;

			valence[indices[i]] ++;
			valence[indices[i+1]] ++;
			valence[indices[i+2]] ++;
		}

		for(int i = 0 ; i < indexed_vertices.size() ; i ++){
			normals[i] = normalize(normals[i]);
			//normals[i] = normals[i] / (valence[i] > 0 ? valence[i] : 1.0);
		}

		valence.clear();
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
		Mesh(){

	}

/*
	Mesh(){
		indices = auto();
		triangles = auto();
		indexed_vertices = auto();
	}
	~Mesh();*/
	
};

