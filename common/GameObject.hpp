// #ifndef GAME_OBJECT
// #define GAME_OBJECT

// #include "common/Transform.hpp"
// #include <memory>
// #include <list>
// #include <vector>
// #include "common/Mesh.hpp"
// class Component;

// class GameObject
// {


//     public:
//         GameObject()
//         {
//             this->mesh=new Mesh();
//             this->transformation=new Transform();
            
//         }

//         void compute(Transform *t)
//         { 

//             transformation = new Transform(t->model * transformation->model );

//             for(auto &point : mesh->points)
//             {
//                 point = transformation->apply(point);
//             }
//         }
//         void inverse()
//         {
//             glm::mat4 i = glm::inverse(transformation->model);
//             transformation = new Transform(i );

//             for(auto &point : mesh->points)
//             {
//                 point = transformation->apply(point);
//             }
//             transformation = new Transform(glm::mat4(1.));

//         }


//         Mesh * mesh;

//         Transform* transformation;

//         std::list<  Component * > components;
// };
// #endif
