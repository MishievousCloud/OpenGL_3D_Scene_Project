#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


using namespace std;

int width, height;

// Draw Primitive(s)
void draw() {
    // glDrawArrays variables
    GLenum mode = GL_TRIANGLES;
    GLint first = 0;
    GLsizei count = 3;

    glDrawArrays(mode, first, count);
}

// Create and compile shaders
static GLuint CompileShader(const string& source, GLuint shaderType) {
    // Create shader object
    GLuint shaderID = glCreateShader(shaderType);
    const char* src = source.c_str(); // reference source code

    // Attach source code to shader object
    glShaderSource(shaderID, 1, &src, nullptr);

    // Compile shader
    glCompileShader(shaderID);

    // Return compiled shader ID
    return shaderID;
}

// Create program object
static GLuint CreateShaderProgram(const string& vertexShader, const string& fragmentShader) {
    // Compile vertex shader
    GLuint vertexShaderCompiled = CompileShader(vertexShader, GL_VERTEX_SHADER);

    // Compile fragment shader
    GLuint fragmentShaderCompiled = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    // Create program object
    GLuint shaderProgram = glCreateProgram();

    // Attach vertex and fragment shaders to program object
    glAttachShader(shaderProgram, vertexShaderCompiled);
    glAttachShader(shaderProgram, fragmentShaderCompiled);

    // Link shaders to create .exe
    glLinkProgram(shaderProgram);

    // Delete compiled shaders
    glDeleteShader(vertexShaderCompiled);
    glDeleteShader(fragmentShaderCompiled);

    // Return shader program
    return shaderProgram;
}

int main(void)
{

    width = 640; height = 480;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Justin Aebi", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Define position attributes for the triangle
    GLfloat vertices[] = {
        -0.5,-0.5,0.0, // v0
        -0.5,0.5,0.0,  // v1
        0.5,-0.5,0.0   // v2
    };

    GLuint VBO;

    glGenBuffers(1, &VBO); // Create VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Selects and activates VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Load vertices into the buffer 

    // Specify attribute location and layout to gpu
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Vertex shader source code
    string vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec4 vPosition;"
        "void main()\n"
        "{\n"
        "gl_Position = vPosition;"
        "}\n";

    // Fragment shader source code
    string fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 fragColor;"
        "void main()\n"
        "{\n"
        "fragColor = vec4(1.0, 0.0, 0.0, 1.0);"
        "}\n";

    // Creating shader program
    GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Use shader program exe
    //glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        // Resize window and graphics simultaneously
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Draw primitive(s)
        draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}