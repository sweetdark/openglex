#include "gltools.h"
#include <math.h>
#include <stdio.h>

static GLbyte *pImage = NULL;
static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;


static GLint iRenderMode = 1;
static GLboolean bHistogram = GL_FALSE;

void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	pImage = gltLoadTGA("horse.tga", &iWidth, &iHeight, &iComponents, &eFormat);
}

void ShutdownRC()
{
	if (pImage)
	{
		free(pImage);
		pImage = NULL;
	}
}

void RenderScene()
{
	if (!pImage)
		return;

	glClear(GL_COLOR_BUFFER_BIT);
	//反转的颜色表�?
	static GLubyte invertTable[256][3];
	//黑白颜色 缩放矩阵
	static GLfloat lumMat[16] = { 0.30f, 0.30f, 0.30f, 0.0f,
																0.59f, 0.59f, 0.59f, 0.0f,
																0.11f, 0.11f, 0.11f, 0.0f,
																0.0f, 0.0f, 0.0f, 1.0f};
	//锐化卷积�?
	static GLfloat mSharpen[3][3] = {
		{0.0f, -1.0f, 0.0f},
		{-1.0f, 5.0f, -1.0f},
		{0.0f, -1.0f, -0.0f}};

	//浮雕卷积�?
	static GLfloat mEmboss[3][3] = {
		{2.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, -1.0f}
	};

	static GLint histoGram[256];

	glRasterPos2i(0, 0);
	
	GLint viewPort[4];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glPixelZoom((GLfloat)viewPort[2] / iWidth, (GLfloat)viewPort[3] / iHeight);

	if (bHistogram == GL_TRUE)
	{
		glMatrixMode(GL_COLOR);
		glLoadMatrixf(lumMat);
		glMatrixMode(GL_MODELVIEW);

		glHistogram(GL_HISTOGRAM, 256, GL_LUMINANCE, GL_FALSE);
		glEnable(GL_HISTOGRAM);
	}

	switch(iRenderMode)
	{
		case 5: // 锐化
			glConvolutionFilter2D(GL_CONVOLUTION_2D, GL_RGB, 3, 3, GL_LUMINANCE, GL_FLOAT, mSharpen);
			glEnable(GL_CONVOLUTION_2D);
	}

	glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void ProcessMenu(int value)
{
  iRenderMode = value;
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
	glutInitWindowSize(600 ,600);
	glutCreateWindow("OpenGL Imaging subset");

	// Check for imaging subset, must be done after window
	// is create or there won't be an OpenGL context to query
	if(gltIsExtSupported("GL_ARB_imaging") == 0)
	{
		printf("Imaging subset not supported\r\n");
		return 0;
	}

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	// Create the Menu and add choices
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Save Image",0);
	glutAddMenuEntry("Raw Stretched Image",1);
	glutAddMenuEntry("Increase Contrast",2);
	glutAddMenuEntry("Invert Color", 3);
	glutAddMenuEntry("Emboss Image", 4);
	glutAddMenuEntry("Sharpen Image", 5);
	glutAddMenuEntry("Histogram", 6);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();          // Do setup

	glutMainLoop();     // Main program loop

	ShutdownRC();       // Do shutdown

	return 0;
}

