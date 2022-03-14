/*        auto modelMatrix = glm::mat4(1.0);
        //modelMatrix = glm::translate(modelMatrix ,glm::vec3(0.5, 0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, chairAngle, glm::vec3(0.0, 0.0, 1.0));
        modelMatrix = glm::scale(modelMatrix ,glm::vec3(1.0));
        modelMatrix = glm::translate(modelMatrix ,glm::vec3(0.0,-0.5,0.0));*/






        /*****************TODO***********************/
        // Model matrix : an identity matrix (model will be at the origin) then change

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms

        /****************************************/


        
/*

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);


        modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix ,glm::vec3(-1.0, -1.0, 0.0));
        modelMatrix = glm::scale(modelMatrix ,glm::vec3(0.5));

        glUniformMatrix4fv(modelMatrix_uniform      , 1, false, glm::value_ptr(modelMatrix));


        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );


        modelMatrix = glm::mat4(1.0);
        modelMatrix = glm::translate(modelMatrix ,glm::vec3(1.0, -1.0, 0.0));
        modelMatrix = glm::scale(modelMatrix ,glm::vec3(-0.5,0.5,0.5));

        glUniformMatrix4fv(modelMatrix_uniform      , 1, false, glm::value_ptr(modelMatrix));

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

        modelMatrix = glm::mat4(1.0);
        //modelMatrix = glm::translate(modelMatrix ,glm::vec3(0.5, 0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, chairAngle, glm::vec3(0.0, 0.0, 1.0));
        modelMatrix = glm::scale(modelMatrix ,glm::vec3(1.0));
        modelMatrix = glm::translate(modelMatrix ,glm::vec3(0.0,-0.5,0.0));

        glUniformMatrix4fv(modelMatrix_uniform      , 1, false, glm::value_ptr(modelMatrix));

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

*/

        ///////////////






/*
        if(cameraMode){
            modelMatrix = glm::mat4(1.0);
            viewMatrix = glm::lookAt(camera_position, camera_target+camera_position, camera_up);

            //cout<<camera_position<<" | "<<(camera_target+camera_position)<<" | "<<camera_up<<endl;

            projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);

            camera.viewMatrix = viewMatrix;
            camera.projectionMatrix = projectionMatrix;

            modelMatrix = glm::mat4(1.0);
            modelMatrix = glm::rotate(modelMatrix, (float)(3.1415926535897932384626433832795/2.0), glm::vec3(1.0, 0.0, 0.0));
            modelMatrix = glm::translate(modelMatrix ,glm::vec3(-1.0, -1.0, 0.0));
            modelMatrix = glm::scale(modelMatrix ,glm::vec3(10.0));
        }else{ 
            modelMatrix = glm::mat4(1.0);
            viewMatrix = glm::lookAt(vec3(8.16655, 0.26985, -9.64445), vec3(7.8268, -0.136002, -8.79601), vec3(0,1,0));
            projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);


            camera.viewMatrix = viewMatrix;
            camera.projectionMatrix = projectionMatrix;

            modelMatrix = glm::mat4(1.0); 
            //cout<<"deltaTime : "<<time<<endl;
            modelMatrix = glm::translate(modelMatrix ,glm::vec3(1.0, -1.0, 5));
            modelMatrix = glm::scale(modelMatrix ,glm::vec3(10.0));
            modelMatrix = glm::rotate(modelMatrix, terrainAngle, glm::vec3(0.0, 1.0, 0.0));
            modelMatrix = glm::translate(modelMatrix ,glm::vec3(-0.5, 0.0, -0.5));
            modelMatrix = glm::rotate(modelMatrix, (float)(3.141592653589793238462643383279/2.0), glm::vec3(1.0, 0.0, 0.0));

            terrainAngle+=rotationSpeed*deltaTime*1.0;
        }
*/