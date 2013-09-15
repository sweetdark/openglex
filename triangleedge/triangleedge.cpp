#include <gl\glut.h>
#include <gl\GL.h>
#include <math.h>

//五角星的8个点
static GLfloat points[16] =
{
-50.0f, 50.0f,
0.0f, 125.0f,
50.0f, 50.0f,
125.0f, 0.0f,
50.0f, -50.0f,
0.0f, -125.0f,
-50.0f, -50.0f,
-125.0f, 0.0f
};

static GLboolean bFlag = false;

static void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0f, 1.0f, 0.0f);
  glPolygonMode(GL_FRONT, GL_LINE);
  glPolygonMode(GL_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
    //五角星的内部两个三角形不是边界
    glEdgeFlag(bFlag);
    glVertex2f(points[0], points[1]);
    glVertex2f(points[4], points[5]);
    glVertex2f(points[12], points[13]);

    glVertex2f(points[4], points[5]);
    glVertex2f(points[8], points[9]);
    glVertex2f(points[12], points[13]);
    glEdgeFlag(true);

    glVertex2f(points[0], points[1]);
    glVertex2f(points[2], points[3]);
    glEdgeFlag(bFlag);
    glVertex2f(points[4], points[5]);

    glVertex2f(points[4], points[5]);
    glEdgeFlag(true);
    glVertex2f(points[8], points[9]);
    glVertex2f(points[6], points[7]);

    glVertex2f(points[8], points[9]);
    glVertex2f(points[10], points[11]);
    glEdgeFlag(bFlag);
    glVertex2f(points[12], points[13]);

    glVertex2f(points[12], points[13]);
    glEdgeFlag(true);
    glVertex2f(points[0], points[1]);
    glVertex2f(points[14], points[15]);

  glEnd();
  glutSwapBuffers();
}

static void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

static void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);
  GLfloat range = 150.0f;
  GLfloat aspect = (GLfloat)w / (GLfloat)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(w <= h)
  {
    glOrtho(-range, range, -range/aspect, range/aspect, -range, range);
  }
  else
  {
    glOrtho(-range*aspect, range*aspect, -range, range, -range, range);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutPostRedisplay();
}
int main(int args, char* argv[])
{
  glutInit(&args, argv);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(300, 300);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("edge");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();
  return 0;
}