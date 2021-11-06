#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int width, height;

// Draw Primitive(s)
void draw() {
	// glDrawArrays variables
	GLenum mode = GL_TRIANGLES;
	GLsizei indices = 6;
	glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);
}

// Create and compile shaders
static GLuint CompileShader(const string& source, GLuint shaderType) {

	// Create shader object
	GLuint shaderID = glCreateShader(shaderType);
	const char* src = source.c_str();

	// Attach source code to Shader object
	glShaderSource(shaderID, 1, &src, nullptr);

	// Compile Shader
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

	// Delete compiled vertex and fragment shaders
	glDeleteShader(vertexShaderCompiled);
	glDeleteShader(fragmentShaderCompiled);

	// Return shader program
	return shaderProgram;
}

int main(void) {

	width = 640; 
	height = 480;

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

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
		cout << "Error!" << endl;

	// Define position attributes for the triangle
	GLfloat vertices[] = {
		// Triangle one vertices
		0.0,0.0,0.0,   // i0
		1.0, 0.0, 0.0, // red color

		-0.5,0.0,0.0,  // i1
		0.0,1.0,0.0,   // green color

		0.0,-0.5,0.0,  // i2
		0.0,0.0,1.0,   // blue color

		// Triangle two vertices
		0.5,0.0,0.0,   // i3
		0.0,1.0,1.0,   // cyan color

		0.0,0.5,0.0,   // i4
		1.0,0.0,1.0,   // purple color
	};

	// Define element indices
	GLubyte indices[] = {
		0, 1, 2,
		0, 3, 4
	};

	GLuint VBO, EBO, VAO;

	glGenBuffers(1, &VBO); // Create VBO
	glGenBuffers(1, &EBO); // Create EBO

	glGenVertexArrays(1, &VAO); // Create VAO
	glBindVertexArray(VAO);

		// VBO and EBO Placed in user-defined VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Select VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Select EBO

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Load vertex attributes
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Load indices

		// Specify attribute location and layout to GPU
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	// Vertex shader source code
	string vertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec4 vPosition;"
		"layout(location = 1) in vec4 aColor;"
		"out vec4 oColor;"
		"void main()\n"
		"{\n"
		"gl_Position = vPosition;"
		"oColor = aColor;"
		"}\n";

	// Fragment shader source code
	string fragmentShaderSource =
		"#version 330 core\n"
		"in vec4 oColor;"
		"out vec4 fragColor;"
		"void main()\n"
		"{\n"
		"fragColor = oColor;"
		"}\n";

	// Creating shader program
	GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		// Resize window and graphics simultaneously
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// Use Shader Program exe and select VAO before drawing 
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Draw primitive(s)
		draw();

		// Unbind Shader exe and VAO after drawing per frame
		glBindVertexArray(0);
		glUseProgram(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//Clear GPU resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}