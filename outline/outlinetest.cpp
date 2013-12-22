#include "gltools.h"

static GLfloat angle = 0.0f;

static int iMode = 0;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

}

void DrawQuad()
{
  glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, -5.0f);
    glVertex3f(-1.0f, -1.0f, -5.0f);
    glVertex3f(1.0f, -1.0f, -5.0f);
    glVertex3f(1.0f, 1.0f, -5.0f);
    glEnd();
  glPopMatrix();
}

void DrawNormal()
{
  glPushMatrix();
  //往里移动并旋转
  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(angle, 1.0f, 1.0f, 0.0f);

  //画线框立方体
  glColor3f(1.0f, 0.0f, 0.0f);
  glLineWidth(3.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutSolidCube(1);

  //画实心立方体
  glColor3f(0.0f, 0.0f, 0.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glutSolidCube(1);

  glPopMatrix();
}

void UseOffset()
{
  //保存之前的矩阵和属性
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  //往里移动并旋转
  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(angle, 1.0f, 1.0f, 0.0f);

  //画线框立方体,制造轮廓
  glColor3f(1.0f, 0.0f, 0.0f);
  //设置线的宽度为3
  glLineWidth(3.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutSolidCube(1);

  //画实心立方体
  glColor3f(0.0f, 0.0f, 0.0f);
  //设置多边形偏移，往屏幕外靠近观察点进行偏移
  glPolygonOffset(-1.5, -1.0f);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glutSolidCube(1);

  glPopAttrib();
  glPopMatrix();
}

void UseStencil()
{
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  //清除模板缓冲区
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);
  //一开始设置为总是通过模板测试，建立模板
  glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
  //模板测试失败时与模板测试通过但深度测试失败时，模板的值保持不修改.
  //通过模板测试与深度测试时，使用上面glStencilFunc指定的ref（1）替换掉
  glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
  glEnable( GL_STENCIL_TEST );


  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(angle, 1.0f, 1.0f, 0.0f);

  //绘制实体的立方体
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glColor3f(0.0f, 0.0f, 0.0f);
  glutSolidCube(1);

  //设置只有模板缓冲区值不为1的，地方才通过模板测试
  glStencilFunc(GL_NOTEQUAL, 1, 0xFFFF);
  //绘制线框立方体
  glLineWidth(3.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidCube(1);
  
  glPopAttrib();
  glPopMatrix();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (iMode == 1)
  {
    UseOffset();
  }
  else if (iMode == 2)
  {
    UseStencil();
  }
  else
  {
    DrawNormal();
  }
  
  glutSwapBuffers();

}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  GLfloat aspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0, aspect, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

void TimerFunc(int value)
{
  angle += 0.5f;
  if (angle > 360.5f)
  {
    angle = 0.0f;
  }

  glutPostRedisplay();
  glutTimerFunc(50, TimerFunc, 1);
}

void ProcessMenu(int value)
{
  iMode = value;
  glutPostRedisplay();
}

int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
  glutCreateWindow("outline");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();

  glutTimerFunc(50, TimerFunc, 1);

  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("normal", 0);
  glutAddMenuEntry("polygon offset", 1);
  glutAddMenuEntry("stencil", 2);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutMainLoop();

  return 0;
}