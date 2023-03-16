#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Sphere.h"

const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

enum renderEnum {
    MODEL, CODE
};

struct Transform {
    Transform() : m_world_matrix(1.0f) {}
    glm::mat4 get_combined_matrix() {
        return glm::translate(glm::mat4(1.0f), m_position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(x_rotation_angle), glm::vec3(-1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(y_rotation_angle), glm::vec3(0, -1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(z_rotation_angle), glm::vec3(0, 0, -1)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
    }
    glm::mat4 m_world_matrix; //TODO: private
    glm::vec3 m_position = glm::vec3(0.0f);
    float x_rotation_angle = 0.0f;
    float y_rotation_angle = 0.0f;
    float z_rotation_angle = 0.0f;
    float m_scale = 1.0;

    glm::vec3 getRight() const
    {
        return m_world_matrix[0];
    }
    glm::vec3 getUp() const
    {
        return m_world_matrix[1];
    }
    glm::vec3 getBackward() const
    {
        return m_world_matrix[2];
    }
    glm::vec3 getGlobalScale() const
    {
        return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };
    }
};

struct SceneGraphNode {


    Shader shaderTemp = Shader("../../src/vertexShader.vert", "../../src/fragmentShader.frag");
    Shader shaderTemp2 = Shader("../../src/vertexShaderModel.vert", "../../src/fragmentShaderModel.frag");
    GLuint texture;
    Model modelTemp = Model();
    Sphere sphereTemp = Sphere();
    renderEnum tempRender;
    unsigned int VAOSphere;
 

    std::vector<std::shared_ptr<SceneGraphNode>> m_children;
    Transform m_transform;
    bool m_dirty;

    SceneGraphNode() : m_dirty(true) {}
    explicit SceneGraphNode(const Transform& t) : m_transform(t), m_dirty(true) {}
    void add_child(const std::shared_ptr<SceneGraphNode>& sgn) {
        m_children.push_back(sgn);
    }
    bool detach_child(const std::shared_ptr<SceneGraphNode>& sgn) {
        for (int i = 0; i < m_children.size(); i++) {
            if (m_children[i] == sgn) {
                m_children[i] = nullptr;
                m_children.erase(m_children.begin() + i);
                return true;
            }
        }
        return false;
    }
    void update(const Transform& parent_transform, bool dirty) {
        dirty |= m_dirty;
        if (dirty) {
            m_transform.m_world_matrix = m_transform.get_combined_matrix();
            m_transform.m_world_matrix = parent_transform.m_world_matrix * m_transform.m_world_matrix;
            m_dirty = false;
        }
        for (uint32_t i = 0; i < m_children.size(); ++i) {
            m_children[i]->update(m_transform, dirty);
        }
    }
    
    void render(bool is_root) {
        if (!is_root) {
           
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            // camera/view transformation
            glm::mat4 view = camera.GetViewMatrix();


            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);

            if (tempRender == MODEL) {

                //std::cout << "kupaModle" << std::endl;

                shaderTemp2.use();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                shaderTemp2.setMat4("projection", projection);

                // camera/view transformation
                //glm::mat4 view = camera.GetViewMatrix();
                shaderTemp2.setMat4("view", view);


                // render the loaded model
                //glm::mat4 model = glm::mat4(1.0f);
                //model = glm::translate(model, m_transform.m_world_matrix); // translate it down so it's at the center of the scene
                //model = glm::scale(model, glm::vec3(m_transform.m_scale, m_transform.m_scale, m_transform.m_scale));	// it's a bit too big for our scene, so scale it down
                shaderTemp2.setMat4("model", m_transform.m_world_matrix);

                modelTemp.Draw(shaderTemp2);
            }
            
            if(tempRender == CODE)
            {
                //std::cout << "kupa" << std::endl;
                //shaderTemp2.use();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                //shaderTemp2.setMat4("projection", projection);
                //shaderTemp2.setMat4("view", view);
                //shaderTemp2.setMat4("model", m_transform.m_world_matrix);
                //shaderTemp.setMat4("model", model);

                glBindVertexArray(VAOSphere);
                glDrawElements(GL_TRIANGLES, sphereTemp.getIndexSize(), GL_UNSIGNED_INT, sphereTemp.getIndices());


            }

        }
        for (uint32_t i = 0; i < m_children.size(); ++i) {
            m_children[i]->render(false);
        }
    }
    
    void setProperties(Shader shader, unsigned int ttexture, glm::vec3 position, float scale, renderEnum predefined, Model model) {
        //shaderTemp2 = shader;
        texture = ttexture;
        m_transform.m_position = position;
        m_transform.m_scale = scale;
        tempRender = predefined;
        modelTemp = model;
    }
    
    void setPropertiesS(Shader shader, unsigned int ttexture, glm::vec3 position, float scale, renderEnum predefined, Sphere s, unsigned int VAO) {
        //shaderTemp2 = shader;
        texture = ttexture;
        m_transform.m_position = position;
        m_transform.m_scale = scale;
        tempRender = predefined;
        sphereTemp.set(s.getRadius(), s.getSectorCount(), s.getStackCount(), false);
        VAOSphere = VAO;
    }

    void setPropertiesSU(Sphere s) {
        sphereTemp.set(s.getRadius(), s.getSectorCount(), s.getStackCount(), false);
    }


    
    void setRotation(float x, float y, float z)
    {
        m_transform.x_rotation_angle = x;
        m_transform.y_rotation_angle = y;
        m_transform.z_rotation_angle = z;
    }

    void setScale(float x)
    {
        m_transform.m_scale = x;
    }

    void setRound(float thspeed)
    {
        float theta = 0.0;
        theta = theta + thspeed;

        if (theta >= (2.0 * 3.14159))
            theta = theta - (2.0 * 3.14159);

        m_transform.m_position.x = m_transform.m_position.x * glm::cos(theta) - m_transform.m_position.z * glm::sin(theta);
        m_transform.m_position.z = m_transform.m_position.x * glm::sin(theta) + m_transform.m_position.z * glm::cos(theta);
    }

    void setRoundV(float thspeed)
    {
        float theta = 0.0;
        theta = theta + thspeed;

        if (theta >= (2.0 * 3.14159))
            theta = theta - (2.0 * 3.14159);

        m_transform.m_position.x = m_transform.m_position.x * glm::cos(theta) - m_transform.m_position.y * glm::sin(theta);
        m_transform.m_position.y = m_transform.m_position.x * glm::sin(theta) + m_transform.m_position.y * glm::cos(theta);
    }


    void update_transform() {
        m_transform.m_world_matrix = m_transform.get_combined_matrix();
        m_dirty = true;
    }
    Transform& get_transform() {
        return m_transform;
    }

    void spinMeRound(float y) {

        m_transform.y_rotation_angle += y;

    }

};