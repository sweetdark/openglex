#include "gltools.h"
#include <math.h>

#define POLYGON_DRAW(num1,num2,num3,num4) \
  glBegin(GL_POLYGON); \
  glColor3fv(points[num1]); \
  glVertex3fv(points[num1]); \
  glColor3fv(points[num2]); \
  glVertex3fv(points[num2]); \
  glColor3fv(points[num3]); \
  glVertex3fv(points[num3]); \
  glColor3fv(points[num4]); \
  glVertex3fv(points[num4]); \
  glEnd();
static GLfloat points[8][3] = 
{
  { 0.0f, 0.0f, 0.0f } , { 255.0f, 0.0f, 0.0f } , { 255.0f, 255.0f, 0.0f } , { 0.0f, 255.0f, 0.0f },
  { 0.0f, 255.0f, 255.0f }, { 0.0f, 0.0f, 255.0f }, { 255.0f, 0.0f, 255.0f }, { 255.0f, 255.0f, 255.0f }
};

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
}

static void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
  {
    h = 1;
  }

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLfloat range = 300.0f;
  GLfloat aspect = (GLfloat)w / (GLfloat)h;
  if (w >= h)
  {
    glOrtho(-range, range, -range / aspect, range / aspect, -range, range);
  }
  else
  {
    glOrtho(-range * aspect, range * aspect, -range, range, -range, range);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutPostRedisplay();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  static GLfloat yRot = 0.0f;
  yRot += 0.5f;
  if (yRot > 360.0f)
  {
    yRot = 0.0f;
  }
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(-100.0f, -100.0f, 0.0f);
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  //背面
  POLYGON_DRAW(3, 2, 1, 0)
  //左侧面
  POLYGON_DRAW(0, 5, 4, 3) 
  //正面
  POLYGON_DRAW(4, 5, 6, 7)
  //右侧面
  POLYGON_DRAW(6, 1, 2, 7)
  //上面
  POLYGON_DRAW(2, 3, 4, 7)
  //下面
  POLYGON_DRAW(0, 1, 6, 5)
  glPopMatrix();
  glutSwapBuffers();
}

void TimerFunc(int value)
{
  glutPostRedisplay();
  glutTimerFunc(50, TimerFunc, 1);
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800,800);
  glutCreateWindow("Cube Demo");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  glutTimerFunc(50, TimerFunc, 1);
  SetupRC();

  glutMainLoop();

  return 0;
}
