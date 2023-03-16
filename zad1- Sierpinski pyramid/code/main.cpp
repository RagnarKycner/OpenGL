// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include <stdio.h>
#include "stb_image.h"
#include <cmath>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "D:\Users\wojci\Source\assignment-1-sierpinski-tetrahedron-menger-sponge-RagnarKycner\build\src\shaderClass.h"
#include <vector>


#define IMGUI_IMPL_OPENGL_LOADER_GLAD

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

ImVec4 pyramidColourRed = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
ImVec4 pyramidColourGreen = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
ImVec4 pyramidColourBlue = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
ImVec4 pyramidColourSth = ImVec4(0.0f, 1.0f, 1.0f, 1.00f);

std::vector<glm::vec3> BasicPiramid(GLfloat H) {
    GLfloat h = 3.0 * H * float(sqrt(2.0)) / 4.0,
        a = 3.0 * H / float(sqrt(6.0));
    std::vector<glm::vec3> pyramid =
    {
        glm::vec3(0.0, (float)(H / 2.0), 0.0),                                   glm::vec3(0.5f, 1.0f, 0.0f),   //top - 1
        glm::vec3(0.0, (float)(-H / 2.0), (float)(h * 2.0 / 3.0)),               glm::vec3(0.5f, 0.0f, 0.0f),   //back - 2
        glm::vec3((float)(-a / 2.0), (float)(-H / 2.0), (float)(-h / 3.0)),      glm::vec3(0.0f, 0.0f, 0.0f),   //left - 3
        glm::vec3((float)(a / 2.0), (float)(-H / 2.0), (float)(-h / 3.0)),       glm::vec3(1.0f, 0.0f, 0.0f)    //right - 4
    };
    return pyramid;
}

void pyramidBuilder(std::vector<glm::vec3>& pyramidVerticies, std::vector<GLuint>& pyramidIndicies, int recurency, int v0, int v1, int v2, int v3) {

    if (recurency == 0) {
        pyramidIndicies.push_back(v0 / 2);
        pyramidIndicies.push_back(v1 / 2);
        pyramidIndicies.push_back(v2 / 2);

        pyramidIndicies.push_back(v0 / 2);
        pyramidIndicies.push_back(v1 / 2);
        pyramidIndicies.push_back(v3 / 2);

        pyramidIndicies.push_back(v0 / 2);
        pyramidIndicies.push_back(v2 / 2);
        pyramidIndicies.push_back(v3 / 2);

        pyramidIndicies.push_back(v1 / 2);
        pyramidIndicies.push_back(v2 / 2);
        pyramidIndicies.push_back(v3 / 2);
        return;
    }

    recurency--;

    int tmpV1, tmpV2, tmpV3;
    int buffor0, buffor1;
    //powyzej 
    pyramidVerticies.push_back((pyramidVerticies[v0] + pyramidVerticies[v1]) * 0.5f);
    tmpV1 = pyramidVerticies.size() - 1;
    pyramidVerticies.push_back((pyramidVerticies[v0 + 1] + pyramidVerticies[v1 + 1]) * 0.5f);
    pyramidVerticies.push_back((pyramidVerticies[v0] + pyramidVerticies[v2]) * 0.5f);
    tmpV2 = pyramidVerticies.size() - 1;
    pyramidVerticies.push_back((pyramidVerticies[v0 + 1] + pyramidVerticies[v2 + 1]) * 0.5f);
    pyramidVerticies.push_back((pyramidVerticies[v0] + pyramidVerticies[v3]) * 0.5f);
    tmpV3 = pyramidVerticies.size() - 1;
    pyramidVerticies.push_back((pyramidVerticies[v0 + 1] + pyramidVerticies[v3 + 1]) * 0.5f);
    pyramidBuilder(pyramidVerticies, pyramidIndicies, recurency, v0, tmpV1, tmpV2, tmpV3);

    //tyl
    buffor0 = tmpV2;
    buffor1 = tmpV3;
    pyramidVerticies.push_back((pyramidVerticies[v1] + pyramidVerticies[v2]) * 0.5f);
    tmpV2 = pyramidVerticies.size() - 1;
    pyramidVerticies.push_back((pyramidVerticies[v1 + 1] + pyramidVerticies[v2 + 1]) * 0.5f);
    pyramidVerticies.push_back((pyramidVerticies[v1] + pyramidVerticies[v3]) * 0.5f);
    tmpV3 = pyramidVerticies.size() - 1;
    pyramidVerticies.push_back((pyramidVerticies[v1 + 1] + pyramidVerticies[v3 + 1]) * 0.5f);
    pyramidBuilder(pyramidVerticies, pyramidIndicies, recurency, tmpV1, v1, tmpV2, tmpV3);

    //lewy
    tmpV1 = tmpV3;
    pyramidVerticies.push_back((pyramidVerticies[v2] + pyramidVerticies[v3]) * 0.5f);
    tmpV3 = pyramidVerticies.size() - 1;
    pyramidVerticies.push_back((pyramidVerticies[v2 + 1] + pyramidVerticies[v3 + 1]) * 0.5f);
    pyramidBuilder(pyramidVerticies, pyramidIndicies, recurency, buffor0, tmpV2, v2, tmpV3);

    //prawy
    pyramidBuilder(pyramidVerticies, pyramidIndicies, recurency, buffor1, tmpV1, tmpV3, v3);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    //Zmienne g³ównie okno
    int windowWidth = 1000, windowHeight = 1000;
    ImVec4 pyramidColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    static int recurency = 0;
    int lastRecurency = recurency;
    static int xSpin = 0, ySpin = 0;

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Pyramid", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();

    //SHADER
    Shader shaderProgram("D:/graphic_test/OpenGLPAG/src/vertexShader.vert", "D:/graphic_test/OpenGLPAG/src/fragmentShader.frag");

    //PIRAMIDA
    GLfloat Height = 1;
    std::vector<glm::vec3> pyramid = BasicPiramid(Height);
    std::vector<GLuint> indices;
    pyramidBuilder(pyramid, indices, recurency, 0, 2, 4, 6);

    //BUFORY
    //inicjacja buforów (przekazanie tablicy do obiektu tablicy i wys³anie do bufora)
    GLuint VertexArrayObject, VertexBufferObject, ElementBufferObject;
    glGenVertexArrays(1, &VertexArrayObject); //zawsze przed vbo
    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);

    glBindVertexArray(VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, pyramid.size() * sizeof(glm::vec3), &pyramid[0], GL_DYNAMIC_DRAW); // - dla listy

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //TEXTURA
    //zaladowanie obrazka
    int imageWidth, imageHeight, numberOfColourChannels;
    stbi_set_flip_vertically_on_load(true); //nie trzeba ale to odwraca obrazek do dobrej pozycji
    unsigned char* bytes = stbi_load("C:/Users/wojci/Desktop/dash.png", &imageWidth, &imageHeight, &numberOfColourChannels, 0);
    //utworzenie tekstury
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0); //texture0, bo jest to pierwsza tekstura w bundle
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //generowanie tekstury
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    //generowanie mniejszych wymiarow jak z ikonkami
    glGenerateMipmap(GL_TEXTURE_2D);

    //odpinanie
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    //textura do shader-a
    GLuint tex0Uniform = glGetUniformLocation(shaderProgram.shaderProgram, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uniform, 0);

    //lokalizaja koloru wzgledem okna
    glViewport(0, 0, windowWidth, windowHeight);
    //wybranie koloru tla
    glClearColor(0.0f, 0.22f, 0.34f, 1.0f);
    //wywo³anie koloru tla
    glClear(GL_COLOR_BUFFER_BIT);
    //wymiana front bufor z back bufor
    glfwSwapBuffers(window);

    GLfloat scale = 1.0f;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 perspective = glm::mat4(1.0f);

    int viewLocation = glGetUniformLocation(shaderProgram.shaderProgram, "view");
    int modelLocation = glGetUniformLocation(shaderProgram.shaderProgram, "model");
    int perspectiveLocation = glGetUniformLocation(shaderProgram.shaderProgram, "proj");
    int colorLocation = glGetUniformLocation(shaderProgram.shaderProgram, "inColor");
    bool flag = false;

    glEnable(GL_DEPTH_TEST);

    float cos = 0.0f;

    //ODSWIEZANIE
    while (!glfwWindowShouldClose(window))
    {
        if (lastRecurency == recurency && flag == false) {
            flag = true;
            glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
            glBufferData(GL_ARRAY_BUFFER, pyramid.size() * sizeof(glm::vec3), &pyramid[0], GL_DYNAMIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);
        }

        glBindVertexArray(0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.Activate();

        //tekstura
        glBindTexture(GL_TEXTURE_2D, texture);

        model = glm::rotate(model, glm::radians((float)ySpin), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians((float)xSpin), glm::vec3(1.0f, 0.0f, 0.0f));

        int scaleLocation = glGetUniformLocation(shaderProgram.shaderProgram, "scale");
        glProgramUniform1f(shaderProgram.shaderProgram, scaleLocation, scale);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, glm::value_ptr(perspective));
        glm::vec3 color = glm::vec3(pyramidColour.x, pyramidColour.y, pyramidColour.z);
        glUniform3fv(colorLocation, 1, glm::value_ptr(color));

        //rysowanie buforem
        glBindVertexArray(VertexArrayObject);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        //event handler
        glfwPollEvents();

        //male menu
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Mini Menu");
        ImGui::SliderInt("Pyramid Recurency", &recurency, 0, 10);
        //ImGui::ColorEdit3("Pyramid colour", (float*)&pyramidColour);
        ImGui::SliderFloat("scale", &scale, 0, 1.5, "%.2f");
        ImGui::SliderInt("Y Spin", &ySpin, -10, 10);
        ImGui::SliderInt("X Spin", &xSpin, -10, 10);
        ImGui::End();

        if (lastRecurency != recurency) {
            flag = false;
            pyramid = BasicPiramid(Height);
            indices.clear();
            pyramidBuilder(pyramid, indices, recurency, 0, 2, 4, 6);
            lastRecurency = recurency;
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);

    }


    // zabijanie wszystkiego
    glDeleteVertexArrays(1, &VertexArrayObject);
    glDeleteBuffers(1, &VertexBufferObject);
    glDeleteBuffers(1, &ElementBufferObject);
    //glDeleteTextures(1, &texture);
    shaderProgram.Delete();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}