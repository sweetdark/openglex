#include <gl/glut.h>
#include <gl/gl.h>

static GLfloat x = 50.0f;
static GLfloat y = 50.0f;
static GLfloat z = 20.0f;

static void RenderSences()
{
	glClearColor(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();

	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
	{
		h = 1;
	}
	glViewPort(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat aspect = (GLfloat)w / (GLfloath);
	if (w <= h)
	{
		//glOrtho();
	}
}
int main(int args, char **argv)
{
	glutInit(&args, argv);
	glutInitWindowSize(200, 200);
	glutCreateWindow("draw points");
	glutDisplayFunc(RenderSences);	
	glutReshapeFunc(ChangeSize);
	glutMainLoop();
	return 0;
}
