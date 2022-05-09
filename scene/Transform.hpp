#ifndef TRANSFORM
#define TRANSFORM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>

class Transform
{
    public:

        glm::mat4 model;
        quat cat;


        Transform()
        {
            this->model = glm::mat4(1.f);
        }
        Transform(glm::vec3 translation)
        {


            model = glm::translate(glm::mat4(1.), translation);
        }
        Transform( glm::mat4 mat)
        {


            model = mat;
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
            //return  glm::translate(model, translation )*glm::mat4(m)*glm::mat4(s);
            return model;
        }
        glm::vec3 apply(glm::vec3 p)
        {
            glm::vec4 pt = glm::vec4(p,1.);
            pt = this->model * pt;
            glm::vec3 res = glm::vec3(pt.x,pt.y,pt.z);
            return res;
        }


        void multiply(Transform* t)
        {
 
            this->model = t->model*model;
            
        }
        Transform * inverse()
        {
            Transform * result = new Transform();
            //result->s = 1.f/this->s;
            //result->m = glm::inverse(this->m);
            //result->translation = -this->translation;
            return result;

        }

        // vec3 getTranslation(){
            
        //     return vec3(value_ptr(model)[12],value_ptr(model)[13],value_ptr(model)[14]);
        // }
        
};


#endif