#include <gl\glut.h>
#include <stdio.h>
#include <stdlib.h>

static float rotAngle = 0.0f;

static const GLfloat v1[] = {
	-0.5, 0.5,
	0.5, -0.5
};

static const GLfloat v2[] = {
	0.5, 0.5,
	-0.5, -0.5
};

static void init() 
{
	/*GLint v[2] = {0, 0};
	glGetIntegerv(GL_SMOOTH_LINE_WIDTH_RANGE, v);
	printf("smotth line widht range is %d - %d \n", v[0], v[1]);*/
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth(1.5);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

static void RenderSenceCB(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glVertexPointer(2, GL_FLOAT, 0, v1);
	glEnableClientState(GL_VERTEX_ARRAY );
	glColor4f(1.0, 1.0, 0.0, 1.0);
	glPushMatrix();
	glRotatef(-rotAngle, 0.0, 0.0, 0.1);
	glDrawArrays(GL_LINES, 0, 3);
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, v2);
	glEnableClientState(GL_VERTEX_ARRAY );
	glColor4f(0.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glRotatef(rotAngle, 0.0, 0.0, 0.1);
	glDrawArrays(GL_LINES, 0, 3);
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);

	glFlush();
	glutSwapBuffers();
}

static void InitGlutCallbacks()
{
	glutDisplayFunc(&RenderSenceCB);
}

int main(int args, char **argv)
{
	glutInit(&args, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("·´×ßÑù");
	init();
	InitGlutCallbacks();
	
	glutMainLoop();
}