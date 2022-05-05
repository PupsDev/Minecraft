struct Intersection{
    glm::vec3 hitPoint;
    float t;
};

class Triangle
{
    public:
        glm::vec3 v[3];

        Triangle();
        Triangle(glm::vec3 const & v0 , glm::vec3 const & v1 , glm::vec3 const & v2 )
        {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }
        bool intersect(const glm::vec3 &orig,const glm::vec3 &dir,Intersection& intersection) const
        {
            float t_min =0.;
            float t_max =100.;
            glm::vec3 e1, e2 , h, s, q;
            float a,f,u,v,t;
            e1=this->v[1]-this->v[0];
            e2=this->v[2]-this->v[0];
            
            h = glm::cross(dir,e2);
            a=dot(e1,h);
            

            if (a > -t_min && a < t_min)
                return false;    // Le rayon est parallèle au triangle.
            
            // Single Sided
            /*
            if(dot(dir,normal)<0)
            {
                return false;
            }
            */

            f = 1.0/a;

            
            s = orig - this->v[0];
        
            u = f * (dot(s,h));

            if (u < 0.0 || u > 1.0)
                return false;

            q = glm::cross(s,e1);
            v = f * dot(dir,q);
            
            if (v < 0.0 || u + v > 1.0)
                return false;
            
            t = f * dot(e2,q);
            //cout<<"T="<<t<<endl;

            if (t > t_min && t < t_max) // Intersection avec le rayon
            {
                /*float area = glm::dot( normal, glm::cross( (this->v[1] - this->v[0]), (this->v[2] - this->v[0]) )  ) ;
                float areaPBC = glm::dot( normal, glm::cross( (this->v[1] - this->v[0]), (this->v[2] - this->v[0]) )  ) ;
                float areaPCA = glm::dot( normal, glm::cross( (this->v[2] - this->v[0]), (this->v[0] - this->v[0]) )  ) ;

                float u0 = areaPBC / area ; // alpha
                float u1 = areaPCA / area ; // beta
                float u2 = 1.0f - u0 - u1 ; // gamma
                */

                intersection.t = t;
                
                intersection.hitPoint = orig + intersection.t * dir;
                /*
                glm::vec3 norm = vNormal[0]*u0 + vNormal[1]*u1+ vNormal[2]*u2;

                if(flatShaded)
                {
                    intersection.normal = normal;
                }
                else
                {
                    intersection.normal = norm;
                }
                
                intersection.frontFace= true;
                intersection.material = material;
                */
                
                return true;
            }
            else // On a bien une intersection de droite, mais pas de rayon.
                return false;
            

            
        }
       
        
};
