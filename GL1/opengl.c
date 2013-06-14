#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const GLfloat factor = 0.1f;

//void MyDisplay()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glRectf(-0.8f, -0.8f, 0.8f, 0.8f);
//	glFlush();
//}
//»­ÕýÏÒ
//void MyDisplay()
//{
//	GLfloat x;
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_LINES);
//	glVertex2f(1.0f, 0.0f);
//	glVertex2f(-1.0f, 0.0f);
//	glVertex2f(0.0f, 1.0f);
//	glVertex2f(0.0f, -1.0f);
//	glEnd();
//	glBegin(GL_LINE_STRIP);
//	for (x = -1.0f/factor; x < 1.0f/factor; x += 0.01f )
//	{
//		glVertex2f(x * factor, sin(x) * factor);
//	}
//	glEnd();
//	glFlush();
//}

//void MyDisplay()
//{
//	GLubyte Mask[128];
//	FILE *fp;
//	fp = fopen("d:\\mask.bmp", "rb");
//	if (!fp)
//		exit(0);
//	if (fseek(fp, -(int)sizeof(Mask), SEEK_END))
//		exit(0);
//	if (!fread(Mask, sizeof(Mask), 1, fp))
//	{
//		exit(0);
//	}
//	fclose(fp);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glColor3f(0.0f, 0.1f, 0.2f);
//	glEnable(GL_POLYGON_STIPPLE);
//	glPolygonStipple(Mask);
//	glRectf(-0.5f, -0.5f, 0.0f, 0.0f);
//	glDisable(GL_POLYGON_STIPPLE);
//	glRectf(0.0f, 0.0f, 0.5f, -.5f);
//	glFlush();
//}
//static int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//	glutInitWindowPosition(200,200);
//	glutInitWindowSize(400, 400);
//	glutCreateWindow("Hello World");
//	glutDisplayFunc(&MyDisplay);
//	glutMainLoop();
//	return 0;
//}