#include "gltools.h"
#include <stdio.h>

static GLfloat yRot = 0.0f;

#define SUN 1
#define EARTH 2
#define MOON 3

#define BUFFER_LENGTH 128
void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  
}

void RenderSphere()
{
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -10.0f);

  //初始化名称栈
  glInitNames();
  //往栈顶压栈，压如一个名称
  glPushName(0);
  glColor3f(1.0f, 0.0f, 0.0f);
  //用当前名称SUN替换掉栈顶名称
  glLoadName(SUN);
  glutSolidSphere(1.0, 26, 26);

  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  glTranslatef(2.0f, 0.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  //用EARTH替换掉栈顶名称
  glLoadName(EARTH);
  //glPushName(EARTH);
  glutSolidSphere(0.3, 26, 26);

  glTranslatef(1.0f, 0.0f, 0.0f);
  glColor3f(0.25f, 0.25f, 0.75f);
  //用当前名称MOON替换掉栈顶名称
  glLoadName(MOON);
  //glPushName(MOON);
  glutSolidSphere(0.1, 26, 26);
  glPopMatrix();
}
void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RenderSphere();
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

  gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void TimerFunc(int value)
{
  yRot += 0.5;
  if (yRot > 360.0f)
  {
    yRot = 0.0f;
  }
  glutPostRedisplay();
  glutTimerFunc(50, TimerFunc, 1);
}

//处理点击记录
void ProcessHit(int hits, GLuint *buf)
{
  for (int i = 1; i <= hits; ++i)
  {
    GLuint nameNum = *buf;
    printf("hit number %d \n", i);
    printf("name stack count is %d\n", *buf); buf++;
    printf("min z value is %g\n", (float)*buf/0x7FFFFFFF); buf++;
    printf("max z value is %g\n", (float)*buf/0x7FFFFFFF); buf++;
    printf("name value is : ");
    for (int j = 0; j < nameNum; ++j)
    {
      switch(*buf)
      {
      case SUN:
        printf("SUN \t");
        break;
      case EARTH:
        printf("EARTH \t");
        break;
      case MOON:
        printf("MOON \t");
        break;
      default:
        break;
      }
      buf++;
    }
    printf("\n");
  }
}

void ProcessSelection(int x, int y)
{
  GLint viewport[4], hits;

  static GLuint selectBuffer[BUFFER_LENGTH];
  //设置选择缓冲区
  glSelectBuffer(BUFFER_LENGTH, selectBuffer);

  //切换到投影矩阵，我们需要创建 可视区域
  glMatrixMode(GL_PROJECTION);
  //保留原先的 投影矩阵，以便恢复
  glPushMatrix();
    glLoadIdentity();
    //获得视口
    glGetIntegerv(GL_VIEWPORT, viewport);
    //切换到选择模式
    glRenderMode(GL_SELECT);
    GLfloat aspect = (GLfloat)viewport[2]/(GLfloat)viewport[3];
    //创建一个描述可视区域的矩阵
    gluPickMatrix(x, viewport[3]-y+viewport[1], 2, 2, viewport);
    //与投影矩阵相乘，得到可视区域
    gluPerspective(35.0, aspect, 1.0, 200.0);
    //在选择模式下 渲染图元
    RenderSphere();
    //返回点击记录数。
    hits = glRenderMode(GL_RENDER);
    ProcessHit(hits, selectBuffer);
    glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
}

void MouseCallBack(int key, int state, int x, int y)
{
  if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    ProcessSelection(x, y);
  }
}

int main(int args, char* argv[])
{
  glutInit(&args, argv);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GL_DEPTH);
  glutCreateWindow("selection");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutTimerFunc(50, TimerFunc, 1);
  glutMouseFunc(MouseCallBack);
  glutMainLoop();
  return 0;
}