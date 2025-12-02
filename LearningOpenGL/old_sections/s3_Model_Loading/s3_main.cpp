//
//  LearningOpenGL
//  Section 3 - Model Loading
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <custom/ShaderClass.h>
#include <custom/CameraClass.h>
#include <custom/ModelClass.h>

#include <iostream>

using namespace glm;
using namespace std;

// function prototypes ////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);

// Window Settings ////////
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// camera settings ////////
Camera cam;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

// time stuff ////////
float deltaTime = 0.0f, lastFrame = 0.0f;

// lighting settings ////////
vec3 lightDirection(4.0f, 0.0f, -4.0f);

int main()
{
    
    // Initialize GLFW and set the version to 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // use the shader class to set up shaders ////////
    Shader ourShader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
    
    stbi_set_flip_vertically_on_load(true);
    Model ourModel("assets/models/backpack/backpack.obj");

    // THE RENDER LOOP WOOOOOO ////////
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // set the shader
        ourShader.use();
        ourShader.setVec3("viewPos", cam.camPos);
        ourShader.setFloat("shininess", 64.0f);
        
        ourShader.setVec3("light.direction", lightDirection);
        ourShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        ourShader.setVec3("light.diffuse", 0.75f, 0.75f, 0.75f);
        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        
        // view/projection transformations
        mat4 projection = perspective(radians(cam.getFOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        mat4 view = cam.getViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        mat4 model = mat4(1.0f);
        model = translate(model, vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = scale(model, vec3(1.0f, 1.0f, 1.0f));    // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourShader.setMat3("normalMat", mat3(transpose(inverse(model))));
        ourModel.Draw(ourShader);
        
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    cam.doMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.doScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
    // if press escape key close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
       cam.doCameraMovement(CameraMovement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
       cam.doCameraMovement(CameraMovement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
       cam.doCameraMovement(CameraMovement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
       cam.doCameraMovement(CameraMovement::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
       cam.doCameraMovement(CameraMovement::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
       cam.doCameraMovement(CameraMovement::DOWN, deltaTime);
    }
}

// utility function that loads in a 2D texture from an image
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
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
        cout << "Texture failed to load at path: " << path << endl;
        stbi_image_free(data);
    }

    return textureID;
}
