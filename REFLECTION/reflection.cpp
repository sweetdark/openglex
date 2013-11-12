#include "gltools.h"
#include "math3d.h"

//光源位置
GLfloat lightPos[4] = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat lightPosMirror[4] = {-100.0f, -100.0f, 50.0f, 1.0f};

GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void DrawGround()
{
  //画地面
  GLfloat fExtent = 20.0f;
  GLfloat fStep = 0.5f;
  GLfloat y = 0.0f;
  GLfloat fColor;
  GLfloat iStrip, iRun;
  GLint iBounce = 0;

  glShadeModel(GL_FLAT);
  for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
  {
    glBegin(GL_TRIANGLE_STRIP);
    for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
    {
      if((iBounce %2) == 0)
        fColor = 1.0f;
      else
        fColor = 0.0f;

      glColor4f(fColor, fColor, fColor, 0.5f);
      glVertex3f(iStrip, y, iRun);
      glVertex3f(iStrip + fStep, y, iRun);

      iBounce++;
    }
    glEnd();
  }
  glShadeModel(GL_SMOOTH);
}

void DrawWorld()
{
  glColor3f(1.0f, 0.0f, 0.0f);
  glPushMatrix();

    glTranslatef(0.0f, 0.5f, -3.5f);
    gltDrawTorus(0.25, 0.08, 68, 37);

  glPopMatrix();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glPushMatrix();
    //设置光源的倒影
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosMirror);

    glPushMatrix();
      //因为是镜像，所以要反过来绘制
      glFrontFace(GL_CW);          
      glScalef(1.0f, -1.0f, 1.0f);
      DrawWorld();
      glFrontFace(GL_CCW);
    glPopMatrix();
    //画地面时，关闭光源, 地面可见并均匀着色。
    glDisable(GL_LIGHTING);
    //打开混合，设置混合因子
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //画地面
    DrawGround();
    //关闭混合效果
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

  glPopMatrix();
  //设置光源在左上角
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  DrawWorld();

  glutSwapBuffers();
}


void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  GLfloat faspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0, faspect, 1.0f, 50.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //往Y轴负方向平移一点
  glTranslatef(0.0f, -0.4f, 0.0f);
  glutPostRedisplay();

}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //开启深度测试，剔除物体背面
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  //逆时针方向绘制的面为正面
  glFrontFace(GL_CCW);
  
  //开启光照，设置光源参数
  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
  glEnable(GL_LIGHT0);

  //开启颜色追踪，设置镜面加亮效果
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Reflection");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();
  return 0;
}