#include "gltools.h"
#include "math3d.h"

GLint numOfPoints = 4;
static GLfloat controlPoints[4][3] = {{-4.0f, 0.0f, 0.0f},
{-6.0f, 4.0f, 0.0f},
{6.0f, -4.0f, 0.0f},
{4.0f, 0.0f, 0.0f}};

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glColor3f(1.0f, 0.0f, 1.0f);
}

void DrawPoints()
{
  glPointSize(2.5f);
  glBegin(GL_POINTS);
    for (int i = 0; i < numOfPoints; ++i)
    {
      glVertex3fv(controlPoints[i]);
    }
  glEnd();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
  {
    h = 1;
  }

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-10.0f, 10.0f, -10.0f, 10.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  glMap1f(GL_MAP1_VERTEX_3, 0.0f, 100.0f, 3, numOfPoints, &controlPoints[0][0]);
  //必须在绘制顶点之前开启
  glEnable(GL_MAP1_VERTEX_3);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < 100; i++)
  {
    glEvalCoord1f((GLfloat)i);
  }

  glEnd();

  glMapGrid1f(100, 0.0, 100.0);

  glEvalMesh1(GL_LINE, 0, 100);
  DrawPoints();

  glutSwapBuffers();

}

int main (int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Bezier");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();

  return 0;

}