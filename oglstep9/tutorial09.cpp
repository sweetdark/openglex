#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include "math_3d.h"
#include <assert.h>
#include <string.h>

GLint gWorldLocation;
GLuint VBO;

const char *pVS = "#version 330  \n"
"layout (location=0) in vec3 Position; \n"
"uniform mat4 gWorld;  \n"
"out vec4 Color;  \n"
"void main()  \n"
"{ \n"
"	gl_Position = gWorld * vec4(Position, 1.0);	\n"
"	Color = vec4(clamp(Position, 0.0, 1.0), 1.0);	\n"
"}";

static const char *pFS = "#version 330	\n"
"out vec4 FragColor; \n "
"in vec4 Color;	\n"
"void main() \n"
"{	\n"
"	FragColor = Color;	\n"
"}";

static void RendersenceCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;
	Scale += 0.001f;

	Matrix4f World;
	World.m[0][0]=sinf(Scale); World.m[0][1]=0.0f;        World.m[0][2]=0.0f;        World.m[0][3]=0.0f;
	World.m[1][0]=0.0f;        World.m[1][1]=sinf(Scale); World.m[1][2]=0.0f;        World.m[1][3]=0.0f;
	World.m[2][0]=0.0f;        World.m[2][1]=0.0f;        World.m[2][2]=sinf(Scale); World.m[2][3]=0.0f;
	World.m[3][0]=0.0f;        World.m[3][1]=0.0f;        World.m[3][2]=0.0f;        World.m[3][3]=1.0f;

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
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

static void InitializeGlutCallBacks()
{
	glutDisplayFunc(&RendersenceCB);
	glutIdleFunc(&RendersenceCB);
}

static void AddShader(GLuint ShaderProgram, GLenum ShaderType, const char* ShaderText)
{
	GLuint ShaderObj = glCreateShader(ShaderType);
	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error can not create Shader type %d\n");
		exit(1);
	}

	GLint Length[1];
	const GLchar *string[1];
	string[0] = ShaderText;
	Length[0] = ::strlen(ShaderText);
	glShaderSource(ShaderObj, 1, string, Length);
	GLenum Success;
	Success = glGetError();
	if (GL_NO_ERROR != Success)
	{
		fprintf(stderr, "Error:shadersource is not valid \n");
		exit(1);
	}
	glCompileShader(ShaderObj);
	GLint status;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		GLchar InfoLog[1024] = {0};
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error : compile error '%s' \n ", InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);

}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	AddShader(ShaderProgram, GL_VERTEX_SHADER, pVS);
	AddShader(ShaderProgram, GL_FRAGMENT_SHADER, pFS);

	glLinkProgram(ShaderProgram);
	GLint Success;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		GLchar InfoLog[1024] = {0};
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, InfoLog);
		fprintf(stderr, "Error : Link Error '%s'\n", InfoLog);
		exit(0);
	}
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");

	assert(gWorldLocation != 0xFFFFFFFF);
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		GLchar InfoLog[1024] = {0};
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, InfoLog);
		fprintf(stderr, "Error : Invalid shader program '%s'\n", InfoLog);
		exit(1);
	}
	glUseProgram(ShaderProgram);
}

int main(int args, char **argv)
{
	glutInit(&args, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1024, 600);
	glutCreateWindow("tutorial 8");

	InitializeGlutCallBacks();

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "glew init error \n");
		return 1;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	CreateVertexBuffer();
	CompileShaders();
	glutMainLoop();
	system("pause");
	return 0;
}