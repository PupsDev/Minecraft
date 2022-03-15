class Cube : public  GameObject
{

public:
    double size = 1;

    vec3 pos;

    int visibility = 63; //<-- le bit board dédicasse à mathieu 

    vector<Mesh> faces;


    void draw(Camera camera, Transform parent = Transform()){ 
		

		Transform tmp = t;
        tmp.applyTranslation(pos);
		tmp.applyTransformation(parent);

        glUseProgram(programID);

		glUniformMatrix4fv(viewMatrix_uniform       , 1, false, glm::value_ptr(camera.viewMatrix));
        glUniformMatrix4fv(projectionMatrix_uniform , 1, false, glm::value_ptr(camera.projectionMatrix));
		glUniform3fv(viewPosUniform, 1, &camera.position[0]);

        
        int p2 = 1;
        for(int i = 0 ; i< 6 ; i ++){

            if(visibility & p2){
                faces[i].draw(tmp);
            }

            p2*=2;
        }

		for(int i = 0 ; i < childs.size() ; i ++){
			childs[i]->draw(camera,tmp);
		}
	}

    void loadOnGpu(GLuint id){

		programID = id;
		glUseProgram(programID);
        for(int i=0 ; i<faces.size();i++){
            faces[i].loadOnGpu(programID);
        }

		viewMatrix_uniform = glGetUniformLocation(programID,"view");
		projectionMatrix_uniform = glGetUniformLocation(programID,"projection");
		viewPosUniform = glGetUniformLocation(programID , "viewPos");
	}

    void setTexture(const char * tex){
        for(int i = 0 ; i < faces.size() ; i ++){
            faces[i].loadTexture(tex);
        }
    }

    Cube(){

        faces = vector<Mesh>(6,Mesh());        

        faces[0].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f )); 
        faces[0].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f, 1.0f )); 
        faces[0].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f )); 

        faces[0].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));
        faces[0].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));
        faces[0].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f));

        faces[0].uvs.push_back(vec2(0.000000f, 1.0f-0.000000f));
        faces[0].uvs.push_back(vec2(0.000000f, 1.0f-0.5f));
        faces[0].uvs.push_back(vec2(0.333333f, 1.0f-0.5f));

        faces[0].uvs.push_back(vec2(0.000000f, 1.0f-0.000000f));
        faces[0].uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        faces[0].uvs.push_back(vec2(0.333333f, 1.0f-0.000000f));


        faces[1].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));
        faces[1].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));
        faces[1].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));

        faces[1].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));
        faces[1].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f, 1.0f));
        faces[1].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));

        faces[1].uvs.push_back(vec2(0.666666f, 1.0f-0.5f));
        faces[1].uvs.push_back(vec2(0.333333f, 1.0f-1.0f));
        faces[1].uvs.push_back(vec2(0.666666f, 1.0f-1.0f));

        faces[1].uvs.push_back(vec2(0.666666f, 1.0f-0.5f));
        faces[1].uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        faces[1].uvs.push_back(vec2(0.333333f, 1.0f-1.0f));

 
        faces[2].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f  )); 
        faces[2].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f)); 
        faces[2].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f )); 

        faces[2].uvs.push_back(vec2(1.000000f, 1.0f-0.000000f));
        faces[2].uvs.push_back(vec2(0.666666f, 1.0f-0.5f));
        faces[2].uvs.push_back(vec2(1.000000f, 1.0f-0.5f));

        faces[2].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f));
        faces[2].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));
        faces[2].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));        

        faces[2].uvs.push_back(vec2(1.000000f, 1.0f-0.000000f));
        faces[2].uvs.push_back(vec2(0.666666f, 1.0f-0.000000f));
        faces[2].uvs.push_back(vec2(0.666666f, 1.0f-0.5f));       

    

        faces[3].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));
        faces[3].indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f, 1.0f));
        faces[3].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));

        faces[3].uvs.push_back(vec2(1.000000f, 1.0f-1.0f));
        faces[3].uvs.push_back(vec2(1.000000f, 1.0f-0.5f));
        faces[3].uvs.push_back(vec2(0.666666f, 1.0f-0.5f));

    

        faces[3].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        faces[3].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));
        faces[3].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));

        faces[3].uvs.push_back(vec2(0.666666f, 1.0f-1.0f));
        faces[3].uvs.push_back(vec2(1.000000f, 1.0f-1.0f));
        faces[3].uvs.push_back(vec2(0.666666f, 1.0f-0.5f));


        faces[4].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        faces[4].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));
        faces[4].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f));

        faces[4].uvs.push_back(vec2(0.666666f, 1.0f-0.000000f));
        faces[4].uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        faces[4].uvs.push_back(vec2(0.666666f, 1.0f-0.5f));

        

        faces[4].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));
        faces[4].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        faces[4].indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));

        faces[4].uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        faces[4].uvs.push_back(vec2(0.666666f, 1.0f-0.000000f));
        faces[4].uvs.push_back(vec2(0.333333f, 1.0f-0.000000f));
        


        faces[5].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        faces[5].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f));
        faces[5].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f));

        faces[5].uvs.push_back(vec2(0.000000f, 1.0f-0.5f));
        faces[5].uvs.push_back(vec2(0.000000f, 1.0f-1.0f));
        faces[5].uvs.push_back(vec2(0.333333f, 1.0f-1.0f));

        faces[5].indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        faces[5].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f));
        faces[5].indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));

        faces[5].uvs.push_back(vec2(0.000000f, 1.0f-0.5f));
        faces[5].uvs.push_back(vec2(0.333333f, 1.0f-1.0f));
        faces[5].uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        
        


        for(int k = 0 ; k<faces.size(); k ++){
            for(int i = 0 ; i < faces[k].indexed_vertices.size(); i+=3){
                vec3 tmpNormal = cross(faces[k].indexed_vertices[i+1]-faces[k].indexed_vertices[i],faces[k].indexed_vertices[i+2]-faces[k].indexed_vertices[i]);
                for(int j = 0; j < 3 ; j ++){
                    faces[k].normals.push_back(tmpNormal);
                }
            }
        }


        for(int j = 0 ; j < faces.size() ; j ++){
            for(int i = 0 ; i < faces[j].indexed_vertices.size() ; i++){
                //cout<<"i : "<<i<<endl;
                faces[j].indices.push_back(i);
            }
        }

    }
    
    
    //~Cube();


};