#include "gltools.h"
#include <math.h>

//外围轮廓线
#define COAST_POINTS 24
GLdouble vCoast[COAST_POINTS][3] = {{-70.0, 30.0, 0.0 },
{-50.0, 30.0, 0.0 },
{-50.0, 27.0, 0.0 },
{ -5.0, 27.0, 0.0 },
{  0.0, 20.0, 0.0 },
{  8.0, 10.0, 0.0 },
{ 12.0,  5.0, 0.0 },
{ 10.0,  0.0, 0.0 },
{ 15.0,-10.0, 0.0 },
{ 20.0,-20.0, 0.0 },
{ 20.0,-35.0, 0.0 },
{ 10.0,-40.0, 0.0 },
{  0.0,-30.0, 0.0 },
{ -5.0,-20.0, 0.0 },
{-12.0,-10.0, 0.0 },
{-13.0, -5.0, 0.0 },
{-12.0,  5.0, 0.0 },
{-20.0, 10.0, 0.0 },
{-30.0, 20.0, 0.0 },
{-40.0, 15.0, 0.0 },
{-50.0, 15.0, 0.0 },
{-55.0, 20.0, 0.0 },
{-60.0, 25.0, 0.0 },
{-70.0, 25.0, 0.0 }};

//湖的轮廓线
#define LAKE_POINTS 4
GLdouble vLake[LAKE_POINTS][3] = {{ 10.0, -20.0, 0.0 },
{ 15.0, -25.0, 0.0 },
{ 10.0, -30.0, 0.0 },
{  5.0, -25.0, 0.0 }};

#define LINE_LOOP 1
#define TESS 2
#define COMPLEX 3
static int iMode = LINE_LOOP;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void tessError(GLenum code)
{
  const char *str = (const char*)gluErrorString(code);
  glutSetWindowTitle(str);
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f(1.0f, 0.0f, 1.0f);
  glPushMatrix();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  switch(iMode)
  {
  case LINE_LOOP:
    {
      glBegin(GL_LINE_LOOP);
      for (int i = 0; i < COAST_POINTS; ++i)
      {
        glVertex3dv(vCoast[i]);
      }
      glEnd();
    }
    break;
  case TESS:
    {
      //创建镶嵌器对象
      GLUtesselator *pTess = gluNewTess();
      //设置回调函数
      gluTessCallback(pTess, GLU_TESS_BEGIN, (CallBack)glBegin);
      gluTessCallback(pTess, GLU_TESS_END, (CallBack)glEnd);
      gluTessCallback(pTess, GLU_TESS_VERTEX, (CallBack)glVertex3dv);
      gluTessCallback(pTess, GLU_TESS_ERROR, (CallBack)tessError);

      //开始一个多边形
      gluTessBeginPolygon(pTess, NULL);
      //开始一个轮廓
      gluTessBeginContour(pTess);
      //设置轮廓的顶点
      for (int i = 0; i < COAST_POINTS; ++i)
      {
        gluTessVertex(pTess, vCoast[i], vCoast[i]);
      }
      gluTessEndContour(pTess);
      gluTessEndPolygon(pTess);
      gluDeleteTess(pTess);
    }
    break;
  case COMPLEX:
    {
      GLUtesselator *pTess = gluNewTess();
      gluTessCallback(pTess, GLU_TESS_BEGIN, (CallBack)glBegin);
      gluTessCallback(pTess, GLU_TESS_END, (CallBack)glEnd);
      gluTessCallback(pTess, GLU_TESS_VERTEX, (CallBack)glVertex3dv);
      gluTessCallback(pTess, GLU_TESS_ERROR, (CallBack)tessError);

      //指定奇数的轮廓为填充，偶数的轮廓是镂空的。这也是默认的设置
      gluTessProperty(pTess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

      gluTessBeginPolygon(pTess, NULL);

      gluTessBeginContour(pTess);
      for (int i = 0; i < COAST_POINTS; ++i)
      {
        gluTessVertex(pTess, vCoast[i], vCoast[i]);
      }
      gluTessEndContour(pTess);

      gluTessBeginContour(pTess);
      for (int i = 0; i < LAKE_POINTS; ++i)
      {
        gluTessVertex(pTess, vLake[i], vLake[i]);
      }
      gluTessEndContour(pTess);
      gluTessEndPolygon(pTess);
      gluDeleteTess(pTess);
    }
    break;
  default:
    break;
  }

  glPopMatrix();

  glutSwapBuffers();
}


void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
  {
    h = 1;
  }

  glViewport(0, 0, w, h);

  GLfloat aspect = (GLfloat)w/(GLfloat)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(-100.0, 100.0, -100.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("florida");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("LINE_LOOP", LINE_LOOP);
  glutAddMenuEntry("Tess", TESS);
  glutAddMenuEntry("Complex", COMPLEX);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  SetupRC();
  glutMainLoop();
  return 0;
}