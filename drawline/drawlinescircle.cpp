#include <gl/glut.h>
#include <gl/gl.h>
#include <math.h>

static GLfloat GL_PI = 3.14159f;
static GLfloat xRot = 20.0f;
static GLfloat yRot = 30.0f;

//绘制场景
static void RenderSences()
{
  GLfloat z = -50.0f;  
  //清空画布
  glClear(GL_COLOR_BUFFER_BIT);
  //设置画图使用的颜色
  glColor3f(1.0f, 0.0f, 0.0f);
  //保存矩阵
  glPushMatrix();
  //旋转, 如果不旋转，那些不同层次的点都重叠在一起，看不出3D效果了。
  glRotatef(xRot, 1.0f, 0.0f, 0.0f);
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  glBegin(GL_LINE_STRIP);
  for (GLfloat a = 0.0; a < GL_PI * 3.0f; a += 0.1f)
  {
    GLfloat x = 50.0f * cos(a);
    GLfloat y = 50.0f * sin(a);
    //设置点的大小
    //设置顶点
    glVertex3f(x, y, z);
    //调整z值
    z += 0.5f;
  }
  glEnd();
  glPopMatrix();	
  glFlush();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  GLfloat nRange = 100.0f;
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
//初始化工作
static void SetupRC()
{
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}
//创建窗口
static void CreateWindow() {
  glutInitWindowSize(200, 200);
  glutCreateWindow("3d Points");
}

static void SpecialKey(int key, int x, int y)
{
  if (key == GLUT_KEY_UP)
  {
    xRot += 5.0f;
  }
  if (key == GLUT_KEY_DOWN)
  {
    xRot -= 5.0f;
  }
  if (key == GLUT_KEY_LEFT)
  {
    yRot += 5.0f;
  }
  if (key == GLUT_KEY_RIGHT)
  {
    yRot -= 5.0f;
  }

  xRot = (GLint)xRot % 360;
  yRot = (GLint)yRot % 360;
  glutPostRedisplay();
}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(200, 200);
  glutCreateWindow("draw points");
  SetupRC();
  glutDisplayFunc(RenderSences);	
  glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKey);
  glutMainLoop();
  return 0;
}
