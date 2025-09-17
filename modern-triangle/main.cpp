#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
constexpr GLint WIDTH = 800;
constexpr GLint HEIGHT = 600;

GLuint VAO;
GLuint VBO;

GLuint shaderProgram;

std::string readShaderFile(const std::string &shaderPath)
{
    if (std::ifstream inputFile(shaderPath); inputFile.is_open()) {
        std::string vertexShader;
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        vertexShader = buffer.str();
        inputFile.close();
        return vertexShader;
    } else {
        std::cerr << "Error: Could not open file " << shaderPath << std::endl;
        return "";
    }
}

void createTriangle()
{
    constexpr GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

std::string vShader = readShaderFile("../vertexShader.glsl");
std::string fShader = readShaderFile("../fragmentShader.glsl");

void addShader(const GLuint program, const std::string &shaderCode, const GLenum shaderType)
{
    const GLuint shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode.c_str();

    GLint codeLength[1];
    codeLength[0] = static_cast<int>(shaderCode.length());

    glShaderSource(shader, 1, code, codeLength);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
        std::cout << "Error compiling the " << shaderType << " shader: " << eLog << std::endl;
        return;
    }

    glAttachShader(program, shader);
}

void compileShaders()
{
    shaderProgram = glCreateProgram();

    if (!shaderProgram)
    {
        std::cout << "Error creating shader program!" << std::endl;
        return;
    }

    addShader(shaderProgram, vShader, GL_VERTEX_SHADER);
    addShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), nullptr, eLog);
        std::cout << "Error linking program: " << eLog << std::endl;
        return;
    }

    glBindVertexArray(VAO);

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), nullptr, eLog);
        std::cout << "Error validating program: " << eLog << std::endl;
    }

    glBindVertexArray(0);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile - No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(
        WIDTH, HEIGHT, "Hello Triangle", nullptr, nullptr
    );

    if (!mainWindow)
    {
        std::cout << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information
    int bufferWidth;
    int bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW!" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    createTriangle();
    compileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}