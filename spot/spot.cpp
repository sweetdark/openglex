#include "gltools.h"

GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};
//光源位置
GLfloat lightPos[] = {0.0f, 0.0f, 70.0f, 1.0f};
//聚光灯光照方向朝z轴负方向
GLfloat spotDir[] = {0.0f, 0.0f, -1.0f};
//用于旋转
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

#define FLAT_MODE 1
#define SMOOTH_MODE 2
#define LOW_LEVEL 3
#define MEDIUM_LEVEL 4
#define HIGH_LEVEL 5

int iShade = FLAT_MODE;
int iLevel = LOW_LEVEL;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //开启深度测试
  glEnable(GL_DEPTH_TEST);
  //剔除掉不必要的背面
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  //开启光照
  glEnable(GL_LIGHTING);
  //设置light0光照参数
  glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambientLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  //设置光源位置和方向
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0f);
  //设置全局环境光
  glLightModelfv(GL_AMBIENT, ambientLight);
  //开启light0
  glEnable(GL_LIGHT0);
  
  //开启和设置颜色追踪
  glEnable(GL_COLOR_MATERIAL);
  //设置多边形正面的材料的环境光和漫反射光属性为颜色追踪
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  //设置镜面光的反射属性
  glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  glEnable(GL_NORMALIZE);
}

void RenderScene()
{
  if (iShade == FLAT_MODE)
  {
    glShadeModel(GL_FLAT);
  }
  else
  {
    glShadeModel(GL_SMOOTH);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //矩阵压栈
  glPushMatrix();

    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    //旋转后重新设置光源位置
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    glColor3ub(255, 0, 0);
    glutSolidCone(4.0, 6.0, 15, 15);

    glPushAttrib(GL_LIGHTING_BIT);
      //关闭光照画小黄球
      glDisable(GL_LIGHTING);
      glColor3ub(255, 255, 0);
      glutSolidSphere(3.0, 15, 15);
    glPopAttrib();

  //矩阵出栈
  glPopMatrix();
  glColor3ub(0, 0, 255);
  if (iLevel == LOW_LEVEL)
  {
    glutSolidSphere(30.0f, 8, 8);
  }
  else if (iLevel == MEDIUM_LEVEL)
  {
    glutSolidSphere(30.0f, 10, 10);
  }
  else
  {
    glutSolidSphere(30.0f, 15, 15);
  }

  glutSwapBuffers();
}

void ChangeSize(int w, int h)
{
  if (h == 0)
  {
    h = 1;
  }
  //设置视口
  glViewport(0, 0, w, h);
  GLfloat faspect = (GLfloat)w/(GLfloat)h;

  //透视投影变换
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(35.0, faspect, 1.0, 350.0);

  //模型视图矩阵
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //先往显示器里平移
  glTranslatef(0.0f, 0.0f, -250.0f);

  glutPostRedisplay();
}

void SpecialKeys(int keys, int x, int y)
{
  //改变旋转角度
  if (keys == GLUT_KEY_UP)
  {
    xRot -= 5.0f;
  }
  if (keys == GLUT_KEY_DOWN)
  {
    xRot += 5.0f;
  }

  if (keys == GLUT_KEY_LEFT)
  {
    yRot += 5.0f;
  }

  if (keys == GLUT_KEY_RIGHT)
  {
    yRot -= 5.0f;
  }

  if (xRot > 356.0f)
  {
    xRot = 0.0f;
  }
  else if (xRot < -1.0f)
  {
    xRot = 355.0f;
  }

  if (yRot > 356.0f)
  {
    yRot = 0.0f;
  }
  else if (yRot < -1.0f)
  {
    yRot = 355.0f;
  }

  glutPostRedisplay();
}

void ProcessMenu(int key)
{
  switch(key)
  {
  case 1:
    iShade = FLAT_MODE;
    break;
  case 2:
    iShade = SMOOTH_MODE;
    break;
  case 3:
    iLevel = LOW_LEVEL;
    break;
  case 4:
    iLevel = MEDIUM_LEVEL;
    break;
  case 5:
    iLevel = HIGH_LEVEL;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}
int main(int args, char *arv[])
{
  glutInit(&args, arv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("spot light");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKeys);
  //创建菜单
  int menuID = glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("Flag Mode", 1);
  glutAddMenuEntry("SMOOTH Mode", 2);
  glutAddMenuEntry("Low Level", 3);
  glutAddMenuEntry("Midum Level", 4);
  glutAddMenuEntry("High Level", 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  SetupRC();
  glutMainLoop();
  return 0;
}