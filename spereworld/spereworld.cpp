#include "gltools.h"
#include "glFrame.h"
#include <math.h>


#define SPHERE_NUM 20
//球体的角色帧
GLFrame spheres[SPHERE_NUM];
//照相机帧
GLFrame camara;

static void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //设置多边形模式为画线模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  GLfloat x;
  GLfloat z;
  //随机获得球体的位置 在-20 到20之间
  for (unsigned int i = 0; i < SPHERE_NUM; ++i)
  {
    x = (GLfloat)(rand() % 400 - 200) * 0.1f;
    z = (GLfloat)(rand() % 400 - 200) * 0.1f;
    spheres[i].SetOrigin(x, 0.0f, z);
  }

  glEnable(GL_DEPTH_TEST);
}

static void DrawGround()
{
  GLfloat y = -4.0f;
  GLfloat fStep = 2.0f;
  GLfloat fExtent = 40.0f;

  //画网格线
  glBegin(GL_LINES);
    for (GLfloat line = -fExtent; line < fExtent; line += fStep)
    {
      glVertex3f(line, y, -fExtent);
      glVertex3f(line, y, fExtent);

      glVertex3f(-fExtent, y, line);
      glVertex3f(fExtent, y, line);
    }
  glEnd();

}

static void RenderScene()
{
  static GLfloat yRot = 0.0f;
  yRot += 0.5f;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glColor3f(0.0f, 1.0f, 0.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    //照相机变换
    camara.ApplyCameraTransform();
    //画网格线
    DrawGround();
    //画球体
    for (int i = 0; i < SPHERE_NUM; ++i) 
    {
      glPushMatrix();
        //应用角色变换,设置物体的位置
        spheres[i].ApplyActorTransform();
        glutSolidSphere(0.5f, 13, 26);
      glPopMatrix();
    }

    glPushMatrix();
    //往里平移
      glTranslatef(0.0f, 0.0f, -25.0f);
    //旋转
      glRotatef(yRot, 0.0f, 1.0f, 0.0f);
      //画圆环
      gltDrawTorus(0.95, 0.15, 40, 20);
    glPopMatrix();
  glPopMatrix();

  glutSwapBuffers();
}

static void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
  {
    h = 1;
  }

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //透视投影 
  gluPerspective(35.0, (GLdouble)w/h, 1.0, 50.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutPostRedisplay();
  
}

static void TimerFun(int value)
{
  //定时刷新场景
  glutPostRedisplay();
  glutTimerFunc(50, TimerFun, 1);
}

void SpecialKey(int type, int x, int y)
{
  //向上移动照相机
  if (GLUT_KEY_UP == type)
  {
    camara.MoveUp(0.5f);
  }
  //向下移动照相机 
  if (GLUT_KEY_DOWN == type)
  {
    camara.MoveUp(-0.5f);
  }
  //向后移动
  if (GLUT_KEY_LEFT == type)
  {
    camara.MoveForward(-0.5f);
  }
  //向前移动
  if (GLUT_KEY_RIGHT == type)
  {
    camara.MoveForward(0.5f);
  }

  glutPostRedisplay();
}

int main(int args, char** arv)
{
  glutInit(&args, arv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("sphere world");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutTimerFunc(50, TimerFun, 1);
  glutSpecialFunc(SpecialKey);
  SetupRC();
  glutMainLoop();
  return 0;
}