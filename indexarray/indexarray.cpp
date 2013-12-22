#include "gltools.h"

static GLfloat cube[]={-1.0f, -1.0f, -5.0f, //前面的正方形
1.0f, -1.0f,-5.0f,
1.0f, 1.0f, -5.0f, 
-1.0f, 1.0f, -5.0f,
-1.0f, -1.0f, -10.0f,//背面的正方形
1.0f, -1.0f, -10.0f,
1.0f, 1.0f, -10.0f,
-1.0f, 1.0f, -10.0f};

static GLubyte index[]={0, 1, 2, 3, //前面
0, 3, 7, 4, //左面
5, 6, 2, 1, //右面
7, 6, 5, 4, //后面
3, 2, 6, 7, //上面
1, 0, 4, 5 //地面
};

void SetupRC()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0f, 0.0f, 1.0f);

  glPushMatrix();
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cube);

  glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, index);

  glDisableClientState(GL_VERTEX_ARRAY);
  glPopMatrix();
  glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  GLfloat fAspect = (GLfloat)w / (GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0f, fAspect, 1.0f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int args, char* argv[])
{
  glutInit(&args, argv);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("index array");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();
  return 0;
}