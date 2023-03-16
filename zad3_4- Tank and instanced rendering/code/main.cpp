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

//#include "D:\Users\wojci\Source\assignment-3-the-housing-estate-RagnarKycner\build\src\SceneGraph.h"
#include "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/SceneGraph.h"
//#include "D:\Users\wojci\Source\assignment-3-the-housing-estate-RagnarKycner\build\src\Camera.h"
#include "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/Camera.h"
//#include "D:\Users\wojci\Source\assignment-3-the-housing-estate-RagnarKycner\build\src\Shader.h"
#include "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/Shader.h"
//#include "D:\Users\wojci\Source\assignment-3-the-housing-estate-RagnarKycner\build\src\Model.h"
#include "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/Model.h"
//#include "D:\Users\wojci\Source\assignment-3-the-housing-estate-RagnarKycner\build\src\Shared.h"
#include "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/Shared.h"

static void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
GLuint loadTexture(char const* filePath);
unsigned int loadCubemap(vector<std::string> faces);

void setRoundCentre(glm::vec3& pos, float thspeed);
void updatemissile();

#define M_PI 3.14159265358979323846264338327

//unsigned int amount = 40000;
//unsigned int textureRak = loadTexture("../../res/textures/rak.png");
//Model ourModelHouse("D:/Users/wojci/Source/assignment-3-the-housing-estate-RagnarKycner/res/models/house.obj");

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

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
// positions all containers
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
// world space positions of our cubes


bool change_radius_flag = false;
bool change_sectors_flag = false;
bool change_stacks_flag = false;

bool change_root_flag = false;
bool change_InstancedH_flag = false;
bool change_InstancedR_flag = false;

bool show_change_root_flag = false;
bool show_change_InstancedH_flag = false;
bool show_change_InstancedR_flag = false;



float rotofsys = 0.0f;



float offSGx = 0.0f;
float offSGy = 0.0f;
float offSGz = 0.0f;

float offsetHx = 0.0f;
float offsetHy = 0.0f;
float offsetHz = 0.0f;

float offHx = 10.0f;
float offHy = 0.0f;
float offHz = 0.0f;

float offRx = 0.0f;
float offRy = 0.0f;
float offRz = 0.0f;

int iter = 0;
int counter = 0;

float scaleSG = 1.0f;
float scaleH = 1.0f;
float scaleR = 1.0f;

float rotationSG = 0.0f;
float rotationH = 0.0f;
float rotationR = 0.0f;


bool pointLightFlag = true;
bool spotLight1Flag = true;
bool spotLight2Flag = true;
bool dirLightFlag = true;
float DirX = -0.2f;
float DirY = -1.0f;
float DirZ = -0.3f;
float pointAmbient[3] = { 0.07f, 0.07f, 0.07f };
float spotAmbient[3] = { 0.7f, 0.7f, 0.7f };
float spotAmbient1[3] = { 0.7f, 0.7f, 0.7f };
float dirAmbient[3] = { 0.7f, 0.7f, 0.7f };

glm::vec3 spotDirection(1.0f, 0.0f, 0.0f);
glm::vec3 spot2Direction(0.0f, 1.0f, 0.0f);


float constant = 1.0f;
float linear = 0.01;
float quadratic = 0.01;
bool* k = new bool(true);

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 PosZero(0.0f, 0.0f, 0.0f);
glm::vec3 PosZero2(-5.0f, 0.0f, -5.0f);
glm::vec3 PosZero3(5.0f, 2.2f, 7.5f);

glm::vec3 PosLight1(5.0f, 2.2f, 7.5f);
glm::vec3 PosSpot1(1.0f, 1.f, 5.0f);
glm::vec3 PosSpot2(2.0f, 1.0f, -15.0f);


glm::vec3 PosDir(0.0f, 20.0f, .0f);


glm::vec3 PosCam(0.0f, 20.0f, 20.f);


float ttrot = 0.0f;
float ttrotupdown = 0.0;

float EGrav = 0.69f;
float VGrav = 50.0f;

glm::vec3 postank(0.0f, 0.0f, 0.0f);
glm::vec3 postrack(0.0f, 0.0f, -10.0f);
glm::vec3 postankoff(0.0f, 0.0f, 0.0f);
glm::vec3 posmissile(0.0f, 208.0f, 250.0f);

glm::vec3 tankcampos(0.0f, 2.5, -6.9f);
bool missileflag = false;
bool tankcontrollflag = false;
bool tankcontroll = false;

bool sterleftflag = false;
bool sterrightflag = false;


float Xcentre = 0.0f;
float Zcentre = 0.0f;
float X = 0.0f;
float Z = 0.0f;

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};



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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "zad4", NULL, NULL);
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
    //stbi_set_flip_vertically_on_load(true);

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


    Model ourModelFloor("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/floor.obj");
    Model ourModelHouse("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/house.obj");
    Model ourModelRoof("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/roof.obj");
    Model ourModelSphere("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/sphere.obj");


    Model czokZero("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/Zero.obj");

    Model czokBase("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/tankbase.obj");
    Model czokTower("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/tanktower.obj");
    Model czokBarrel("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/tankbarrel.obj");
    Model czokBack("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/refractback.obj");
    Model czokLeft("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/left.obj");
    Model czokRight("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/right.obj");
    Model czokMissile("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/models/missile.obj");

    Shader ourShaderModel("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/vertexShaderModel.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/fragmentShaderModel.frag");
    Shader ourShaderModelLeft("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/vertexShaderModel.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/fragmentShaderMove.frag");
    Shader ourShaderModelRight("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/vertexShaderModel.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/fragmentShaderMove.frag");
    Shader ourShaderInstanced("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/vertexShaderInstanced.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/fragmentShaderInstanced.frag");
    Shader lightingShader("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/lightingShader.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/lightingShader.frag");
    Shader skyboxShader("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/skyboxShader.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/skyboxShader.frag");
    Shader reflectShader("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/reflectShader.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/reflectShader.frag");
    Shader refrectShader("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/reflectShader.vert", "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/build/src/refractShader.frag");
    

    unsigned int textureTM = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/tm.png");
    unsigned int textureWhite = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/aaaa.png");
    unsigned int textureDark = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/dddd.png");
    unsigned int textureStone = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/stone.jpg");
    unsigned int textureBoom = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/boom.png");
    unsigned int textureRak = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/rak.png");

    unsigned int textureCamo = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/camo.jpg");
    
    //unsigned int textureTrack = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/track.jpg");
    //unsigned int textureTrack = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/trackalp.png");
    unsigned int textureTrack = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/trackalpsmall.png");
    
    //unsigned int textureTrack = loadTexture("D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/camo.jpg");



    unsigned int amount = 25;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];

    for (unsigned int i = 0; i < amount; i++)
    {

        glm::mat4 model = glm::mat4(1.0f);
        float x = offSGx + offHx * iter;
        float y = offSGy + offHy;
        float z = offSGz + offHz;
        model = glm::translate(model, glm::vec3(x, y, z));

        offHz += -10.0;
        counter++;

        if (counter == 5) {
            iter++;
            offHz = 0.0f;
            counter = 0;
        }

        float scale = scaleSG * 0.02;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        //float rotAngle = static_cast<float>((rand() % 360));
        //model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    glm::mat4* modelRMatrices;
    modelRMatrices = new glm::mat4[amount];

    offHx = 10.0f;
    offHy = 0.0f;
    offHz = 0.0f;
    iter = 0;
    counter = 0;

    for (unsigned int i = 0; i < amount; i++)
    {

        glm::mat4 model = glm::mat4(1.0f);
        float x = offSGx + offRx + offHx * iter;
        float y = offSGy + offRy + offHy;
        float z = offSGz + offRz + offHz;
        model = glm::translate(model, glm::vec3(x, y, z));

        offHz += -10.0;
        counter++;

        if (counter == 5) {
            iter++;
            offHz = 0.0f;
            counter = 0;
        }

        float scale = scaleSG * 0.02;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        //float rotAngle = static_cast<float>((rand() % 360));
        //model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelRMatrices[i] = model;
    }





    // configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)----------------


    for (unsigned int i = 0; i < ourModelHouse.meshes.size(); i++)
    {
        unsigned int VAO = ourModelHouse.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    unsigned int bufferR;
    glGenBuffers(1, &bufferR);
    glBindBuffer(GL_ARRAY_BUFFER, bufferR);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelRMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < ourModelRoof.meshes.size(); i++)
    {
        unsigned int VAO = ourModelRoof.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }



    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> faces
    {
        "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/skybox/right.jpg",
        "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/skybox/left.jpg",
        "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/skybox/top.jpg",
        "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/skybox/bottom.jpg",
        "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/skybox/front.jpg",
        "D:/Users/wojci/Source/assignment-4-the-interactive-scene-RagnarKycner/res/textures/skybox/back.jpg"
    };

    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    lightingShader.use();

    offHx = 5.0f;
    offHy = 0.0f;
    offHz = 0.0f;
    offRx = 0.0f;
    offRy = 0.0f;
    offRz = 0.0f;
    iter = 0;
    counter = 0;

    std::shared_ptr<SceneGraphNode> root_node;
    std::shared_ptr<SceneGraphNode> floor;
    std::shared_ptr<SceneGraphNode> house;
    std::shared_ptr<SceneGraphNode> housesInstanced;
    std::shared_ptr<SceneGraphNode> roofsInstanced;
    std::shared_ptr<SceneGraphNode> roof;

    std::shared_ptr<SceneGraphNode> tankPosZero1;
    std::shared_ptr<SceneGraphNode> tankCam;


    std::shared_ptr<SceneGraphNode> tankzero;
    std::shared_ptr<SceneGraphNode> tankzero2;
    std::shared_ptr<SceneGraphNode> tankzero3;
    std::shared_ptr<SceneGraphNode> tankbase;
    std::shared_ptr<SceneGraphNode> tanktower;
    std::shared_ptr<SceneGraphNode> tanktowerback;
    std::shared_ptr<SceneGraphNode> tanktowerbarrel;
    std::shared_ptr<SceneGraphNode> tankright;
    std::shared_ptr<SceneGraphNode> tanktleft;

    std::shared_ptr<SceneGraphNode> tankmissile;

    std::shared_ptr<SceneGraphNode> light1;
    std::shared_ptr<SceneGraphNode> light2;
    std::shared_ptr<SceneGraphNode> light3;
    std::shared_ptr<SceneGraphNode> light4;

    root_node = std::make_shared<SceneGraphNode>();
    floor = std::make_shared<SceneGraphNode>();
    house = std::make_shared<SceneGraphNode>();
    housesInstanced = std::make_shared<SceneGraphNode>();
    roofsInstanced = std::make_shared<SceneGraphNode>();
    roof = std::make_shared<SceneGraphNode>();


    tankPosZero1 = std::make_shared<SceneGraphNode>();
    tankCam = std::make_shared<SceneGraphNode>();


    tankzero = std::make_shared<SceneGraphNode>();
    tankzero2 = std::make_shared<SceneGraphNode>();
    tankzero3 = std::make_shared<SceneGraphNode>();
    tankbase = std::make_shared<SceneGraphNode>();
    tanktower = std::make_shared<SceneGraphNode>();
    tanktowerback = std::make_shared<SceneGraphNode>();
    tanktowerbarrel = std::make_shared<SceneGraphNode>();
    tankright = std::make_shared<SceneGraphNode>();
    tanktleft = std::make_shared<SceneGraphNode>();

    tankmissile = std::make_shared<SceneGraphNode>();

    light1 = std::make_shared<SceneGraphNode>();
    light2 = std::make_shared<SceneGraphNode>();
    light3 = std::make_shared<SceneGraphNode>();
    light4 = std::make_shared<SceneGraphNode>();

    root_node->add_child(floor);
    floor->setProperties(ourShaderModel, textureStone, PosZero, .1f, 1, ourModelFloor);


    root_node->add_child(tankPosZero1);
    tankPosZero1->setProperties(lightingShader, textureCamo, postank, 1.0f, 1, czokZero);


    root_node->add_child(tankzero);
    tankzero->setProperties(lightingShader, textureCamo, postank, 1.f, 1, czokZero);

    tankzero->add_child(tankzero2);
    tankzero2->setProperties(lightingShader, textureCamo, PosZero, 1.f, 1, czokZero);

    tankzero2->add_child(tankzero3);
    tankzero3->setProperties(lightingShader, textureCamo, PosZero, 1.f, 1, czokZero);



    tankzero3->add_child(tankbase);
    tankbase->setProperties(lightingShader, textureCamo, PosZero, 0.01f, 1, czokBase);

    tankbase->add_child(tankCam);
    tankCam->setProperties(lightingShader, textureCamo, tankcampos, 1.f, 1, czokZero);

    tankbase->add_child(tanktower);
    tanktower->setProperties(reflectShader, cubemapTexture, PosZero, 1.0f, 3, czokTower);

    tanktower->add_child(tanktowerback);
    tanktowerback->setProperties(refrectShader, textureCamo, PosZero, 1.0f, 3, czokBack);

    tanktower->add_child(tanktowerbarrel);
    tanktowerbarrel->setProperties(lightingShader, textureCamo, PosZero, 1.0f, 1, czokBarrel);

    tankbase->add_child(tankright);
    tankright->setProperties(ourShaderModelRight, textureTrack, PosZero, 1.0f, 1, czokRight);

    tankbase->add_child(tanktleft);
    tanktleft->setProperties(ourShaderModelLeft, textureTrack, PosZero, 1.0f, 1, czokLeft);


    tanktowerbarrel->add_child(tankmissile);
    tankmissile->setProperties(lightingShader, textureRak, PosZero +posmissile, 1.1f, 1, czokMissile);


    //root_node->add_child(house);
    //house->setProperties(lightingShader, textureRak, -PosZero2, .02f, 1, ourModelHouse);

    //house->add_child(roof);
    //roof->setProperties(lightingShader, textureTM, -PosZero2, 1.f, 1, ourModelRoof);



    root_node->add_child(housesInstanced);
    housesInstanced->setProperties(ourShaderInstanced, textureRak, PosZero, .02f, 2, ourModelHouse);

    root_node->add_child(roofsInstanced);
    roofsInstanced->setProperties(ourShaderInstanced, textureTM, PosZero, .02f, 2, ourModelRoof);







    root_node->add_child(light1);
    light1->setProperties(ourShaderModel, textureTM, PosZero3, 0.01f, 1, ourModelSphere);

    root_node->add_child(light2);
    light2->setProperties(ourShaderModel, textureTM, PosSpot1, 0.003f, 1, ourModelRoof);
    light2->m_transform.z_rotation_angle = 270.0f;
    light2->m_transform.m_position.y += 0.5;
    root_node->add_child(light3);
    light3->setProperties(ourShaderModel, textureTM, PosSpot2, 0.003f, 1, ourModelRoof);
    light3->m_transform.z_rotation_angle = 180.0f;


    //root_node->add_child(light4);
    //light4->setProperties(ourShaderModel, textureRak, PosZero, 0.1f, 1, ourModelRoof);
    //light4->setRotation(DirX*180, DirY*180, DirZ*180);

    glm::vec3 offsetCametatank(0.0f, 2.5, -6.9f);
    glm::vec3 offsetCametatank2(0.0f, 3., 0.0f);

    while (!glfwWindowShouldClose(window))
    {
        
        if (change_InstancedH_flag || change_root_flag) {

            for (unsigned int i = 0; i < amount; i++)
            {

                glm::mat4 model = glm::mat4(1.0f);
                float x = offSGx + offsetHx + offHx * iter;
                float y = offSGy + offsetHy + offHy;
                float z = offSGz + offsetHz + offHz;
                model = glm::translate(model, glm::vec3(x, y, z));

                offHz += -5.0;
                counter++;

                if (counter == 200) {
                    iter++;
                    offHz = 0.0f;
                    counter = 0;
                }

                float scale = scaleSG * scaleH * 0.02;
                model = glm::scale(model, glm::vec3(scale));

                // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
                //float rotAngle = static_cast<float>((rand() % 360));
                //model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
                //float rotAngle = rotationH;
                //model = glm::rotate(model, rotAngle, glm::vec3(1.0f, 1.0f, 1.0f));

                // 4. now add to list of matrices
                modelMatrices[i] = model;
            }

            // configure instanced array
            // -------------------------
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

            // set transformation matrices as an instance vertex attribute (with divisor 1)----------------


            for (unsigned int i = 0; i < ourModelHouse.meshes.size(); i++)
            {
                unsigned int VAO = ourModelHouse.meshes[i].VAO;
                glBindVertexArray(VAO);
                // set attribute pointers for matrix (4 times vec4)
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);
            }

            iter = 0;
            counter = 0;
        }

        if (change_InstancedR_flag || change_InstancedH_flag || change_root_flag) {
            change_InstancedR_flag = false;
            change_InstancedH_flag = false;
            change_root_flag = false;

            for (unsigned int i = 0; i < amount; i++)
            {

                glm::mat4 model = glm::mat4(1.0f);
                float x = offSGx + offRx + offsetHx + offHx * iter;
                float y = offSGy + offRy + offsetHy + offHy;
                float z = offSGz + offRz + offsetHz + offHz;
                model = glm::translate(model, glm::vec3(x, y, z));

                offHz += -5.0;
                counter++;

                if (counter == 200) {
                    iter++;
                    offHz = 0.0f;
                    counter = 0;
                }

                float scale = scaleSG * scaleH * scaleR * 0.02;
                model = glm::scale(model, glm::vec3(scale));

                // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
                //float rotAngle = static_cast<float>((rand() % 360));
                //model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
                //float rotAngle = rotationR;
                //model = glm::rotate(model, rotAngle, glm::vec3(1.0f, 1.0f, 1.0f));

                // 4. now add to list of matrices
                modelRMatrices[i] = model;
            }

            // configure instanced array
            // -------------------------

            glBindBuffer(GL_ARRAY_BUFFER, bufferR);
            glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelRMatrices[0], GL_STATIC_DRAW);

            // set transformation matrices as an instance vertex attribute (with divisor 1)----------------


            for (unsigned int i = 0; i < ourModelRoof.meshes.size(); i++)
            {
                unsigned int VAO = ourModelRoof.meshes[i].VAO;
                glBindVertexArray(VAO);
                // set attribute pointers for matrix (4 times vec4)
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);
            }

            iter = 0;
            counter = 0;
        }
        

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        current_time = glfwGetTime();
        passed_time = current_time - last_time;
        last_time = current_time;
        unprocessed_time += passed_time;

        processInput(window);
        if (tankcontroll) {

            tanklook = true;

            


            //ourShaderModelMove.use();
           



            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

                ourShaderModelLeft.use();
                ourShaderModelLeft.setFloat("time", -unprocessed_time);

                ourShaderModelRight.use();
                ourShaderModelRight.setFloat("time", unprocessed_time);

                sterleftflag = true;

                //tankzero2->m_transform.y_rotation_angle += 0.5;
                tankbase->m_transform.y_rotation_angle -= 1.;

            }
            else
            {
                sterleftflag = false;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

                ourShaderModelLeft.use();
                ourShaderModelLeft.setFloat("time", unprocessed_time);

                ourShaderModelRight.use();
                ourShaderModelRight.setFloat("time", -unprocessed_time);

                sterrightflag = true;

                //tankzero2->m_transform.y_rotation_angle -= 0.5;
                tankbase->m_transform.y_rotation_angle += 1.;
            }
            else
            {
                sterrightflag = false;
            }

            glm::vec3 front2;
            front2.x = 0.1*(cos(glm::radians(yawn)) * cos(glm::radians(-10.0)));
            front2.y = (sin(glm::radians(-10.0)));
            //front.y = 0;
            front2.z = 0.1*(sin(glm::radians(yawn)) * cos(glm::radians(-10.0)));
            //front2 = glm::normalize(front2);

            //cout << front2.x << "    " << front2.z << "  " << endl;

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

                //postankoff.z += 0.1;
                //tankzero3->m_transform.m_position.z += 0.1;
                tankbase->m_transform.m_position.x += front2.x;
                tankbase->m_transform.m_position.z += front2.z;
                

                ourShaderModelLeft.use();

                if (sterleftflag) {
                    ourShaderModelLeft.setFloat("time", 0.5*unprocessed_time);
                }
                else
                {
                    ourShaderModelLeft.setFloat("time", unprocessed_time);
                }
                

                ourShaderModelRight.use();
                
                if (sterrightflag) {
                    ourShaderModelRight.setFloat("time", 0.5*unprocessed_time);
                }
                else
                {
                    ourShaderModelRight.setFloat("time", unprocessed_time);
                }


            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

                //postankoff.z -= 0.1;
                //tankzero3->m_transform.m_position.z -= 0.1;
                tankbase->m_transform.m_position.x -= front2.x;
                tankbase->m_transform.m_position.z -= front2.z;
               

                ourShaderModelLeft.use();

                if (sterleftflag) {
                    ourShaderModelLeft.setFloat("time", -1.5 * unprocessed_time);
                }
                else
                {
                    ourShaderModelLeft.setFloat("time", -unprocessed_time);
                }



                ourShaderModelRight.use();

                if (sterrightflag) {
                    ourShaderModelRight.setFloat("time", -1.5 * unprocessed_time);
                }
                else
                {
                    ourShaderModelRight.setFloat("time", -unprocessed_time);
                }
            }

            //tankbase->setProperties(lightingShader, textureCamo, postank + postankoff, 0.01f, 1, czokBase);
            //Xcentre = tankPosZero2->m_transform.m_position.x * glm::cos(tankPosZero1->m_transform.y_rotation_angle) - tankPosZero2->m_transform.m_position.z * glm::sin(tankPosZero1->m_transform.y_rotation_angle);
            //Zcentre = tankPosZero2->m_transform.m_position.x * glm::sin(tankPosZero1->m_transform.y_rotation_angle) + tankPosZero2->m_transform.m_position.z * glm::cos(tankPosZero1->m_transform.y_rotation_angle);

            //cout << Xcentre << "   " << Zcentre << endl;

            //tankbase->m_transform.m_position.x = Xcentre;
            //tankbase->m_transform.m_position.z = Zcentre;

            //offsetCametatank.x = tankzero3->m_transform.m_position.x * glm::cos(tankzero2->m_transform.y_rotation_angle) - tankzero3->m_transform.m_position.z * glm::sin(tankzero2->m_transform.y_rotation_angle);
            //offsetCametatank.z = tankzero3->m_transform.m_position.x * glm::sin(tankzero2->m_transform.y_rotation_angle) + tankzero3->m_transform.m_position.z * glm::cos(tankzero2->m_transform.y_rotation_angle);

            //X = tankzero3->m_transform.m_position.x + (Xcentre - tankzero3->m_transform.m_position.x) * glm::cos(tankzero2->m_transform.y_rotation_angle) - (Zcentre - tankzero3->m_transform.m_position.z) * glm::sin(tankzero2->m_transform.y_rotation_angle);
            //Z = tankzero3->m_transform.m_position.z + (Xcentre - tankzero3->m_transform.m_position.x) * glm::cos(tankzero2->m_transform.y_rotation_angle) - (Zcentre - tankzero3->m_transform.m_position.z) * glm::sin(tankzero2->m_transform.y_rotation_angle);
            


            //offsetCametatank.x = X;
            //offsetCametatank.y = 3.;
            //offsetCametatank.z = Z;

            //cout << offsetCametatank.x << "  " << offsetCametatank.z  << endl;
            //cout << tankzero2->m_transform.y_rotation_angle+ 90 << endl;
           
            yawn = tankbase->m_transform.y_rotation_angle + 90;

            glm::vec3 front;
            front.x = -10.5*(cos(glm::radians(yawn)) * cos(glm::radians(-10.0)));
            front.y = -15*(sin(glm::radians(-10.0)));
            //front.y = 0;
            front.z = -10.5*(sin(glm::radians(yawn)) * cos(glm::radians(-10.0)));

            tankpos = tankbase->m_transform.m_position + offsetCametatank2 + front;

        }
        else
        {
            tanklook = false;
        }
        lightingShader.use();


        //cout << tankbase->m_transform.z_rotation_angle << "   " << tankzero->m_transform.z_rotation_angle << "   " << tankzero2->m_transform.z_rotation_angle << "   " << tankzero3->m_transform.z_rotation_angle  << "   " << endl;// tankbase->m_transform.m_position << "   "
        
        //if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        //    change_InstancedH_flag = true;
        //}


            

        root_node->setTransform(offSGx, offSGy, offSGz);
        root_node->setScale(scaleSG);
        light1->setRound(0.0075);

        PosLight1 = light1->m_transform.m_position;
        light4->setRotation(DirX * 180, DirY * 180, DirZ * 180);
        light4->m_transform.m_position = PosDir;


        tanktower->setRotation(0, ttrot, 0);
        tanktowerbarrel->setRotation(ttrotupdown, 0, 0);

        //cout << camera.Front.x << " " << camera.Front.y << " " << camera.Front.z << " " << camera.Front.b << " " << camera.Front.g << " " << camera.Front.p << " " << camera.Front.r << " " << camera.Front.s << " " << camera.Front.t << " " << endl;

        //cout << ttrotupdown << "  " << endl;

        if (missileflag) {

            tankmissile->m_transform.m_position.y -= EGrav;
            tankmissile->m_transform.m_position.z += VGrav;

            EGrav += 0.15f;
            VGrav -= 0.33f;


            if (tankmissile->m_transform.m_position.y <= -3) {
                tankmissile->m_transform.m_position = postank + posmissile;
                missileflag = false;

                EGrav = 0.70f;
                VGrav = 50.0f;
            }
        }


        root_node->update_transform();

        root_node->update(Transform(), false);

        if (unprocessed_time >= frame_time) {
            should_render = true;
            unprocessed_time -= frame_time;

        }

        glClearColor(0.00f, 0.05f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightingShader.use();

        lightingShader.setFloat("material.shininess", 50.0f);

        // directionalLight
        lightingShader.setBool("dirLight[0].flag", dirLightFlag);
        lightingShader.setVec3("dirLight[0].direction", DirX, DirY, DirZ);
        lightingShader.setVec3("dirLight[0].ambient", dirAmbient[0], dirAmbient[1], dirAmbient[2]);
        lightingShader.setVec3("dirLight[0].diffuse", 0.7f, 0.7f, 0.7f);
        lightingShader.setVec3("dirLight[0].specular", 0.7f, 0.7f, 0.7f);

        // point light 1
        lightingShader.setBool("pointLights[0].flag", pointLightFlag);
        lightingShader.setVec3("pointLights[0].position", PosLight1);
        lightingShader.setVec3("pointLights[0].ambient", pointAmbient[0], pointAmbient[1], pointAmbient[2]);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

        // spotLight
        /*
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 1.1f, 1.1f, 1.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        */
        // spotLight1
        lightingShader.setBool("spotLight[0].flag", spotLight1Flag);
        lightingShader.setVec3("spotLight[0].position", PosSpot1);
        lightingShader.setVec3("spotLight[0].direction", spotDirection);
        lightingShader.setVec3("spotLight[0].ambient", spotAmbient[0], spotAmbient[1], spotAmbient[2]);
        lightingShader.setVec3("spotLight[0].diffuse", 0.7f, 0.7f, 0.7f);
        lightingShader.setVec3("spotLight[0].specular", 0.7f, 0.7f, 0.7f);
        lightingShader.setFloat("spotLight[0].constant", 1.0f);
        lightingShader.setFloat("spotLight[0].linear", 0.09f);
        lightingShader.setFloat("spotLight[0].quadratic", 0.032f);
        lightingShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));

        // spotLight2
        lightingShader.setBool("spotLight[1].flag", spotLight2Flag);
        lightingShader.setVec3("spotLight[1].position", PosSpot2);
        lightingShader.setVec3("spotLight[1].direction", spot2Direction);
        lightingShader.setVec3("spotLight[1].ambient", spotAmbient1[0], spotAmbient1[1], spotAmbient1[2]);
        lightingShader.setVec3("spotLight[1].diffuse", 0.1f, 0.1f, 0.1f);
        lightingShader.setVec3("spotLight[1].specular", 0.1f, 0.1f, 0.1f);
        lightingShader.setFloat("spotLight[1].constant", 1.0f);
        lightingShader.setFloat("spotLight[1].linear", 0.09f);
        lightingShader.setFloat("spotLight[1].quadratic", 0.032f);
        lightingShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(15.0f)));



        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureWhite);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureStone);

        // render containers
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        if (should_render) {
            should_render = false;

            root_node->render(false);

        }

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default


        glfwPollEvents();


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        if (show_Interface)
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Interface", &show_Interface);                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Press 'I' to close Interface and controll camera ");               // Display some text (you can use a format strings too)
            ImGui::Text("Press 'T' to enable/disable controll tank ");               // Display some text (you can use a format strings too)
            ImGui::Text("");               // Display some text (you can use a format strings too)
            ImGui::Text("Tank control during enabled tank controll:");
            ImGui::Text("WSAD - moving");
            ImGui::Text("Num 5 - shoot");
            ImGui::Text("Num 4, Num 6 - rotation of turret");
            ImGui::Text("Num 1, Num 8 - angle of barrel");
            ImGui::Text("");




            /*

            if (ImGui::SliderFloat("offRx", &offRx, -15.f, 15.0f, "offRx = %.2f")) {
                change_InstancedR_flag = true;
            }
            if (ImGui::SliderFloat("offRy", &offRy, -15.f, 15.0f, "offRy = %.2f")) {
                change_InstancedR_flag = true;
            }
            if (ImGui::SliderFloat("offRz", &offRz, -15.f, 15.0f, "offRz = %.2f")) {
                change_InstancedR_flag = true;
            }

            if (ImGui::SliderFloat("offHx", &offsetHx, -15.f, 15.0f, "offHx = %.2f")) {
                change_InstancedH_flag = true;
            }
            if (ImGui::SliderFloat("offHy", &offsetHy, -15.f, 15.0f, "offHy = %.2f")) {
                change_InstancedH_flag = true;
            }
            if (ImGui::SliderFloat("offHz", &offsetHz, -15.f, 15.0f, "offHz = %.2f")) {
                change_InstancedH_flag = true;
            }

            if (ImGui::SliderFloat("offSGx", &offSGx, -15.f, 15.0f, "offSGx = %.2f")) {
                change_root_flag = true;
            }
            if (ImGui::SliderFloat("offSGy", &offSGy, -15.f, 15.0f, "offSGy = %.2f")) {
                change_root_flag = true;
            }
            if (ImGui::SliderFloat("offSGz", &offSGz, -15.f, 15.0f, "offSGz = %.2f")) {
                change_root_flag = true;
            }


            if (ImGui::SliderFloat("scaleR", &scaleR, 0.25f, 2.5f, "scaleR = %.2f")) {
                change_InstancedR_flag = true;
            }
            if (ImGui::SliderFloat("scaleH", &scaleH, 0.25f, 2.5f, "scaleH = %.2f")) {
                change_InstancedH_flag = true;
            }
            if (ImGui::SliderFloat("scaleSG", &scaleSG, 0.25f, 2.5f, "scaleSG = %.2f")) {
                change_root_flag = true;
            }


            ImGui::Checkbox("Pointlight", &pointLightFlag);
            ImGui::Checkbox("Spotlight", &spotLight1Flag);
            ImGui::Checkbox("Spotlight2", &spotLight2Flag);
            ImGui::Checkbox("Directional Light", &dirLightFlag);

            ImGui::ColorEdit3("Pointlight Color", pointAmbient);
            ImGui::ColorEdit3("Directional Light Color", dirAmbient);
            ImGui::SliderFloat("Directional Light xDir", &DirX, -1, 1);
            ImGui::SliderFloat("Directional Light yDir", &DirY, -1, 1);
            ImGui::SliderFloat("Directional Light zDir", &DirZ, -1, 1);

            

            ImGui::ColorEdit3("Spotlight 1 Color", spotAmbient);

            ImGui::SliderFloat("Spotlight 1 xDir", &spotDirection.x, -1, 1);
            ImGui::SliderFloat("Spotlight 1 yDir", &spotDirection.y, -1, 1);
            ImGui::SliderFloat("Spotlight 1 zDir", &spotDirection.z, -1, 1);


            ImGui::ColorEdit3("Spotlight 2 Color", spotAmbient1);

            ImGui::SliderFloat("Spotlight 2 xDir", &spot2Direction.x, -1, 1);
            ImGui::SliderFloat("Spotlight 2 yDir", &spot2Direction.y, -1, 1);
            ImGui::SliderFloat("Spotlight 2 zDir", &spot2Direction.z, -1, 1);
            */

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

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

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

    if (!show_Interface) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!tankcontroll) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        ttrot = ttrot - 1.0;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        ttrot = ttrot + 1.0;
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        if (ttrotupdown < 6) {
            ttrotupdown = ttrotupdown + 0.2;
        }
        
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        if (ttrotupdown > -4.5) {
            ttrotupdown = ttrotupdown - 0.2;
        }

    if (!missileflag && glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
        missileflag = true;

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

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        if (!tankcontrollflag) {
            tankcontrollflag = true;
            if (tankcontroll) {
                tankcontroll = false;
            }
            else {
                tankcontroll = true;

            }
        }
    }
    else
    {
        tankcontrollflag = false;
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

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}