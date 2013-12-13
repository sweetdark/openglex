#include "gltools.h"
#include "math3d.h"

//控制点
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

//画控制点
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

  //使用正交投影
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-10.0f, 10.0f, -10.0f, 10.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  //设置贝塞尔曲线，这个函数其实只需要调用一次，可以放在SetupRC中设置
  glMap1f(GL_MAP1_VERTEX_3, //生成的数据类型
    0.0f, //u值的下界
    100.0f, //u值的上界
    3, //顶点在数据中的间隔，x,y,z所以间隔是3
    numOfPoints, //u方向上的阶，即控制点的个数
    &controlPoints[0][0] //指向控制点数据的指针
  );

  //必须在绘制顶点之前开启
  glEnable(GL_MAP1_VERTEX_3);
  //使用画线的方式来连接点
  /*glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= 100; i++)
  {
    glEvalCoord1f((GLfloat)i);
  }
  glEnd();*/

  glMapGrid1f(100, 0.0f, 100.0f);

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