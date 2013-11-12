#include "gltools.h"
#include "glFrame.h"
#include "math3d.h"
#include <math.h>


#define SPHERE_NUM 20
//球体的角色帧
GLFrame spheres[SPHERE_NUM];
//照相机帧
GLFrame camara;

GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};

GLfloat lightPos[] = {-85.0f, 120.0f, 50.0f};

M3DMatrix44f shadowMat;

static void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  
  //setup light0 and enable light0
  glEnable(GL_LIGHTING);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHT0);

  //open color material
  glEnable(GL_COLOR_MATERIAL);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  GLfloat x;
  GLfloat z;
  //随机获得球体的位置 在-20 到20之间
  for (unsigned int i = 0; i < SPHERE_NUM; ++i)
  {
    x = (GLfloat)(rand() % 400 - 200) * 0.1f;
    z = (GLfloat)(rand() % 400 - 200) * 0.1f;
    spheres[i].SetOrigin(x, 0.0f, z);
  }

  M3DVector3f vPoints[3] = {{ 0.0f, -2.0f, 0.0f },
  { 10.0f, -2.0f, 0.0f },
  { 5.0f, -2.0f, -5.0f }};



  M3DVector4f planeVec;
  m3dGetPlaneEquation(planeVec, vPoints[0], vPoints[1], vPoints[2]); 
  m3dMakePlanarShadowMatrix(shadowMat, planeVec, lightPos);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_NORMALIZE);
}

void DrawGround(void)
{
  GLfloat fExtent = 40.0f;
  GLfloat fStep = 1.0f;
  GLfloat y = -2.0f;
  GLint iStrip, iRun;

  for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
  {
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up

    for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
    {
      glVertex3f(iStrip, y, iRun);
      glVertex3f(iStrip + fStep, y, iRun);
    }
    glEnd();
  }
}


static void DrawScene(int value)
{
  //static GLfloat yRot = 0.0f;
  //yRot += 0.5f;
  if (value)
    glColor3ub(0, 0, 0);
  else
    glColor3ub(0, 255, 0);

  //画球体
  for (int i = 0; i < SPHERE_NUM; ++i) 
  {
    glPushMatrix();
    //应用角色变换,设置物体的位置
    spheres[i].ApplyActorTransform();
    glutSolidSphere(0.5f, 13, 26);
    glPopMatrix();
  }

  //往里平移
  glTranslatef(0.0f, 0.0f, -25.0f);
  //旋转
  //glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  //画圆环
  gltDrawTorus(0.95, 0.15, 40, 20);
}

static void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_LIGHTING);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    //照相机变换
    camara.ApplyCameraTransform();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    //画网格线
    DrawGround();  
    
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMultMatrixf((GLfloat*)shadowMat);
    DrawScene(1);

    glPopMatrix();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    DrawScene(0);

    

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