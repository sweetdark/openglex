#include <gl\glut.h>
#include <stdlib.h>


const GLfloat v[] =
{
  0.1, 0.9, 0.0,
  0.1, 0.1, 0.0,
  0.7, 0.5, 0.0,
  0.9, 0.9, 0.0,
  0.3, 0.5, 0.0,
  0.9, 0.1, 0.0
};

static void  init()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_FLAT);
  glVertexPointer(3, GL_FLOAT, 0, v);

  glEnableClientState(GL_VERTEX_ARRAY);
}
static void DrawLeft()
{
  glColor4f(1.0, 1.0, 0.0, 0.75);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void DrawRight()
{
  glColor4f(0.0, 1.0, 1.0, 0.75);
  glDrawArrays(GL_TRIANGLES, 3, 3);
}
static void RenderSenceCB()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  DrawLeft();
  DrawRight();
  glFlush();
  glutSwapBuffers();
}

static void InitializeGlutCallBacks()
{
  glutDisplayFunc(&RenderSenceCB);
}

//int main(int args, char **argv)
//{
//  glutInit(&args, argv);
//  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//  glutInitWindowPosition(100, 0);
//  glutInitWindowSize(600, 600);
//  glutCreateWindow("alpha");
//  init();
//  InitializeGlutCallBacks();
//  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
// 
//  glutMainLoop();
//  return 0;
//}