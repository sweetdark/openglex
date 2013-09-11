#include "headers.h"


static void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat x, y, z, angle;
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	bool color;

	for (angle = 0.0f; angle <= (2 * GL_PI); angle += (GL_PI / 8))
	{
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);
		color = !color;
	
		glVertex2f(x, y);

		if(color)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(0.0f, 1.0f, 1.0f);
	}
	glEnd();

	glutSwapBuffers();
}

static void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CW);
	glShadeModel(GL_FLAT);
}



void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;
	if (h == 0)
	{
		h = 1;
	}
	//设置视口
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//设置可视区域
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		glOrtho(-nRange, nRange, -nRange/aspect, nRange/aspect, -nRange, nRange);
	}
	else
	{
		glOrtho(-nRange*aspect, nRange*aspect, -nRange, nRange, -nRange, nRange);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int args, char **argv)
{
	glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(200, 200);
	glutInitWindowSize(200, 200);
	glutCreateWindow("draw points");
  SetupRC();
	glutDisplayFunc(RenderSences);	
	glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKey);
	glutMainLoop();
	return 0;
}

