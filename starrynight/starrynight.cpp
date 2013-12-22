#include "gltools.h"
#include <math.h>
#include "math3d.h"

//屏幕的宽，高
#define  SCREEN_X 800
#define  SCREEN_Y 600

//大中小星星的数量
#define LARGE_NUM 20
#define MEDIUM_NUM 30
#define SMALL_NUM 40

//星星的坐标
M3DVector2f smallStars[SMALL_NUM];
M3DVector2f mediumStars[MEDIUM_NUM];
M3DVector2f largeStars[LARGE_NUM];

#define TEXNUM 2
#define STAR 0
#define MOON 1
GLuint textureObj[TEXNUM];

void ProcessMenu(int value);

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //设置为2D的正投影,使得坐标从屏幕的左下角开始
  gluOrtho2D(0.0, SCREEN_X, 0.0, SCREEN_Y);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutPostRedisplay();
}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //随机获取星星的位置 
  for (int i = 0; i < SMALL_NUM; ++i)
  {
    smallStars[i][0] = (GLfloat)(rand() % SCREEN_X);
    smallStars[i][1] = (GLfloat)(rand() % SCREEN_Y);
  }
  for (int i = 0; i < MEDIUM_NUM; ++i)
  {
    mediumStars[i][0] = (GLfloat)(rand() % SCREEN_X);
    mediumStars[i][1] = (GLfloat)((rand() % SCREEN_Y) + 50);
  }

  for (int i = 0; i < LARGE_NUM; ++i)
  {
    largeStars[i][0] = (GLfloat)(rand() % SCREEN_X);
    largeStars[i][1] = (GLfloat)(rand() % SCREEN_Y);
  }

  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;

  //生成纹理对象
  glGenTextures(TEXNUM, textureObj);
  //加载纹理图片
  glBindTexture(GL_TEXTURE_2D, textureObj[STAR]);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  void *pImage = gltLoadTGA("..\\images\\star.tga", &iWidth, &iHeight, &iComponents, &eFormat);
  if (pImage)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
    free(pImage);
    pImage = NULL;
  }
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, textureObj[MOON]);

  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  pImage = gltLoadTGA("..\\images\\moon.tga", &iWidth, &iHeight, &iComponents, &eFormat);
  if (pImage)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
    free(pImage);
    pImage = NULL;
  }
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  //启用点精灵
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
  glEnable(GL_POINT_SPRITE);

  ProcessMenu(3);

}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f(1.0f, 1.0f, 1.0f);

  glBindTexture(GL_TEXTURE_2D, textureObj[STAR]);
  glEnable(GL_POINT_SPRITE); 
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  glEnableClientState(GL_VERTEX_ARRAY);
  //画小星星
  glPointSize(7.0);

  glVertexPointer(2, GL_FLOAT, 0, &smallStars[0]);
  glDrawArrays(GL_POINTS, 0, SMALL_NUM);

  //glBegin(GL_POINTS);
  //for (int i = 0; i < SMALL_NUM; ++i)
  //    glVertex2fv(smallStars[i]);
  //glEnd();



  ////画中等大小的星星
  glPointSize(12.0);

  glVertexPointer(2, GL_FLOAT, 0, &mediumStars[0]);
  glDrawArrays(GL_POINTS, 0, MEDIUM_NUM);
  //glBegin(GL_POINTS);
  //for (int i = 0; i < MEDIUM_NUM; ++i)
  //{
  //  glVertex2fv(mediumStars[i]);
  //}
  //glEnd();

  ////大星星
  glPointSize(20.0);

  glVertexPointer(2, GL_FLOAT, 0, &largeStars[0]);
  glDrawArrays(GL_POINTS, 0, LARGE_NUM);
  //glBegin(GL_POINTS);
  //for (int i = 0; i < LARGE_NUM; ++i)
  //{
  //  glVertex2fv(largeStars[i]);
  //}
  //glEnd();
  glDisableClientState(GL_VERTEX_ARRAY);

  glBindTexture(GL_TEXTURE_2D, textureObj[MOON]);
  //画月亮
  glPointSize(120.0f);

  GLfloat x = 650.0f;
  GLfloat y = 400.0f;
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();

  //星座连线
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_POINT_SPRITE);
  glLineWidth(3.0);
  glBegin(GL_LINE_STRIP);
  glVertex2f(0.0f, 50.0f);
  glVertex2f(50.0f, 150.0f);
  glVertex2f(100.0f, 20.0f);
  glVertex2f(300.0f, 300.0f);
  glVertex2f(450.0f, 100.0f);
  glVertex2f(600.0f, 200.0f);
  glVertex2f(800.0f, 30.0f);
  glEnd();
  glutSwapBuffers();
}

void ProcessMenu(int value)
{
  switch (value)
  {
  case 1:
    {
      //开启混合
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glEnable(GL_POINT_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glEnable(GL_POLYGON_SMOOTH);
      glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_POINT_SPRITE);
      break;
    }
  case 2:
    {
      //关闭混合
      glDisable(GL_BLEND);
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_POINT_SPRITE);
      break;
    }
  case 3:     // Point Sprites
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    break;
  default:
    break;
  }

  glutPostRedisplay();
}

void ShutdownRC()
{
  glDeleteTextures(TEXNUM, textureObj);
}
int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(SCREEN_X, SCREEN_Y);
  glutCreateWindow("STARRYNIGHT");

  //右键菜单
  int menuID = glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("antialiasing", 1);
  glutAddMenuEntry("normal", 2);
  glutAddMenuEntry("pointsprites", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();


  return 0;
}