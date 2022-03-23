class BoundingBox : public GameObject
{

    public:     
        glm::vec3 pos;

        // On test avec une sphere comme des teuteu
        float radius;
        BoundingBox()
        {
            radius=1.;
            createSphere(mesh.indices,mesh.indexed_vertices,mesh.uvs, 32);
        }
        bool intersect()
        {

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