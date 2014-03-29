#include "gltools.h"
#include "math3d.h"

GLfloat lightPos[4] = {50.0f, 50.0f, 100.0f};
GLfloat camaraPos[4] = {0.0f, 30.0f, 50.0f};
M3DMatrix44f textureMatrix;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
  

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 128);
  glEnable(GL_COLOR_MATERIAL);

  GLint iWidth, iHeight, iCompnent;
  GLenum eFormat;

  GLubyte *pByte = gltLoadTGA("..\stone.tga", &iWidth, &iHeight, &iCompnent, &eFormat);
}


void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  gluLookAt(camaraPos[0], camaraPos[1], camaraPos[2], 0, 0, 0, 0, 1, 0);

  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidCube(10.0f);

  glPopMatrix();

  glutSwapBuffers();
}

void ChangeSize(int w, int h)
{
  if (h == 0)
    h = 1;

  GLfloat aspect = (GLfloat)w/(GLfloat)h;
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(40.0, aspect, 1.0 , 200.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void SpecialKey(int value, int x, int y)
{
  switch(value)
  {
  case GLUT_KEY_LEFT:
    camaraPos[0] += 5.0f;
    break;
  case GLUT_KEY_RIGHT:
    camaraPos[0] -= 5.0f;
    break;
  case GLUT_KEY_DOWN:
    camaraPos[2] += 5.0f;
    break;
  case GLUT_KEY_UP:
    camaraPos[2] -= 5.0f;
    break;
  }

  glutPostRedisplay();
}


int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("eye plane");

  SetupRC();
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKey);
  glutMainLoop();

  return 0;

}

