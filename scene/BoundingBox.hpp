class BoundingBox : public GameObject
{

    public:     
        glm::vec3 pos;

        // On test avec une sphere comme des teuteu
        float radius;
        BoundingBox()
        {

        }
        bool intersect()
        {
            return false;
        }
        

};
class SphereBox : public BoundingBox
{

    public:     
        glm::vec3 pos;

        // On test avec une sphere comme des teuteu
        float radius;
        SphereBox()
        {
            radius=1.;
            createSphere(mesh.indices,mesh.indexed_vertices,mesh.uvs, 32);
        }
        bool intersect()
        {
            return false;
        }
        void createSphere(std::vector<unsigned short> &indices,std::vector<glm::vec3> &indexed_vertices,std::vector<glm::vec2> &indexed_uvs, double sommets)
        {
               
            indexed_vertices.clear();
            indexed_uvs.clear();
            indices.clear();
            //float size = 0.2;

            float const R = 1./(float)(sommets-1);
            float const S = 1./(float)(sommets-1);
            //float radius =1.;

            for(int i = 0 ; i < sommets ; i++)
                for(int j = 0 ; j < sommets ; j++)
                {
                    float const y = sin( -M_PI_2 + M_PI * i * R );
                    float const x = cos(2*M_PI * j * S) * sin( M_PI * i * R );
                    float const z = sin(2*M_PI * j * S) * sin( M_PI * i * R );
                    indexed_uvs.push_back(glm::vec2(j*S, i*R));
                    indexed_vertices.push_back(vec3(x,y,z) * radius);
                    
                    
                }
                

                
            for(int i = 0 ; i < sommets -1; i++)
                for(int j = 0 ; j < sommets-1 ; j++)
                {
                    indices.push_back((i+1)*sommets +j); 
                   
                    indices.push_back(i*sommets +j+1); 
                     indices.push_back(i*sommets +j);

                    indices.push_back(i*sommets +j+1); 
                    indices.push_back((i+1)*sommets +j); 
                    indices.push_back((i+1)*sommets +j+1); 

                }

                
        }




};
class AABB : public BoundingBox
{

    public:     
        glm::vec3 size;

        // On test avec une sphere comme des teuteu
        float radius;
        glm::vec3 vmini;
        glm::vec3 vmaxi;
        AABB()
        {
            radius=1.;
            createMesh(mesh.indices,mesh.indexed_vertices,mesh.uvs);
            wireframe = true;
        }
        AABB(GameObject model)
        {
            radius=1.;
            createMesh(mesh.indices,mesh.indexed_vertices,mesh.uvs);
            wireframe = true;
            resize(model.mesh);
        }
        void resize(Mesh model)
        {
            computeMaxMin(model.indexed_vertices);

            size = glm::vec3(abs(vmini[0]-vmaxi[0]),abs(vmini[1]-vmaxi[1]),abs(vmini[2]-vmaxi[2]));
            Transform * Transformation = new Transform(glm::scale(glm::mat4(1.f),size));
            apply(Transformation);
        }
        void computeMaxMin(std::vector<glm::vec3> indexed_vertices)
        {
            glm::vec3 mini= glm::vec3(FLT_MAX,FLT_MAX,FLT_MAX);
            glm::vec3 maxi= glm::vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);

            for(auto& vert : indexed_vertices)
            {
                for(int i = 0;i <3;i++)
                {
                    if(vert[i]<mini[i])
                    {
                        mini[i]=vert[i];
                    }
                    if(vert[i]>maxi[i])
                    {
                        maxi[i]=vert[i];
                    }

                }

            }
            vmini = mini;
            vmaxi = maxi;

        }

        bool intersect()
        {
            return false;
        }
        void createMesh(std::vector<unsigned short> &indices,std::vector<glm::vec3> &indexed_vertices,std::vector<glm::vec2> &indexed_uvs)
        {
                        indexed_vertices.clear();
            indexed_uvs.clear();
            indices.clear();
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f )); 
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f, 1.0f )); 
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f )); 

        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f));

        indexed_uvs.push_back(vec2(0.000000f, 1.0f-0.000000f));
        indexed_uvs.push_back(vec2(0.000000f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.5f));

        indexed_uvs.push_back(vec2(0.000000f, 1.0f-0.000000f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.000000f));


        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));

        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));

        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-1.0f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-1.0f));

        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-1.0f));

 
        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f  )); 
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f)); 
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f )); 

        indexed_uvs.push_back(vec2(1.000000f, 1.0f-0.000000f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(1.000000f, 1.0f-0.5f));

        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f,-1.0f));        

        indexed_uvs.push_back(vec2(1.000000f, 1.0f-0.000000f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.000000f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.5f));       

    

        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f,-1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));

        indexed_uvs.push_back(vec2(1.000000f, 1.0f-1.0f));
        indexed_uvs.push_back(vec2(1.000000f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.5f));

    

        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));

        indexed_uvs.push_back(vec2(0.666666f, 1.0f-1.0f));
        indexed_uvs.push_back(vec2(1.000000f, 1.0f-1.0f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.5f));


        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f));

        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.000000f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.5f));

        

        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f,-1.0f, 1.0f));

        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.666666f, 1.0f-0.000000f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.000000f));
        


        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f));

        indexed_uvs.push_back(vec2(0.000000f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.000000f, 1.0f-1.0f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-1.0f));

        indexed_vertices.push_back(0.5f*vec3(1.0f, 1.0f, 1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f,-1.0f));
        indexed_vertices.push_back(0.5f*vec3(-1.0f, 1.0f, 1.0f));

        indexed_uvs.push_back(vec2(0.000000f, 1.0f-0.5f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-1.0f));
        indexed_uvs.push_back(vec2(0.333333f, 1.0f-0.5f));


            for(int i = 0 ; i < indexed_vertices.size() ; i++){
                //cout<<"i : "<<i<<endl;
                indices.push_back(i);
            }
        
             
        }



};