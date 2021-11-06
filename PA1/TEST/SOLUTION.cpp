#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int width, height;

// Draw Primitive(s)
void draw()
{
	GLenum mode = GL_TRIANGLES;
	GLsizei indices = 6;
	glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);


}

// Create and Compile Shaders
static GLuint CompileShader(const string& source, GLuint shaderType)
{
	// Create Shader object
	GLuint shaderID = glCreateShader(shaderType);
	const char* src = source.c_str();

	// Attach source code to Shader object
	glShaderSource(shaderID, 1, &src, nullptr);

	// Compile Shader
	glCompileShader(shaderID);

	// Return ID of Compiled shader
	return shaderID;

}

// Create Program Object
static GLuint CreateShaderProgram(const string& vertexShader, const string& fragmentShader)
{
	// Compile vertex shader
	GLuint vertexShaderComp = CompileShader(vertexShader, GL_VERTEX_SHADER);

	// Compile fragment shader
	GLuint fragmentShaderComp = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	// Create program object
	GLuint shaderProgram = glCreateProgram();

	// Attach vertex and fragment shaders to program object
	glAttachShader(shaderProgram, vertexShaderComp);
	glAttachShader(shaderProgram, fragmentShaderComp);

	// Link shaders to create executable
	glLinkProgram(shaderProgram);

	// Delete compiled vertex and fragment shaders
	glDeleteShader(vertexShaderComp);
	glDeleteShader(fragmentShaderComp);

	// Return Shader Program
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
	window = glfwCreateWindow(width, height, "Main Window", NULL, NULL);
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

	GLfloat vertices[] = {

		// Triangle 1
		-0.5, -0.5, 0.0, // index 0
		1.0, 0.0, 0.0, // red

		-0.5, 0.5, 0.0, // index 1
		0.0, 1.0, 0.0, // green

		0.5, -0.5, 0.0,  // index 2	
		0.0, 0.0, 1.0, // blue

		// Triangle 2	
		0.5, 0.5, 0.0,  // index 3	
		1.0, 0.0, 1.0 // purple
	};

	// Define element indices
	GLubyte indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	GLuint VBO, EBO, VAO;

	glGenBuffers(1, &VBO); // Create VBO
	glGenBuffers(1, &EBO); // Create EBO

	glGenVertexArrays(1, &VAO); // Create VOA
	glBindVertexArray(VAO);

		// VBO and EBO Placed in User-Defined VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Select VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Select EBO


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Load vertex attributes
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Load indices 

																						 // Specify attribute location and layout to GPU
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VOA or close off (Must call VOA explicitly in loop)

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

	// Creating Shader Program
	GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

	// Use Shader Program exe once
	//glUseProgram(shaderProgram);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		// Resize window and graphics simultaneously
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// Use Shader Program exe and select VAO before drawing 
		glUseProgram(shaderProgram); // Call Shader per-frame when updating attributes
		glBindVertexArray(VAO); // User-defined VAO must be called before draw. 

			// Draw primitive(s)
			draw();

		// Unbind Shader exe and VOA after drawing per frame
		glBindVertexArray(0); //Incase different VAO wii be used after
		glUseProgram(0); // Incase different shader will be used after

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