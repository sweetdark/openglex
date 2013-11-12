#include "gltools.h"
#include "math3d.h"

static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;
static GLfloat xRot, yRot;

static GLfloat noLight[4] = {0.0f, 0.0f, 0.0f, 1.0f};
static GLfloat ambientLight[4] = {0.3f, 0.3f, 0.3f, 1.0f};
static GLfloat diffuseLight[4] = {0.7f, 0.7f, 0.7f, 1.0f};
static GLfloat brightLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat lightPos[] = { 5.0f, 5.0f, 5.0f, 1.0f};

void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, brightLight);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHT0);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, brightLight);
  glMateriali(GL_FRONT, GL_SHININESS, 20);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	void *pImage = NULL;
	pImage = gltLoadTGA("..\\stone.tga", &iWidth, &iHeight, &iComponents, &eFormat);

	if (pImage)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
		free(pImage);
		pImage = NULL;
	}

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	M3DVector3f vertices[5] = {{0.0f, 0.8f, 0.0f},
		{-.50f, 0.0f, -.50f},
		{.50f, 0.0f, -.50f},
		{-.50f, 0.0f, .50f},
		{.50f, 0.0f, .50f}};

  M3DVector3f normal;

  glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.2, 12, 12);
    glEnable(GL_LIGHTING);
  glPopMatrix();

	glPushMatrix();
    glTranslatef(0.0f, -0.3f, -4.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    xRot += 0.1f;
    yRot += 0.5f;

		glBegin(GL_TRIANGLES);
      glColor3f(1.0f, 0.0f, 0.0f);
  		glNormal3f(0.0f, -1.0f, 0.0f);
  		glVertex3fv(vertices[1]);
  		glVertex3fv(vertices[2]);
  		glVertex3fv(vertices[4]);

  		glVertex3fv(vertices[4]);
  		glVertex3fv(vertices[3]);
  		glVertex3fv(vertices[1]);

      //前面
      glColor3f(0.0f, 1.0f, 0.0f);
  		m3dFindNormal(normal, vertices[0], vertices[3], vertices[4]);
      glNormal3fv(normal);
  		glVertex3fv(vertices[0]);
  		glVertex3fv(vertices[3]);
  		glVertex3fv(vertices[4]);

      //左侧面
      glColor3f(0.0f, 0.0f, 1.0f);
      m3dFindNormal(normal, vertices[1], vertices[3], vertices[0]);
      glNormal3fv(normal);
      glVertex3fv(vertices[1]);
      glVertex3fv(vertices[3]);
      glVertex3fv(vertices[0]);

      //右侧面
      glColor3f(0.0f, 1.0f, 1.0f);
      m3dFindNormal(normal, vertices[0], vertices[4], vertices[2]);
      glNormal3fv(normal);
      glVertex3fv(vertices[0]);
      glVertex3fv(vertices[4]);
      glVertex3fv(vertices[2]);

      //后面
      glColor3f(1.0f, 0.0f, 1.0f);
      m3dFindNormal(normal, vertices[0], vertices[2], vertices[1]);
      glNormal3fv(normal);
      glVertex3fv(vertices[0]);
      glVertex3fv(vertices[2]);
      glVertex3fv(vertices[1]);

		glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	float fAspect = (GLfloat)w/(GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35.0, fAspect, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void TimerFunc(int value)
{
  glutPostRedisplay();
  glutTimerFunc(60, TimerFunc, 1);
}


int main(int args, char *arv[])
{
	glutInit(&args, arv);
	glutInitDisplayMode(GL_RGB | GL_DOUBLE | GL_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("pyramid");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();

  glutTimerFunc(50, TimerFunc, 1);

	glutMainLoop();

	return 0;
}

