
#include <glew.h>
#include "main.h"

#include <string.h>

#include <stdio.h>
#include <glfw3.h>


//Window dimensions
const GLint Width = 800, Height = 600;

GLuint VAO, VBO, shader;

// Vertax Shader
static const char* vShader = "								\n\
#version 330												\n\
															\n\
layout(location = 0) in vec3 pos;							\n\
															\n\
void main()													\n\
{															\n\
	gl_Position = vec4(0.4*pos.x, 0.4*pos.y, pos.z, 1.0);	\n\
}";

//Fragment Shader 
static const char* fshader = "								\n\
#version 330												\n\
															\n\
out vec4 color;											 \n\
															\n\
void main()													\n\
{															\n\
	color = vec4(1.0, 0.0, 0.0, 1.0);						\n\
}";



void createtriangle() 
{
	GLfloat vertices[] = 
	{
	   -1.0f,-1.0f, 0.0f,
	   1.0f,-1.0f,0.0f,
	   0.0f,1.0f,0.0f,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Addshader(GLuint theprogram, const char* shadercode, GLenum shadertype)
{
	GLuint theShader = glCreateShader(shadertype);

	const GLchar* theCode[1];
	theCode[0] = shadercode;

	GLint codelength[1];
	codelength[0] = strlen(shadercode);

	glShaderSource(theShader, 1, theCode, codelength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLOG[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLOG), NULL, eLOG);
		printf("Error compiling the %d shader to program '%s'\n", shadertype, eLOG);
		return;
	}

	glAttachShader(theprogram, theShader);
}


void CompileShader()
{
	shader = glCreateProgram();
	
	if (!shader) {
		printf("error creating shader program\n");
		return;
	}
	Addshader(shader, vShader, GL_VERTEX_SHADER);
	Addshader(shader, fshader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLOG[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLOG), NULL, eLOG);
		printf("Error linking to program '%s'\n", eLOG);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLOG), NULL, eLOG);
		printf("Error validating program '%s'\n", eLOG);
		return;

	}

}

int main()
{
	//initialize GLFW
	if (!glfwInit()) 
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}
	
	//setup GLFW window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(Width, Height, "Test Window", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate;
		return 1;
	}

	//Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extention features
	glewExperimental = GL_TRUE;

	if (glewInit()!= GLEW_OK)
	{
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	createtriangle();
	CompileShader();

	//loop till window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		//get + handle user input events
		glfwPollEvents();

		//Clear window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}

	return 0;
}

main::main()
{
}


main::~main()
{
}
