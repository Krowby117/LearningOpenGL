#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <custom/ShaderClass.h>

#include <iostream>
#include <map>

using namespace glm;
using namespace std;

// function prototypes ////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* setupWindow(int width, int height);

// circle stuff
vector<float> getCircleVerts(float radius, int segments);
void drawCircle();

// Window Settings ////////
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// time stuff ////////
float deltaTime = 0.0f, lastFrame = 0.0f;

int main()
{
    // initilaize the window
    GLFWwindow* window = setupWindow(SCR_WIDTH, SCR_HEIGHT);
    
    // create the shader
    Shader myShader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
    
    // create the circle
    vector<float> circleVerts = getCircleVerts(50.0f, 64);
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVerts.size() * sizeof(float) , circleVerts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        // update the time stuff
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // do input
        processInput(window);
        
        // clear the screen (erase previous frame)
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // circle stuff
        // create the view, model, and projection matrices
        mat4 model = translate(mat4(1.0f), position);
        mat4 view = mat4(1.0f);
        mat4 projection = ortho(-(float)SCR_WIDTH, (float)SCR_WIDTH, -(float)SCR_HEIGHT, (float)SCR_HEIGHT);
        
        // shader information
        myShader.use();
        myShader.setMat4("MVP", projection * view * model);
        myShader.setVec3("fragCol", vec3(1.0f, 1.0f, 1.0f));
        
        // draw the circle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleVerts.size() / 2);
        
        //-// do events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow* window)
{
    // if press escape key close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* setupWindow(int width, int height)
{
    // Initialize GLFW and set the version to 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window object
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }
    
    return window;
}

vector<float> getCircleVerts(float radius, int segments)
{
    vector<float> verts;
    verts.reserve((segments + 2) * 2);
    
    // draw a circle
    verts.push_back(0.0f);
    verts.push_back(0.0f);

    for (unsigned int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.141592653589 * ((float)i / (float)segments);
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        
        verts.push_back(x);
        verts.push_back(y);
    }
    
    return verts;
}
