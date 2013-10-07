#include <gl/glut.h>
#include <gl/GL.h>
#include <math.h>

static void SetupRC()
{

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

static void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glShadeModel(GL_SMOOTH);
    // Draw the triangle
    glBegin(GL_TRIANGLES);
    // Red Apex
    glColor3ub((GLubyte)255,(GLubyte)0,(GLubyte)0);
    glVertex3f(0.0f,200.0f,0.0f);
    // Green on the right bottom corner
    glColor3ub((GLubyte)0,(GLubyte)255,(GLubyte)0);
    glVertex3f(200.0f,-70.0f,0.0f);
    // Blue on the left bottom corner
    glColor3ub((GLubyte)0,(GLubyte)0,(GLubyte)255);
    glVertex3f(-200.0f, -70.0f, 0.0f);
  glEnd();

  glutSwapBuffers();
}

static void ChangeSize(GLsizei w, GLsizei h)
{

  GLfloat nRange = 300.0f;
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


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(800,800);
  glutCreateWindow("color triangel");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  glutMainLoop();

  return 0;
}

