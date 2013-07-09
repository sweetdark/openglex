#include <stdio.h>
#include <string.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <math.h>
#include "math_3d.h"
#include <assert.h>

GLuint VBO;
GLint gWorldLocation;

static const char *pVS = "#version 330	\n"
"layout (location = 0) in vec3 Position; \n"
"uniform mat4 gWorld;	\n"
"void main()		\n"
"{			\n"
"	gl_Position = gWorld * vec4(Position, 1.0);	\n"
"}";

static const char *pFS = "#version 330	\n"
"out vec4 FragColor;	\n"
"void main()	\n"
"{		\n"
"	FragColor = vec4(1.0, 1.0, 0.0, 1.0);	\n"
"}";

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	static float Scale = 0.0f;
	Scale += 0.001f;
  
	Matrix4f World;
	World.m[0][0] = 1.0f; World.m[0][1] = 0.0f; World.m[0][2] = 0.0f; World.m[0][3] = sin(Scale);
	World.m[1][0] = 0.0f; World.m[1][1] = 0.0f; World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
	World.m[2][0] = 0.0f; World.m[2][1] = 0.0f; World.m[2][2] = 1.0f; World.m[2][3] = 0.0f;
	World.m[3][0] = 0.0f; World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f; 
	
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}


static void InitializeGlutCallBacks()
{
	glutDisplayFunc(&RenderSceneCB);
	glutIdleFunc(&RenderSceneCB);
}

static void CreateVertexBuffer()
{
	Vector3f Vertices[3];
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3f(0.0f, 1.0f, 0.0f);
	Vertices[2] = Vector3f(1.0f, -1.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error Creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = ::strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint Success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &Success);
	if (!Success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error Compiling shader type %d : '%s'\n",
		ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
	
	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);
	
	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};
	
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error Linking shader Program: '%s'\n", ErrorLog);
		exit(1);
	}
	
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
	
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program : '%s'\n", ErrorLog);
		exit(1);
	}
	
	glUseProgram(ShaderProgram);
}


int main(int argc, char **argv) 
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(1024, 768);
  glutCreateWindow("Tutorial 06");

  InitializeGlutCallBacks();

  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error : '%s\n", glewGetErrorString(res));
    return 1;
  }

  printf("GL version : %s\n", glGetString(GL_VERSION));
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  CreateVertexBuffer();

  CompileShaders();

  glutMainLoop();
  return 0;
}
