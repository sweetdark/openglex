#include "gltools.h"
#include "math3d.h"
#include "glframe.h"

//纹理文件目录
static const char *szCubeFile[] = {"..\\pos_x.tga", "..\\neg_x.tga","..\\pos_y.tga", "..\\neg_y.tga","..\\pos_z.tga", "..\\neg_z.tga"};

//立方体纹理
static GLenum cube[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                 GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                 GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                 GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                 GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

static GLfloat fExtent = 10.0f;

static GLFrame camra;

//画立方体盒子
void DrawSkyBox()
{
  glPushMatrix();
    //手动指定纹理坐标
    glBegin(GL_QUADS);
      //-x
      glTexCoord3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-fExtent, fExtent, -fExtent);
      glTexCoord3f(-1.0f, 1.0f, 1.0f);
      glVertex3f(-fExtent, fExtent, fExtent);
      glTexCoord3f(-1.0f, -1.0f, 1.0f);
      glVertex3f(-fExtent, -fExtent, fExtent);
      glTexCoord3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-fExtent, -fExtent, -fExtent);

      //+x
      glTexCoord3f(1.0f, -1.0f, -1.0f);
      glVertex3f(fExtent, -fExtent, -fExtent);
      glTexCoord3f(1.0f, -1.0f, 1.0f);
      glVertex3f(fExtent, -fExtent, fExtent);
      glTexCoord3f(1.0f, 1.0f, 1.0f);
      glVertex3f(fExtent, fExtent, fExtent);
      glTexCoord3f(1.0f, 1.0f, -1.0f);
      glVertex3f(fExtent, fExtent, -fExtent);

      //+y
      glTexCoord3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-fExtent, fExtent, -fExtent);
      glTexCoord3f(-1.0f, 1.0f, 1.0f);
      glVertex3f(-fExtent, fExtent, fExtent);
      glTexCoord3f(1.0f, 1.0f, 1.0f);
      glVertex3f(fExtent, fExtent, fExtent);
      glTexCoord3f(1.0f, 1.0f, -1.0f);
      glVertex3f(fExtent, fExtent, -fExtent);

      //-y
      glTexCoord3f(1.0f, -1.0f, -1.0f);
      glVertex3f(fExtent, -fExtent, -fExtent);
      glTexCoord3f(1.0f, -1.0f, 1.0f);
      glVertex3f(fExtent, -fExtent, fExtent);
      glTexCoord3f(-1.0f, -1.0f, 1.0f);
      glVertex3f(-fExtent, -fExtent, fExtent);
      glTexCoord3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-fExtent, -fExtent, -fExtent);


      //-z
      glTexCoord3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-fExtent, -fExtent, -fExtent);
      glTexCoord3f(1.0f, -1.0f, -1.0f);
      glVertex3f(fExtent, -fExtent, -fExtent);
      glTexCoord3f(1.0f, 1.0f, -1.0f);
      glVertex3f(fExtent, fExtent, -fExtent);
      glTexCoord3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-fExtent, fExtent, -fExtent);

      //+z
      glTexCoord3f(-1.0f, 1.0f, 1.0f);
      glVertex3f(-fExtent, fExtent, fExtent);
      glTexCoord3f(1.0f, 1.0f, 1.0f);
      glVertex3f(fExtent, fExtent, fExtent);
      glTexCoord3f(1.0f, -1.0f, 1.0f);
      glVertex3f(fExtent, -fExtent, fExtent);
      glTexCoord3f(-1.0f, -1.0f, 1.0f);
      glVertex3f(-fExtent, -fExtent, fExtent);
    glEnd();
  glPopMatrix();
}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //隐藏面消除
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;
  
  //设置纹理环境
  glTexEnvi(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_ENV, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  for (int i = 0; i < 6; ++i)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    void *pImage = gltLoadTGA(szCubeFile[i], &iWidth, &iHeight, &iComponents, &eFormat);
    if (pImage)
    {
      glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
      free(pImage);
    }
  }
  //设置自动生成纹理坐标的方式为投影
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

  //先往里移一点
  camra.MoveForward(-5.0f);
  glEnable(GL_TEXTURE_CUBE_MAP);

}


void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  camra.ApplyCameraTransform();

  //天空的纹理坐标手工设置
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  DrawSkyBox();

  //开启纹理坐标自动生成
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);

  //绘制球体
  glPushMatrix();
    //切换到纹理矩阵
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
      M3DMatrix44f m,invert;
      //获取照相机的位置，并进行反转,形成正确的反射纹理
      camra.GetCameraOrientation(m);
      m3dInvertMatrix44(invert,m);
      glMultMatrixf(invert);

      gltDrawSphere(0.75f, 41, 41);
    glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glPopMatrix();
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

  glutPostRedisplay();
}


void SpecialKey(int value, int x, int y)
{
  if (value == GLUT_KEY_LEFT)
  {
    camra.RotateLocalY(-0.1f);
  }

  if (value == GLUT_KEY_RIGHT)
  {
    camra.RotateLocalY(0.1f);
  }

  if (value == GLUT_KEY_UP)
  {
    camra.MoveForward(0.1f);
  }

  if (value == GLUT_KEY_DOWN)
  {
    camra.MoveForward(-0.1f);
  }

  glutPostRedisplay();
}

int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("cubemap");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();

  glutSpecialFunc(SpecialKey);
  glutMainLoop();

  return 0;
}