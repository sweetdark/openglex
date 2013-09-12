#include "headers.h"

static void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(50.0f, 0.0f);
		glVertex2f(50.0f, 50.0f);
		glVertex2f(0.0f, 50.0f);
	glEnd();

	glFlush();
}

static void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	GLfloat range = 100.0f;
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w <= h)
	{
		glOrtho(-range, range, -range/aspect, range/aspect, -range, range);
	}
	else
	{
		glOrtho(-range*aspect, range*aspect, -range, range, -range, range);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}
int main(int args, char *argv[])
{
	glutInit(&args, argv);
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(200, 200);
	glutInitDisplayMode(GLUT_RGBA);
  glutCreateWindow("polygon");
	SetupRC();
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMainLoop();
	return 0;
}
