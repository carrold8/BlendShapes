// Created in part by following tutorials in the LearnOpenGL online textbook https://learnopengl.com/About
// Dear ImGui library used to create User Interface


#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include<list>
#include<algorithm>



// Assimp file loader

#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"


#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/norm.hpp>




/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define DEMO "ball.dae"                                                    // FaceMesh Number: 0
                                                                           
                                                                           
// Define the High_Res models                                              
#define NEUTRAL "High_Res_Blend/neutral.obj"                               // FaceMesh Index: 1        
// Middle blendshape models                                                
#define JAW_OPEN "High_Res_Blend/Mery_jaw_open.obj"                        // FaceMesh Index: 2             Weight Index: 0
#define KISS "High_Res_Blend/Mery_kiss.obj"                                // FaceMesh Index: 3             Weight Index: 1
// Delta Blendshape models                                                                                                
#define L_BROW_LOWER "High_Res_Blend/Mery_l_brow_lower.obj"                // FaceMesh Index: 4             Weight Index: 2
#define L_BROW_NARROW "High_Res_Blend/Mery_l_brow_narrow.obj"              // FaceMesh Index: 5             Weight Index: 3
#define L_BROW_RAISE "High_Res_Blend/Mery_l_brow_raise.obj"                // FaceMesh Index: 6             Weight Index: 4
#define LEFT_CLOSED "High_Res_Blend/Mery_l_eye_closed.obj"                 // FaceMesh Index: 7             Weight Index: 5
#define L_LOWER_O "High_Res_Blend/Mery_l_eye_lower_open.obj"               // FaceMesh Index: 8             Weight Index: 6
#define L_UPPER_O "High_Res_Blend/Mery_l_eye_upper_open.obj"               // FaceMesh Index: 9             Weight Index: 7
#define L_NOSE_WRINKLE "High_Res_Blend/Mery_l_nose_wrinkle.obj"            // FaceMesh Index: 10            Weight Index: 8
#define L_PUFF "High_Res_Blend/Mery_l_puff.obj"                            // FaceMesh Index: 11            Weight Index: 9
#define L_SAD "High_Res_Blend/Mery_l_sad.obj"                              // FaceMesh Index: 12            Weight Index: 10
#define L_SMILE "High_Res_Blend/Mery_l_smile.obj"                          // FaceMesh Index: 13            Weight Index: 11
#define L_SUCK "High_Res_Blend/Mery_l_suck.obj"                            // FaceMesh Index: 14            Weight Index: 12
// Right Blendshape Models                                                                                                
#define R_BROW_LOWER "High_Res_Blend/Mery_r_brow_lower.obj"                // FaceMesh Index: 15            Weight Index: 13
#define R_BROW_NARROW "High_Res_Blend/Mery_r_brow_narrow.obj"              // FaceMesh Index: 16            Weight Index: 14
#define R_BROW_RAISE "High_Res_Blend/Mery_r_brow_raise.obj"                // FaceMesh Index: 17            Weight Index: 15
#define REFT_CLOSED "High_Res_Blend/Mery_r_eye_closed.obj"                 // FaceMesh Index: 18            Weight Index: 16
#define R_LOWER_O "High_Res_Blend/Mery_r_eye_lower_open.obj"               // FaceMesh Index: 19            Weight Index: 17
#define R_UPPER_O "High_Res_Blend/Mery_r_eye_upper_open.obj"               // FaceMesh Index: 20            Weight Index: 18
#define R_NOSE_WRINKLE "High_Res_Blend/Mery_r_nose_wrinkle.obj"            // FaceMesh Index: 21            Weight Index: 19
#define R_PUFF "High_Res_Blend/Mery_r_puff.obj"                            // FaceMesh Index: 22            Weight Index: 20
#define R_SAD "High_Res_Blend/Mery_r_sad.obj"                              // FaceMesh Index: 23            Weight Index: 21
#define R_SMILE "High_Res_Blend/Mery_r_smile.obj"                          // FaceMesh Index: 24            Weight Index: 22
#define R_SUCK "High_Res_Blend/Mery_r_suck.obj"                            // FaceMesh Index: 25            Weight Index: 23





// Camera Variables
vec3 cameraPos = vec3(0.0f, 15.0f, 50.0f);
vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraDirec = normalise(cameraPos - cameraTarget);
vec3 up = vec3(0.0f, 1.0f, 0.0f);
vec3 camRight = normalise(cross(up, cameraDirec));
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;



// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// mouse state
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

float mouse_x, mouse_y;


#pragma region SimpleTypes
typedef struct ModelData
{
    size_t mPointCount = 0;
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec2> mTextureCoords;
    std::vector<glm::vec3> mTangents;
    std::vector<glm::vec3> mBitangents;

    Eigen::VectorXf floatVertices;


} ModelData;
#pragma endregion SimpleTypes

// A vector to hold all of the facial mesh data
std::vector<ModelData> FaceMeshes;


// Vector of vec3 picker positions
std::vector<glm::vec3> PickerPositions;



// Column Vector to store the weights for each of the 24 deltaBlendshapes
Eigen::VectorXf  weights;

// (3n x k) Matrix of stacked deltablendshape verticies 
Eigen::MatrixXf B_DeltaBlendShapes;
Eigen::MatrixXf B_w;

// (3c x k) matrix consisting of rows of B corresponding to constrained verticies
Eigen::MatrixXf B_Bar;


//Column Vector of stacked neutral mesh verticies (xyz)
Eigen::VectorXf f0;
//Eigen::VectorXf f0 = Eigen::Map<Eigen::VectorXf, Eigen::Unaligned>(dummy.data(), dummy.size());

// Column Vector of final drawn model -> F = f0 + Bw
Eigen::VectorXf F;

// Model Data of final model to create VAO
ModelData FinalMesh;




std::vector<GLuint> constraints_m0;
std::vector<GLuint> constraints_m;
Eigen::VectorXf  m0(3 * constraints_m0.size(), 1);
Eigen::VectorXf  m(3 * constraints_m.size(), 1);


using namespace std;

// Shader Program Declaration
GLuint shaderProgramP;



#pragma region BlendShapes
// Model Data for blendshapes
ModelData demo, neutral, jaw_open, kiss, 
l_brow_lower,
l_brow_narrow,
l_brow_raise,
l_eye_closed,
l_eye_lower_open,
l_eye_upper_open,
l_nose_wrinkle,
l_puff,
l_sad,
l_smile,
l_suck,
r_brow_lower,
r_brow_narrow,
r_brow_raise,
r_eye_closed,
r_eye_lower_open,
r_eye_upper_open,
r_nose_wrinkle,
r_puff,
r_sad,
r_smile,
r_suck;




// VAO Value for each blendshape model
GLuint  final_vao;






#pragma endregion

// Array to store weights read from animation data file
double animation_data[6000];


GLuint loc1, loc2, loc3, loc4, loc5;


// Function declarations


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);

//Load Mesh Function 
ModelData load_mesh(const char* file_name);

// Shader Functions
char* readShaderSource(const char* shaderFile);
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
GLuint CompileShaders(const char* vertex, const char* fragment);

// VBO functions
GLuint generateObjectBufferMesh(ModelData mesh_data, GLuint shader);

// Degreees and Radian Conversion
float degrees(float radians);
double radians(double degree);



// Display Functions
void display();

// Init and Update Function
void init();
void updateScene();

// Multiply Vector by a float
vec3 vecXfloat(float f, vec3 v1);

// Mouse Callback and Key input
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);


// Merge Blendshapes Data
void Blend_Faces();
// Animate Blendshapes
void AnimateFace(GLFWwindow* window);

// Manipulator Functions
void ManipulatorCreator(int x, int y, glm::mat4 VM, glm::mat4 P, std::vector<ModelData> model);
void manipulatorPickerFrom(int x, int y, glm::mat4 VM, glm::mat4 P);          // Returns the index for the next function
void manipulatorPickerTo(int x, int y, glm::mat4 VM, glm::mat4 P, std::vector<ModelData> model, GLuint pickerNo);

// Generate Weights from Manipulators
void my_direct_manip_method();


// Set Width and Height Values for screen
const unsigned int screen_width = 1000;
const unsigned int screen_height = 750;

// Bool for animation
bool animate = false;

glm::mat4 modelView;
glm::mat4 projection;


glm::vec3 test_vertex = glm::vec3(0.0f);
ModelData box;
GLuint box_vao;

// Manipulator Globals
int manip_index;

// Manipulator Colours 
vec3 mani_1_col = vec3(0.0f, 1.0f, 0.0f);
vec3 mani_2_col = vec3(0.0f, 1.0f, 0.0f);
vec3 mani_3_col = vec3(0.0f, 1.0f, 0.0f);
vec3 mani_4_col = vec3(0.0f, 1.0f, 0.0f);


// TESTING INDEX
GLuint test_index;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Animation 3", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


    init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");



    while (!glfwWindowShouldClose(window))
    {


        glEnable(GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Calculate new mesh vertices positions and update VAO to draw
        Blend_Faces();
        final_vao = generateObjectBufferMesh(FinalMesh, shaderProgramP);

        
        processInput(window);


        // Use "c" key to set animate to true and begin animation
        if (animate == true) {
            AnimateFace(window);
            animate = false;
        }


        display();
        updateScene();


        ImGui::Begin("ImGui Window");
        ImGui::Text("Weights");
        ImGui::SliderFloat("Right Smile", &weights[22], 0.0f, 1.0f);
        ImGui::SliderFloat("Left Smile", &weights[11], 0.0f, 1.0f);
        ImGui::SliderFloat("Right EyeClose", &weights[16], 0.0f, 1.0f);
        ImGui::SliderFloat("Left EyeClose", &weights[5], 0.0f, 1.0f);
        ImGui::SliderFloat("Kiss", &weights[1], 0.0f, 1.0f);
        ImGui::SliderFloat("Jaw", &weights[0], 0.0f, 1.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {


    if (PickerPositions.size() > 0) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

            
            
            
            manipulatorPickerFrom(mouse_x, mouse_y, modelView, projection);
            
            // Change Colour of Chosen Manipulator
            if (manip_index == 0) {
                mani_1_col = vec3(1.0f, 0.27f, 0.0f);
            }
            if (manip_index == 1) {
                mani_2_col = vec3(1.0f, 0.27f, 0.0f);
            }
            if (manip_index == 2) {
                mani_3_col = vec3(1.0f, 0.27f, 0.0f);
            }
            if (manip_index == 3) {
                mani_4_col = vec3(1.0f, 0.27f, 0.0f);
            }
            

        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

            manipulatorPickerTo(mouse_x, mouse_y, modelView, projection, FaceMeshes, manip_index);
            my_direct_manip_method();

            mani_1_col = vec3(0.0f, 1.0f, 0.0f);
            mani_2_col = vec3(0.0f, 1.0f, 0.0f);
            mani_3_col = vec3(0.0f, 1.0f, 0.0f);
            mani_4_col = vec3(0.0f, 1.0f, 0.0f);

        }
    }


    if(PickerPositions.size() < 4){

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

            mani_1_col, mani_2_col, mani_3_col, mani_4_col = vec3(0.0f, 1.0f, 0.0f);
            ManipulatorCreator(mouse_x, mouse_y, modelView, projection, FaceMeshes);
        
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {


        }

    }

}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 30.5f * deltaTime;


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += vecXfloat(cameraSpeed, cameraFront);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= vecXfloat(cameraSpeed, cameraFront);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= normalise(cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += normalise(cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        animate = true;
    }

}

// Load mesh data
ModelData load_mesh(const char* file_name) {

    ModelData modelData;

    /* Use assimp to read the model file, forcing it to be read as    */
    /* triangles. The second flag (aiProcess_PreTransformVertices) is */
    /* relevant if there are multiple meshes in the model file that   */
    /* are offset from the origin. This is pre-transform them so      */
    /* they're in the right position.                                 */
    const aiScene* scene = aiImportFile(
        file_name,
        aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace
    );

    if (!scene) {
        fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
        return modelData;
    }

    printf("  %i materials\n", scene->mNumMaterials);
    printf("  %i meshes\n", scene->mNumMeshes);
    printf("  %i textures\n", scene->mNumTextures);

    for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
        const aiMesh* mesh = scene->mMeshes[m_i];
        printf("    %i vertices in mesh\n", mesh->mNumVertices);
        modelData.mPointCount += mesh->mNumVertices;
        for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
            if (mesh->HasPositions()) {
                const aiVector3D* vp = &(mesh->mVertices[v_i]);
                modelData.mVertices.push_back(glm::vec3(vp->x, vp->y, vp->z));
                modelData.floatVertices.conservativeResize(modelData.mVertices.size() * 3, 1);
                modelData.floatVertices(3 * modelData.mVertices.size() - 3) = vp->x;
                modelData.floatVertices(3 * modelData.mVertices.size() - 2) = vp->y;
                modelData.floatVertices(3 * modelData.mVertices.size() - 1) = vp->z;
            }
            if (mesh->HasNormals()) {
                const aiVector3D* vn = &(mesh->mNormals[v_i]);
                modelData.mNormals.push_back(glm::vec3(vn->x, vn->y, vn->z));
            }
            if (mesh->HasTextureCoords(0)) {
                const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
                modelData.mTextureCoords.push_back(glm::vec2(vt->x, vt->y));

            }
            if (mesh->HasTangentsAndBitangents()) {
                /* You can extract tangents and bitangents here              */
                /* Note that you might need to make Assimp generate this     */
                /* data for you. Take a look at the flags that aiImportFile  */
                /* can take.                                                 */
                const aiVector3D* vtan = &(mesh->mTangents[v_i]);
                modelData.mTangents.push_back(glm::vec3(vtan->x, vtan->y, vtan->z));

                const aiVector3D* vbt = &(mesh->mBitangents[v_i]);
                modelData.mBitangents.push_back(glm::vec3(vbt->x, vbt->y, vbt->z));
            }
        }
    }

    FaceMeshes.push_back(modelData);

    aiReleaseImport(scene);
    return modelData;
}




char* readShaderSource(const char* shaderFile) {
    FILE* fp;
    fopen_s(&fp, shaderFile, "rb");

    if (fp == NULL) { return NULL; }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);

    return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    // create a shader object
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        std::cerr << "Error creating shader..." << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }
    const char* pShaderSource = readShaderSource(pShaderText);


    // Bind the source code to the shader, this happens before compilation
    glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
    // compile the shader and check for errors


    glCompileShader(ShaderObj);
    GLint success;
    // check for shader related errors using glGetShaderiv

    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024] = { '\0' };
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        std::cerr << "Error compiling "
            << (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader program: " << InfoLog << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }
    // Attach the compiled shader object to the program object
    glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders(const char* vertex, const char* fragment)
{
    //Start the process of setting up our shaders by creating a program ID
    //Note: we will link all the shaders together into this ID
    GLuint shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        std::cerr << "Error creating shader program..." << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }


    // Create two shader objects, one for the vertex, and one for the fragment shader
    AddShader(shaderProgram, vertex, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fragment, GL_FRAGMENT_SHADER);


    GLint Success = 0;
    GLchar ErrorLog[1024] = { '\0' };
    // After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(shaderProgram);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }

    // program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(shaderProgram);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
        std::cerr << "Press enter/return to exit..." << std::endl;
        std::cin.get();
        exit(1);
    }
    // Finally, use the linked shader program
    // Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
    glUseProgram(shaderProgram);
    return shaderProgram;
}


// Generate VAO for final face mesh
GLuint generateObjectBufferMesh(ModelData mesh_data, GLuint shader) {
    /*----------------------------------------------------------------------------
    LOAD MESH HERE AND COPY INTO BUFFERS
    ----------------------------------------------------------------------------*/

    //Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
    //Might be an idea to do a check for that before generating and binding the buffer.



    unsigned int vp_vbo = 0;
    loc1 = glGetAttribLocation(shader, "vertex_position");
    loc2 = glGetAttribLocation(shader, "vertex_normal");


    glGenBuffers(1, &vp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh_data.mPointCount * sizeof(float), mesh_data.floatVertices.data(), GL_STATIC_DRAW);
    unsigned int vn_vbo = 0;
    glGenBuffers(1, &vn_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh_data.mPointCount * sizeof(float), &mesh_data.mNormals[0], GL_STATIC_DRAW);


    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(loc1);
    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc2);
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    


    return vao;

}

//converts radians to degrees
float degrees(float radians) {
    float pi = 3.14159265359;
    return (radians * (180 / pi));
}

// converts degrees to radians
double radians(double degree) {
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}


// Display Final Mesh 
void display() {



    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramP);



    //Declare your uniform variables that will be used in your shader
    // Vertex Shader Uniforms
    int matrix_location = glGetUniformLocation(shaderProgramP, "model");
    int view_mat_location = glGetUniformLocation(shaderProgramP, "view");
    int proj_mat_location = glGetUniformLocation(shaderProgramP, "proj");
    int colour_location = glGetUniformLocation(shaderProgramP, "objectColor");


    // Root of the Hierarchy
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 persp_proj = glm::perspective(fov, (float)screen_width / (float)screen_height, 0.1f, 1000.0f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 campos = glm::vec3(cameraPos.v[0], cameraPos.v[1], cameraPos.v[2]);
    glm::vec3 camfront = glm::vec3(cameraFront.v[0], cameraFront.v[1], cameraFront.v[2]);
    glm::vec3 camup = glm::vec3(cameraUp.v[0], cameraUp.v[1], cameraUp.v[2]);

    view = glm::lookAt(campos, campos + camfront, camup);

    modelView = model * view;
    projection = persp_proj;
    

     // update uniforms & draw
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(persp_proj));
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colour_location, 1, vec3(1.0f, 0.8f, 0.6f).v);

    glBindVertexArray(final_vao);
    glDrawArrays(GL_TRIANGLES, 0, neutral.mPointCount);

    if (animate == false) {

        if (PickerPositions.size() > 0) {

            glm::mat4 picker_model = glm::mat4(1.0f);
            picker_model = translate(picker_model, PickerPositions[0]);
            picker_model = scale(picker_model, glm::vec3(0.5f, 0.5f, 0.5f));

            glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(picker_model));
            glUniform3fv(colour_location, 1, mani_1_col.v);
            glBindVertexArray(box_vao);
            glDrawArrays(GL_TRIANGLES, 0, box.mPointCount);


        }


        if (PickerPositions.size() > 1) {

            glm::mat4 picker_model = glm::mat4(1.0f);
            picker_model = translate(picker_model, PickerPositions[1]);
            picker_model = scale(picker_model, glm::vec3(0.5f, 0.5f, 0.5f));

            glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(picker_model));
            glUniform3fv(colour_location, 1, mani_2_col.v);
            glBindVertexArray(box_vao);
            glDrawArrays(GL_TRIANGLES, 0, box.mPointCount);

        }

        if (PickerPositions.size() > 2) {

            glm::mat4 picker_model = glm::mat4(1.0f);
            picker_model = translate(picker_model, PickerPositions[2]);
            picker_model = scale(picker_model, glm::vec3(0.5f, 0.5f, 0.5f));

            glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(picker_model));
            glUniform3fv(colour_location, 1, mani_3_col.v);
            glBindVertexArray(box_vao);
            glDrawArrays(GL_TRIANGLES, 0, box.mPointCount);

        }


        if (PickerPositions.size() > 3) {

            glm::mat4 picker_model = glm::mat4(1.0f);
            picker_model = translate(picker_model, PickerPositions[3]);
            picker_model = scale(picker_model, glm::vec3(0.5f, 0.5f, 0.5f));

            glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(picker_model));
            glUniform3fv(colour_location, 1, mani_4_col.v);
            glBindVertexArray(box_vao);
            glDrawArrays(GL_TRIANGLES, 0, box.mPointCount);


        }
    }


}

// Used to update any variables that need changing per frame
void updateScene() {

    static double last_time = 0;
    double curr_time = glfwGetTime();
    if (last_time == 0)
        last_time = curr_time;
    float delta = (curr_time - last_time);
    last_time = curr_time;

    float currentFrame = glfwGetTime();


    deltaTime = curr_time - lastFrame;
    lastFrame = currentFrame;


}



// Initialize everything. Used to load animation file, shader files, and facial models
void init()
{
    // read weights from animation file
    ifstream AniFile("animation.txt");

    for (int i = 0; i < 6000; i++) {

        AniFile >> animation_data[i];

    }
    
    
    // Set up the shaders
    shaderProgramP = CompileShaders("Shader_Files/Phong_Vertex.txt", "Shader_Files/Phong_Fragment.txt");

    // ******************************************************************************************
    // ******************************************************************************************
    //       NOTE!
    // This model will be added to the Model vector, so any iterations through this vector must omit the first one

    box = load_mesh(DEMO);
    box_vao = generateObjectBufferMesh(box, shaderProgramP);
    // ******************************************************************************************

    // Load All meshes 
    neutral = load_mesh(NEUTRAL);
    jaw_open = load_mesh(JAW_OPEN);
    kiss = load_mesh(KISS);
    l_brow_lower = load_mesh(L_BROW_LOWER);
    l_brow_narrow = load_mesh(L_BROW_NARROW);
    l_brow_raise = load_mesh(L_BROW_RAISE);
    l_eye_closed = load_mesh(LEFT_CLOSED);
    l_eye_lower_open = load_mesh(L_LOWER_O);
    l_eye_upper_open = load_mesh(L_UPPER_O);
    l_nose_wrinkle = load_mesh(L_NOSE_WRINKLE);
    l_puff = load_mesh(L_PUFF);
    l_sad = load_mesh(L_SAD);
    l_smile = load_mesh(L_SMILE);
    l_suck = load_mesh(L_SUCK);
    r_brow_lower = load_mesh(R_BROW_LOWER);
    r_brow_narrow = load_mesh(R_BROW_NARROW);
    r_brow_raise = load_mesh(R_BROW_RAISE);
    r_eye_closed = load_mesh(REFT_CLOSED);
    r_eye_lower_open = load_mesh(R_LOWER_O);
    r_eye_upper_open = load_mesh(R_UPPER_O);
    r_nose_wrinkle = load_mesh(R_NOSE_WRINKLE);
    r_puff = load_mesh(R_PUFF);
    r_sad = load_mesh(R_SAD);
    r_smile = load_mesh(R_SMILE);
    r_suck = load_mesh(R_SUCK);


    // Set up column vector f0 ---> The neutral face model is the second model in the FaceMeshes vector
    f0.conservativeResize(FaceMeshes[1].floatVertices.size(), 1);
    f0 = FaceMeshes[1].floatVertices;

    //Set up B matrix known as B_DeltaBlendShapes
    B_DeltaBlendShapes.conservativeResize(FaceMeshes[1].floatVertices.size(), FaceMeshes.size() - 2); // Resize the matrix 
    B_w.conservativeResize(FaceMeshes[1].floatVertices.size(), FaceMeshes.size() - 2); // Resize the matrix 

   // Populate the matrix with the column vectors of verticies for each blendshape
    for (int i = 2; i < FaceMeshes.size(); i++) {

        for (int j = 0; j < FaceMeshes[1].floatVertices.size(); j++) {

            
            B_DeltaBlendShapes( j, i - 2 ) = FaceMeshes[i].floatVertices(j, 1);
           
        }

    }

    weights.conservativeResize(FaceMeshes.size() - 2, 1);
    // Set up weight column vector by initilizing all to 0
    for (int i = 0; i < FaceMeshes.size() - 2; i++) {
        weights(i, 1) = 0.0;
    }
    

    // Set neutral mesh as a placeholder mesh for Final Mesh 
    FinalMesh = FaceMeshes[1];

    Blend_Faces();

    final_vao = generateObjectBufferMesh(FinalMesh, shaderProgramP);

}



// Multiply vec3 by float
vec3 vecXfloat(float f, vec3 v1) {

    vec3 result = vec3((v1.v[0] * f), (v1.v[1] * f), (v1.v[2] * f));
    return result;
}

// Mouse Position Callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {


    int x = static_cast<int>(xposIn);
    int y = static_cast<int>(yposIn);

    mouse_x = x;
    mouse_y = y;
 

}




// function to create delta blendshapes and final mesh
void Blend_Faces() {

    

    // Calculate the Delta Blend Faces multiplied by the weights
    // Bw



    for (int i = 2; i < FaceMeshes.size(); i++) {

        for (int j = 0; j < FaceMeshes[1].floatVertices.size(); j++) {

            int q = i - 2;              
            B_DeltaBlendShapes(j, q) = (FaceMeshes[i].floatVertices(j, 1) - f0(j, 1));    
            B_w(j, q) = B_DeltaBlendShapes(j, q) * weights(i - 2, 1);

        }

    }
    


    // F = f0 + Bw
    float row_total = 0;
    for (int i = 0; i < FaceMeshes[1].floatVertices.size(); i++) {

       // row_total = 0;

        for (int j = 0; j < FaceMeshes.size() - 2; j++) {

            
            row_total += B_w(i, j);
         

         
            
        }

        FinalMesh.floatVertices(i, 1) = f0(i, 1) + row_total;
        row_total = 0;

        
    }



}
// Called to use weights read from imported file to create facial animation
void AnimateFace(GLFWwindow* window) {

    int lineNumber = 0;

    //Assign weights from file to weights
    while (lineNumber < 6000) {

       
        weights[0] = animation_data[lineNumber];
        weights[1] = animation_data[lineNumber + 1];
        weights[2] = animation_data[lineNumber + 2];
        weights[3] = animation_data[lineNumber + 3];
        weights[4] = animation_data[lineNumber + 4];
        weights[5] = animation_data[lineNumber + 5];
        weights[6] = animation_data[lineNumber + 6];
        weights[7] = animation_data[lineNumber + 7];
        weights[8] = animation_data[lineNumber + 8];
        weights[9] = animation_data[lineNumber + 9];
        weights[10] = animation_data[lineNumber + 10];
        weights[11] = animation_data[lineNumber + 11];
        weights[12] = animation_data[lineNumber + 12];
        weights[13] = animation_data[lineNumber + 13];
        weights[14] = animation_data[lineNumber + 14];
        weights[15] = animation_data[lineNumber + 15];
        weights[16] = animation_data[lineNumber + 16];
        weights[17] = animation_data[lineNumber + 17];
        weights[18] = animation_data[lineNumber + 18];
        weights[19] = animation_data[lineNumber + 19];
        weights[20] = animation_data[lineNumber + 20];
        weights[21] = animation_data[lineNumber + 21];
        weights[22] = animation_data[lineNumber + 22];
        weights[23] = animation_data[lineNumber + 23];



        Blend_Faces();
        final_vao = generateObjectBufferMesh(FinalMesh, shaderProgramP);
        display();
        glfwSwapBuffers(window);
        lineNumber += 24;
        Sleep(50);

    }


}



// Create a manipulatior
void ManipulatorCreator(int x, int y, glm::mat4 VM, glm::mat4 P, std::vector<ModelData> model) {


    // get window coords
    glm::vec3 window;
    window.x = x;
    window.y = screen_height - y - 1;
    glReadPixels(x, screen_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window.z);

    // Get Object Coords of Main mesh
    glm::vec3 object = glm::unProject(window, VM, P, glm::vec4(0.0f, 0.0f, screen_width, screen_height));

    PickerPositions.push_back(object);

     // find the nearest Vertex
    GLfloat dist = 10;
    GLfloat temp = 0.0f;
    GLuint m_index = 0; // index of mesh with closest vertex
    GLuint v_index = 0; // index of closest vertex

    // brute force search through all vertices
    for (int i = 2; i < model.size(); i++) {

        for (int j = 0; j < FinalMesh.mVertices.size(); j++) {

            temp = glm::distance(object, FinalMesh.mVertices[j]);

            if (temp <= dist) {

                dist = temp;
                m_index = i;
                v_index = j;
            }


        }

    }

    glm::vec3 vertex = model[m_index].mVertices[v_index];
    constraints_m0.push_back(v_index);
    constraints_m.push_back(v_index);


}



void manipulatorPickerFrom(int x, int y, glm::mat4 VM, glm::mat4 P) {


    // get window coords
    glm::vec3 window;
    window.x = x;
    window.y = screen_height - y - 1;
    glReadPixels(x, screen_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window.z);

    // Get Object Coords
    glm::vec3 object = glm::unProject(window, VM, P, glm::vec4(0.0f, 0.0f, screen_width, screen_height));


     // find the nearest Vertex
    GLfloat dist = 10;
    GLfloat temp = 0.0f;
    GLuint v_index = 0; // index of closest vertex

    // brute force search through all known picker positions // max 4
    for (int i = 0; i < PickerPositions.size(); i++) {

        temp = glm::distance(object, PickerPositions[i]);

        if (temp <= dist) {

            dist = temp;
            v_index = i;
 
        }

    }

    glm::vec3 vertex = PickerPositions[v_index];

   
    manip_index = v_index;

    int m0_resize = (constraints_m0.size() * 3) ;
    int m0_x = 3 * manip_index;
    int m0_y = 3 * manip_index + 1;
    int m0_z = 3 * manip_index + 2;

    m0.conservativeResize(m0_resize, 1);

    m0(m0_x, 1) = vertex.x;
    m0(m0_y, 1) = vertex.y;
    m0(m0_z, 1) = vertex.z;

    


}

void manipulatorPickerTo(int x, int y, glm::mat4 VM, glm::mat4 P, std::vector<ModelData> model, GLuint pickerNo) {


    // get window coords
    glm::vec3 window;
    window.x = x;
    window.y = screen_height - y - 1;
    glReadPixels(x, screen_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window.z);

    // Get Object Coords
    glm::vec3 object = glm::unProject(window, VM, P, glm::vec4(0.0f, 0.0f, screen_width, screen_height));

    // test_vertex = object;
    PickerPositions[manip_index] = object;

     // find the nearest Vertex
    GLfloat dist = 10;
    GLfloat temp = 0.0f;
    GLuint m_index = 0; // index of mesh with closest vertex
    GLuint v_index = 0; // index of closest vertex

    int m_resize = (constraints_m.size() * 3);
    int m_x = 3 * manip_index;
    int m_y = 3 * manip_index + 1;
    int m_z = 3 * manip_index + 2;

    m.conservativeResize(m_resize, 1);
    B_Bar.conservativeResize(m.rows(), B_DeltaBlendShapes.cols());

    // brute force search through all vertices
    for (int i = 2; i < model.size(); i++) {

        for (int j = 0; j < model[1].mVertices.size(); j++) {

            temp = glm::distance(object, model[1].mVertices[j]);

            if (temp <= dist) {

                dist = temp;
                m_index = i;
                v_index = j;
            }

        }


    }

    glm::vec3 vertex = model[m_index].mVertices[v_index];

    test_index = m_index;

   
    m.conservativeResize( m_resize, 1);

    m(m_x, 1) = vertex.x;
    m(m_y, 1) = vertex.y;
    m(m_z, 1) = vertex.z;


    B_Bar.conservativeResize(m.rows(), B_DeltaBlendShapes.cols());

    for (int i = 0; i < B_DeltaBlendShapes.cols(); i++) {
        B_Bar(m_x, i) = B_DeltaBlendShapes(v_index * 3, i);
        B_Bar(m_y, i) = B_DeltaBlendShapes(v_index * 3 + 1, i);
        B_Bar(m_z, i) = B_DeltaBlendShapes(v_index * 3 + 2, i);
    }
    
}


void my_direct_manip_method() {

  

    float alpha = 0.1f;
    float u = 0.001f;

    Eigen::VectorXf prev_weights = weights * (alpha);


    // Set up Identity Matrix I
    Eigen::MatrixXf Identity(24, 24);
    for (int i = 0; i < 24; i++) {

        for (int j = 0; j < 24; j++) {

            if (i == j) {
                Identity(i, j) = 1.0f;
            }
            else {
                Identity(i, j) = 0.0f;
            }
            
        }
        
    }

  

    // Left Side
    Eigen::MatrixXf B_Transpose = B_Bar.transpose(); 
    Eigen::MatrixXf BtB = B_Transpose * B_Bar;
    Eigen::MatrixXf Left_Side = ( BtB + (Identity * (alpha + u)) );
    
    // Right Side
    Eigen::VectorXf M_M0 = m - m0;
    Eigen::MatrixXf Right_Side = (B_Transpose * M_M0);
    Right_Side = Right_Side + prev_weights;
    

    Eigen::LDLT<Eigen::MatrixXf> solver(Left_Side);
    Eigen::VectorXf new_weights = solver.solve(Right_Side);


    for (int i = 0; i < 24; i++) {

        if (new_weights(i, 1) > 1) {
            new_weights(i, 1) = 1;
        }
        if (new_weights(i, 1) < 0) {
            new_weights(i, 1) = 0;
           
        }
        
    }

    weights = new_weights;

}