#include "gltools.h"
#include "math3d.h"
#include "glframe.h"
#include <stdio.h>

GLFrame objFrame;

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void SetupRC()
{
  glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
  
  //开启深度检测，消除隐藏面
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  //开启光照
  GLfloat whiteLight[] = {0.05f, 0.05f, 0.05f, 1.0f};
  GLfloat sourceLight[] = {0.25f, 0.25f, 0.25f, 1.0f};
  GLfloat lightPos[] = {-5.0f, 15.0f, 5.0f, 1.0f};

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, sourceLight);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHT0);

  //开启颜色追踪
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);
  GLfloat aspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0, aspect, 1.0, 40.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glPushMatrix();
    glTranslatef(0.0f, -.6f, -8.0f);
    objFrame.ApplyCameraTransform();
    gltDrawUnitAxes();
    GLUquadricObj *quadricObj = gluNewQuadric();
    gluQuadricDrawStyle(quadricObj, GLU_LINE);
    gluDisk(quadricObj, 0.2, 0.7, 26, 13);
  glPopMatrix();
  glutSwapBuffers();
}

void SpecialKey(int value, int x, int y)
{
  if (value == GLUT_KEY_RIGHT)
  {
    //objFrame.MoveForward(0.5f);
    objFrame.RotateLocalY(0.5f);
    yRot += 0.5f;
  }

  if (value == GLUT_KEY_LEFT)
  {
    //objFrame.MoveForward(-0.5f);
    objFrame.RotateLocalY(-0.5f);
    yRot -= 0.5f;
  }

  if (value == GLUT_KEY_UP)
  {
    objFrame.MoveUp(0.5f);
    xRot += 0.5f;
  }

  if (value == GLUT_KEY_DOWN)
  {
    objFrame.MoveUp(-0.5f);
    xRot -= 0.5f;
  }

  glutPostRedisplay();
}
int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
  glutInitWindowSize(800, 600);
  glutCreateWindow("quadric");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();

  glutSpecialFunc(SpecialKey);
  glutMainLoop();
  return 0;
}
