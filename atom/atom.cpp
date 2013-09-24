#include <gl/glut.h>
#include <gl/GL.h>
#include <math.h>

static void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

static void RenderScene()
{
  static GLfloat fEffect = 0.0f;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //先往里平移画原子核
  glTranslatef(0.0f, 0.0f, -70.0f);
  //红色的原子核
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidSphere(20.0f, 15, 15);
  //青色的电子
  glColor3f(0.0f, 1.0f, 1.0f);
  glPushMatrix();
  glRotatef(fEffect, 0.0f, 1.0f, 0.0f);
  glTranslatef(80.0f, 0.0f, 0.0f);

  glutSolidSphere(10.0f, 15, 15);
  glPopMatrix();
  //黄色的电子
  glColor3f(1.0f, 1.0f, 0.0f);
  glPushMatrix();
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(fEffect, 0.0f, 1.0f, 0.0f);
  glTranslatef(80.0f, 0.0f, 0.0f);

  glutSolidSphere(10.0f, 15, 15);
  glPopMatrix();
  //绿色的电子
  glColor3f(0.0f, 1.0f, 0.0f);
  glPushMatrix();
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(fEffect, 0.0f, 1.0f, 0.0f);
  glTranslatef(80.0f, 0.0f, 0.0f);

  glutSolidSphere(10.0f, 15, 15);
  glPopMatrix();


  fEffect += 10.0f;
  if (fEffect > 360.0)
  {
    fEffect = 0.0f;
  }
  glutSwapBuffers();
}

static void ChangeSize(GLsizei w, GLsizei h)
{

  GLfloat nRange = 200.0f;
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
  //设置为透视
  gluPerspective(85.0, aspect, 80.0, 300.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(100, Timer, 1);
}


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(400,400);
  glutCreateWindow("atom Test");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  glutTimerFunc(100, Timer, 1);
  SetupRC();
  glutMainLoop();
  return 0;
}
