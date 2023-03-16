#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include <stdio.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

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

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

#include "stb_image.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "D:\Users\wojci\Source\assignment-2-the-solar-system-RagnarKycner\build\src\SceneGraph.h"
#include "SceneGraph.h"
//#include "D:\Users\wojci\Source\assignment-2-the-solar-system-RagnarKycner\build\src\Camera.h"
#include "Camera.h"
//#include "D:\Users\wojci\Source\assignment-2-the-solar-system-RagnarKycner\build\src\Shader.h"
#include "Shader.h"
//#include "D:\Users\wojci\Source\assignment-2-the-solar-system-RagnarKycner\build\src\Model.h"
#include "Model.h"
//#include "D:\Users\wojci\Source\assignment-2-the-solar-system-RagnarKycner\build\src\Sphere.h"
#include "Sphere.h"
#include "D:\Users\wojci\Source\assignment-2-the-solar-system-RagnarKycner\build\src\Shared.h"
//#include "Shared.h"

static void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
GLuint loadTexture(char const* filePath);

#define M_PI 3.14159265358979323846264338327


// settings
//const unsigned int SCR_WIDTH = 1080;
//const unsigned int SCR_HEIGHT = 720;

// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool show_Interface = true;
bool show_Interface_flag = false;

bool show_Wireframe = false;
bool show_Wireframe_flag = false;

//time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float current_time = 0.0f;
float passed_time = 0.0f;
float last_time = 0.0f;
float unprocessed_time = 0.0f;
float frame_time = 0.0f;
bool should_render = true;

float vertices[] = {
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(0.0f,  0.0f,  -5.0f),
    glm::vec3(0.0f, 2.5f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),

    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)

};

bool change_radius_flag = false;
bool change_sectors_flag = false;
bool change_stacks_flag = false;


float rotofsys = 0.0f;


int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "zad2", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSwapInterval(1); // Enable vsync

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        spdlog::error("Failed to initialize OpenGL loader!");
        return 1;
    }
    spdlog::info("Successfully initialized OpenGL loader!");

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Model ourModelTp("D:/Users/wojci/Source/assignment-2-the-solar-system-RagnarKycner/build/src/res/models/tp.obj");
    Model ourModelTp("../../res/models/tp.obj");
    //Model ourModelSphere("D:/Users/wojci/Source/assignment-2-the-solar-system-RagnarKycner/build/src/res/models/sphere.obj");
    Model ourModelSphere("../../res/models/sphere.obj");
    //Model ourModelOrbit("D:/Users/wojci/Source/assignment-2-the-solar-system-RagnarKycner/build/src/res/models/orbit.obj");
    Model ourModelOrbit("../../res/models/orbit.obj");

    Model ourModelZero("../../res/models/Zero.obj");

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("../../src/vertexShader.vert", "../../src/fragmentShader.frag");
    Shader ourShaderModel("../../src/vertexShaderModel.vert", "../../src/fragmentShaderModel.frag");

    unsigned int textureTM = loadTexture("../../res/textures/tm.png");
    unsigned int textureWhite = loadTexture("../../res/textures/aaaa.png");
    unsigned int textureDark = loadTexture("../../res/textures/dddd.png");
    unsigned int textureStone = loadTexture("../../res/textures/stone.jpg");
    unsigned int textureBoom = loadTexture("../../res/textures/boom.png");
    unsigned int textureRak = loadTexture("../../res/textures/rak.png");

    glm::vec3 PosZero(0.0f, 0.0f, 0.0f);
    glm::vec3 Pos1planet(-25.0f, 0.0f, 0.0f);
    glm::vec3 Pos2planet(40.0f, 0.0f, 0.0f);
    glm::vec3 Pos3planet(-55.0f, 0.0f, 0.0f);
    glm::vec3 Pos4planet(75.0f, 0.0f, 0.0f);
    glm::vec3 PosFARplanet(75.0f, 0.0f, 0.0f);

    glm::vec3 PosKLeft(-15.0f, 0.0f, 0.0f);
    glm::vec3 PosKRight(25.0f, 0.0f, 0.0f);
    glm::vec3 PosKFront(0.0f, 0.0f, 35.0f);

    std::shared_ptr<SceneGraphNode> root_node;
    std::shared_ptr<SceneGraphNode> star;
    std::shared_ptr<SceneGraphNode> p1;
    std::shared_ptr<SceneGraphNode> p1k1;
    std::shared_ptr<SceneGraphNode> p1k2;
    std::shared_ptr<SceneGraphNode> p2;
    std::shared_ptr<SceneGraphNode> p2Sphere;
    std::shared_ptr<SceneGraphNode> p3;
    std::shared_ptr<SceneGraphNode> p3k1;
    std::shared_ptr<SceneGraphNode> p3k2;
    std::shared_ptr<SceneGraphNode> p3k3;
    std::shared_ptr<SceneGraphNode> p4;
    std::shared_ptr<SceneGraphNode> p4k1;
    std::shared_ptr<SceneGraphNode> p4k2;

    std::shared_ptr<SceneGraphNode> o1;
    std::shared_ptr<SceneGraphNode> o2;
    std::shared_ptr<SceneGraphNode> o3;
    std::shared_ptr<SceneGraphNode> o4;
    
    root_node = std::make_shared<SceneGraphNode>();
    star = std::make_shared<SceneGraphNode>();
    p1 = std::make_shared<SceneGraphNode>();
    p1k1 = std::make_shared<SceneGraphNode>();
    p1k2 = std::make_shared<SceneGraphNode>();
    p2 = std::make_shared<SceneGraphNode>();
    p2Sphere = std::make_shared<SceneGraphNode>();
    p3 = std::make_shared<SceneGraphNode>();
    p3k1 = std::make_shared<SceneGraphNode>();
    p3k2 = std::make_shared<SceneGraphNode>();
    p3k3 = std::make_shared<SceneGraphNode>();
    p4 = std::make_shared<SceneGraphNode>();
    p4k1 = std::make_shared<SceneGraphNode>();
    p4k2 = std::make_shared<SceneGraphNode>();

    o1 = std::make_shared<SceneGraphNode>();
    o2 = std::make_shared<SceneGraphNode>();
    o3 = std::make_shared<SceneGraphNode>();
    o4 = std::make_shared<SceneGraphNode>();
    

    

    root_node->add_child(star);
    star->setProperties(ourShaderModel, textureTM, PosZero, 0.2f, MODEL, ourModelZero);



    star->add_child(p1);
    p1->setProperties(ourShaderModel, textureRak, Pos1planet, 0.5f, MODEL, ourModelSphere);

    p1->setRotation(0.0f, 0.0f, 15.0f);

    star->add_child(o1);
    o1->setProperties(ourShaderModel, textureRak, PosZero, 0.25f, MODEL, ourModelOrbit);

    p1->add_child(p1k1);
    p1k1->setProperties(ourShaderModel, textureDark, PosKLeft, 0.2f, MODEL, ourModelTp);
    
    p1->add_child(p1k2);
    p1k2->setProperties(ourShaderModel, textureDark, PosKRight, 0.2f, MODEL, ourModelTp);
    


    star->add_child(p2);
    p2->setProperties(ourShaderModel, textureStone, Pos2planet, 0.5f, MODEL, ourModelSphere);

    p2->setRotation(0.0f, 0.0f, -25.0f);

    star->add_child(o2);
    o2->setProperties(ourShaderModel, textureStone, PosZero, 0.40f, MODEL, ourModelOrbit);



    star->add_child(p3);
    p3->setProperties(ourShaderModel, textureTM, Pos3planet, 0.5f, MODEL, ourModelSphere);

    p3->setRotation(0.0f, 0.0f, -35.0f);

    star->add_child(o3);
    o3->setProperties(ourShaderModel, textureTM, PosZero, 0.55f, MODEL, ourModelOrbit);

    p3->add_child(p3k1);
    p3k1->setProperties(ourShaderModel, textureDark, PosKLeft, 0.2f, MODEL, ourModelTp);

    p3->add_child(p3k2);
    p3k2->setProperties(ourShaderModel, textureDark, PosKRight, 0.2f, MODEL, ourModelTp);

    p3->add_child(p3k3);
    p3k3->setProperties(ourShaderModel, textureDark, PosKFront, 0.2f, MODEL, ourModelTp);



    star->add_child(p4);
    p4->setProperties(ourShaderModel, textureBoom, Pos4planet, 0.5f, MODEL, ourModelSphere);

    p4->setRotation(0.0f, 0.0f, 45.0f);

    star->add_child(o4);
    o4->setProperties(ourShaderModel, textureBoom, PosZero, 0.75f, MODEL, ourModelOrbit);

    p4->add_child(p4k1);
    p4k1->setProperties(ourShaderModel, textureDark, PosKFront, 0.2f, MODEL, ourModelTp);

    p4->add_child(p4k2);
    p4k2->setProperties(ourShaderModel, textureDark, PosKLeft, 0.2f, MODEL, ourModelTp);
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int VBOSphere, VAOSphere;
    glGenVertexArrays(1, &VAOSphere);
    glGenBuffers(1, &VBOSphere);

    

    glBindVertexArray(VAOSphere);

    glBindBuffer(GL_ARRAY_BUFFER, VBOSphere);
    glBufferData(GL_ARRAY_BUFFER, sphereTemp.getInterleavedVertexSize(), sphereTemp.getInterleavedVertices(), GL_DYNAMIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    
    //normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    
    //texcords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    star->add_child(p2Sphere);
    p2Sphere->setPropertiesS(ourShaderModel, textureWhite, PosZero, 1.0f, CODE, sphereTemp, VAOSphere);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------

    //ourShader.use();
    //ourShader.setInt("texture1", 0);
    //ourShader.setInt("texture2", 1);
    
    //sphereR.set(1.0f, 20, 20, false);
    float newscale = 1.0f;
    bool newscaleflag = false;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        
        if (!show_Wireframe && show_Wireframe_flag) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //show_Polygon_flag = false;
            show_Wireframe = true;
        }
        if (show_Wireframe && !show_Wireframe_flag) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            show_Wireframe_flag = false;
            show_Wireframe = false;
        }


        if (change_sectors_flag || change_stacks_flag || change_radius_flag) {
            change_sectors_flag = false;
            change_stacks_flag = false;

            sphereTemp.set(radius, sectors, stacks, false);
            p2Sphere->setPropertiesSU(sphereTemp);

            glBindVertexArray(VAOSphere);

            glBindBuffer(GL_ARRAY_BUFFER, VBOSphere);
            glBufferData(GL_ARRAY_BUFFER, sphereTemp.getInterleavedVertexSize(), sphereTemp.getInterleavedVertices(), GL_DYNAMIC_DRAW);
            
        }
        

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        current_time = glfwGetTime();
        passed_time = current_time - last_time;
        last_time = current_time;
        unprocessed_time += passed_time;

        processInput(window);

        //star->spinMeRound(0.5);
        p1->spinMeRound(3.0f);
        p2->spinMeRound(5.0f);
        p3->spinMeRound(-3.0f);
        p4->spinMeRound(4.0f);

        p1->setRound(0.01f);
        p2->setRound(0.006f);
        p3->setRound(0.015f);
        p4->setRound(0.005f);


        p1k1->setRound(0.01f);
        p1k2->setRoundV(0.006f);

        p3k1->setRoundV(0.002f);
        p3k1->setRound(0.01f);
        p3k1->setRoundV(0.004f);

        p4k1->setRound(0.009f);
        p4k1->setRoundV(0.003f);

        p4->setScale(newscale);

            if (!newscaleflag) {
                newscale = newscale -0.01f;

                if (newscale < 0.2) {
                    newscaleflag = true;
                }
            }
            else
            {
                newscale = newscale + 0.01f;

                if (newscale > 2.0) {
                    newscaleflag = false;
                }

            }
            

        root_node->update_transform();

        root_node->update(Transform(), false);


        //cout << p1->m_transform.m_position.x << "   " << p1->m_transform.m_position.x << endl;

        if (unprocessed_time >= frame_time) {
            should_render = true;
            unprocessed_time -= frame_time;

        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (should_render) {
            should_render = false;
            
            root_node->render(false);

        }
        glfwPollEvents();


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        if(show_Interface)
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Interface", &show_Interface);                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Press 'I' to close Interface and full controll camera ");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Wireframe Mode", &show_Wireframe_flag);      // Edit bools storing our window open/close state

            if (ImGui::SliderFloat("radius", &radius, 1.f, 15.0f, "radius = %.3f")) {
                change_radius_flag = true;
            }
            if (ImGui::InputInt("input sectors", &sectors)) {
                change_sectors_flag = true;
            }
            if (ImGui::InputInt("input stacks", &stacks)) {
                change_stacks_flag = true;
            }

            if (ImGui::SliderFloat("rotation of system", &rotofsys, 0.0f, 360.0f, "rotation of system = %.3f")) {
                root_node->setRotation(0.0f, rotofsys, 0.0f);
                root_node->update_transform();
            }


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);

    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAOSphere);
    glDeleteBuffers(1, &VBOSphere);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //if (!show_Interface) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    //}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        if (!show_Interface_flag) {
            show_Interface_flag = true;
            if (show_Interface) {
                show_Interface = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else {
                show_Interface = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            }
        }
    }
    else
    {
        show_Interface_flag = false;
    }
}

std::string load_file(std::filesystem::path filepath) {
    std::ifstream file(filepath);
    std::string contents = "";
    std::string line = "";
    if (!file.good()) {
        std::cerr << "Could not load file " << filepath << '\n';
        return contents;
    }
    while (!file.eof()) {
        std::getline(file, line);
        contents.append(line + '\n');
    }
    return contents;
}


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}