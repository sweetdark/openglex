#include "gltools.h"
#include "math3d.h"
#include "glframe.h"

#define COLORMAP 0
#define CUBEMAP 1
#define TEXTURENUM 2

//纹理对象
GLuint textureObj[TEXTURENUM] = {0,0};

//纹理路径数组
static const char *szCubeFile[] = {"..\\pos_x.tga", "..\\neg_x.tga","..\\pos_y.tga", "..\\neg_y.tga","..\\pos_z.tga", "..\\neg_z.tga"};

//立方体贴图
static GLenum cube[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

static GLfloat fExtent = 10.0f;

static GLFrame camra;

void DrawSkyBox()
{
  glPushMatrix();
  glBegin(GL_QUADS);
  //-x
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
  glVertex3f(-fExtent, fExtent, -fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
  glVertex3f(-fExtent, fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
  glVertex3f(-fExtent, -fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
  glVertex3f(-fExtent, -fExtent, -fExtent);

  //+x
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
  glVertex3f(fExtent, -fExtent, -fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
  glVertex3f(fExtent, -fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
  glVertex3f(fExtent, fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
  glVertex3f(fExtent, fExtent, -fExtent);

  //+y
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
  glVertex3f(-fExtent, fExtent, -fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
  glVertex3f(-fExtent, fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
  glVertex3f(fExtent, fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
  glVertex3f(fExtent, fExtent, -fExtent);

  //-y
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
  glVertex3f(fExtent, -fExtent, -fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
  glVertex3f(fExtent, -fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
  glVertex3f(-fExtent, -fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
  glVertex3f(-fExtent, -fExtent, -fExtent);


  //-z
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
  glVertex3f(-fExtent, -fExtent, -fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
  glVertex3f(fExtent, -fExtent, -fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
  glVertex3f(fExtent, fExtent, -fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
  glVertex3f(-fExtent, fExtent, -fExtent);

  //+z
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
  glVertex3f(-fExtent, fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
  glVertex3f(fExtent, fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
  glVertex3f(fExtent, -fExtent, fExtent);
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
  glVertex3f(-fExtent, -fExtent, fExtent);
  glEnd();
  glPopMatrix();
}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;

  glGenTextures(TEXTURENUM, textureObj);

  //设置立方体纹理对象状态
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureObj[CUBEMAP]);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  for (int i = 0; i < 6; ++i)
  {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    void *pImage = gltLoadTGA(szCubeFile[i], &iWidth, &iHeight, &iComponents, &eFormat);
    if (pImage)
    {
      glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
      free(pImage);
    }
  }

  //设置污点纹理对象状态
  glBindTexture(GL_TEXTURE_2D, textureObj[COLORMAP]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  void *pImage = gltLoadTGA("..\\tarnish.tga", &iWidth, &iHeight, &iComponents, &eFormat);
  if (pImage)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
    free(pImage);
  }

  //激活纹理单元0，并启用2D纹理，设置它的纹理和纹理环境，
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureObj[COLORMAP]);
  glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_DECAL);

  //激活纹理单元1，启用CUBEMAP，并设置它的纹理和纹理环境，纹理生成模式
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_CUBE_MAP);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureObj[CUBEMAP]);
  glTexEnvi(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_ENV, GL_MODULATE);
  //设置自动生成纹理坐标的方式为投影
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

  camra.MoveForward(-5.0f);

}


void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  camra.ApplyCameraTransform();
  //先关闭纹理单元0的2D纹理
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  //选择纹理单元1并启用立方体贴图
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_CUBE_MAP);
  //天空的纹理坐标手工设置
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glTexEnvi(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_ENV, GL_DECAL);

  DrawSkyBox();
  //开启纹理坐标自动生成
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  glTexEnvi(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_ENV, GL_MODULATE);

  //绘制球体，激活纹理0和纹理1进行结合
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  //注意每个纹理单元都有自己的纹理矩阵，这里我们操作纹理1，立方体贴图
  glActiveTexture(GL_TEXTURE1);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  M3DMatrix44f m,invert;
  //获取照相机的位置，并进行反转,形成反射的纹理
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

void ShutdownRC()
{
  glDeleteTextures(TEXTURENUM, textureObj);
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
  glutCreateWindow("mutiltex");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();

  glutSpecialFunc(SpecialKey);
  glutMainLoop();
  ShutdownRC();
  return 0;
}