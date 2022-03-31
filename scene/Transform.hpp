#ifndef TRANSFORM
#define TRANSFORM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

class Transform
{
    public:
        float s; // scale
        glm::mat3 m; // rotation
        glm::vec3 translation; // translate

        glm::mat4 model;

        Transform()
        {
            this->s =1.;
            this->m = glm::mat3(1.);
            this->translation = glm::vec3(0.);
            this->model = glm::mat4(1.f);
        }
        Transform(glm::vec3 translation)
        {
            this->s =1.;
            this->m = glm::mat3(1.);
            this->translation = translation;

            model = getMat4();
        }
        Transform(glm::mat3 m)
        {
            this->s =1.;
            this->m = m;
            this->translation = glm::vec3(0.);    

            model = getMat4();                
        }
        Transform(glm::mat3 m,glm::vec3 translation)
        {
            this->s =1.;
            this->m = m;
            this->translation = translation;  

            model = getMat4();  
            
            
        }
         Transform(float s,glm::mat3 m,glm::vec3 translation)
         {
            this->s =s;
            this->m = m;
            this->translation = translation;

            model = getMat4();
            
             
         }
        Transform(glm::mat4 mat)
         {
            this->s =1.;
            this->m = glm::mat3(1.);
            this->translation = glm::vec3(0.);

            this->model = mat;
         }
         
         void print()
         {
             std::cout<<"\nscale: "<<"\n";
             std::cout<<s<<"\n";
             std::cout<<"matrice: "<<"\n";
            for(int i = 0 ; i < 3;i++)
            {
                for(int j=0;j<3;j++)
                {
                    std::cout<<glm::value_ptr(m)[i*3+j]<<" ";
                }
                std::cout<<"\n";

            }
            std::cout<<"translate: "<<"\n";
     
            std::cout<<glm::to_string(translation)<<" ";
            

         }
        void printmat4(glm::mat4 m)
         {

            for(int i = 0 ; i < 4;i++)
            {
                for(int j=0;j<4;j++)
                {
                    std::cout<<glm::value_ptr(m)[j*4+i]<<"\t";
                }
                std::cout<<"\n";

            }
            

         }
        void printmat4()
         {

            for(int i = 0 ; i < 4;i++)
            {
                for(int j=0;j<4;j++)
                {
                    std::cout<<glm::value_ptr(model)[j*4+i]<<"\t";
                }
                std::cout<<"\n";

            }
            

         }
        static glm::mat3 convertMat4(glm::mat4 mat)
                {
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
        glm::mat4 getMat4()
        {
            return  glm::translate(model, translation )*glm::mat4(m)*glm::mat4(s);
            //return  glm::translate(glm::mat4(), translation )*glm::mat4(m)*glm::mat4(s);
        }
        glm::vec3 apply(glm::vec3 p)
        {
            glm::vec4 pt = glm::vec4(p,1.);
            pt = this->model * pt;
            glm::vec3 res = glm::vec3(pt.x,pt.y,pt.z);
            return res;
        }
        glm::vec3 applyToPoint(glm::vec3 p)
        {   
            vec4 tmpVec4 = getMat4() * vec4(p,1);
            return vec3(tmpVec4);

            return m * (s*p) +translation;
        }

        Transform combine_with(Transform & t, float k)
        {
            Transform result;
            result.s = this->s*k + t.s*(1-k);
            return result;
        }
        /*static Transform* multiply(Transform* t)
        {
            Transform * result = new Transform();
            result->s = this->s*t->s;
            result->m = this->m*t->m;
            result->translation = this->translation+t->translation;
            return result;
        }*/
        void multiply(Transform* t)
        {
            model = getMat4();
            this->model = t->model*model;
            
        }
        Transform * inverse()
        {
            Transform * result = new Transform();
            result->s = 1.f/this->s;
            result->m = glm::inverse(this->m);
            result->translation = -this->translation;
            return result;

        }

        // vec3 getTranslation(){
            
        //     return vec3(value_ptr(model)[12],value_ptr(model)[13],value_ptr(model)[14]);
        // }
        
};


#endif