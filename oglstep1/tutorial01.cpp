#include <gl/glut.h>
#include <gl/GL.h>

static void RenderScenceCB() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

static void CreateWindow() {
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(200, 400);
	glutCreateWindow("opengl step 1");
}

int main(int args, char *argc[]) {
	glutInit(&args, argc);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	CreateWindow();
	glutDisplayFunc(RenderScenceCB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutMainLoop();
	return 0;
}