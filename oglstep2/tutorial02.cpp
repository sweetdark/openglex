#include <gl/glew.h>
#include <gl/glut.h>
#include <stdio.h>
#include "math_3d.h"

GLuint VBO;
static void CreateWindow() {
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("opengl step 2");
}

static void RenderPoint() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}

static void CreateVertexBuffer() {
	Vector3f Vertices[1];
	Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

}
static void InitializeGlutCallback() {
	glutDisplayFunc(RenderPoint);
}
int main(int args, char *argc[]) {
	glutInit(&args, argc);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	CreateWindow();
	
	
	InitializeGlutCallback();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	CreateVertexBuffer();
	glutMainLoop();
	return 0;
}
