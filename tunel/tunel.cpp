#include "gltools.h"
#include <stdio.h>

//定义宏常量
#define CEILING 0
#define BRICK 1
#define FLOOR 2
#define TEXTURENUM 3

//纹理图像的路径
const char* texFileName[] = {"..\\ceiling.tga","..\\brick.tga","..\\floor.tga"};

//纹理对象名称
static GLuint textureName[TEXTURENUM];

//旋转与移动
static GLfloat yRot = 0.0f;
static GLfloat zPos = 0.0f;

//切换不同的纹理模式
void ProcessMenu(int value)
{
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
  switch (value)
  {
  case 0:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  	break;
  case 1:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;
  case 2:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    break;
  case 3:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    break;
  case 4:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    break;
  case 5:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    break;
  case 6:
    if (gltIsExtSupported("GL_EXT_texture_filter_anisotropic"))
    {

      //开启各向异性过滤
      GLfloat fLargest;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
      printf("anisotropic:%f\n", fLargest);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
    }
    break;
  default:
    break;
  }

  glutPostRedisplay();
}

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //开启深度测试,消除隐藏面，避免后画的墙画到前面来
  glEnable(GL_DEPTH_TEST);

  //纹理图像的信息
  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;

  //设置纹理环境
  glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_REPLACE);

  //生成纹理对象
  glGenTextures(TEXTURENUM, textureName);

  for (int i = 0; i < TEXTURENUM; ++i)
  {
    void *pImage = gltLoadTGA(texFileName[i], &iWidth, &iHeight, &iComponents, &eFormat);
    
    if (pImage)
    {

      //绑定纹理对象,生成mipmap
      glBindTexture(GL_TEXTURE_2D, textureName[i]);
      gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    free(pImage);
  }

  glEnable(GL_TEXTURE_2D);

}


void ShutdownRC()
{
  //最后删除纹理对象
  glDeleteTextures(TEXTURENUM, textureName);
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

    //移动和旋转
    glTranslatef(0.0f, 0.0f, zPos);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
   
    for(GLfloat z = -60.0f; z <= 0.0f; z += 10.0f)
    {
      //绑定地板纹理绘制地板，注意glBeindTexture在glBegin和glEnd中是无效的
      glBindTexture(GL_TEXTURE_2D, textureName[FLOOR]);
      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10.0f, -10.0f, z);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-10.0f, -10.0f, z + 10.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(10.0f, -10.0f, z + 10.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(10.0f, -10.0f, z);
       
      glEnd();

      //绑定天花板纹理
      glBindTexture(GL_TEXTURE_2D, textureName[CEILING]);
      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10.0f, 10.0f, z);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-10.0f, 10.0f, z + 10.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(10.0f, 10.0f, z + 10.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(10.0f, 10.0f, z);
      glEnd();

      //绑定砖墙的纹理
      glBindTexture(GL_TEXTURE_2D, textureName[BRICK]);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-10.0f, -10.0f, z);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-10.0f, 10.0f, z);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-10.0f, 10.0f, z + 10.0f);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-10.0f, -10.0f, z + 10.0f);

      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(10.0f, -10.0f, z);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(10.0f, 10.0f, z);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(10.0f, 10.0f, z + 10.0f);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(10.0f, -10.0f, z + 10.0f);
      glEnd();
    }

  GLclampf prioritize[TEXTURENUM] = {0.0f, 0.0f, 1.0f};
  glPrioritizeTextures(TEXTURENUM, textureName, prioritize);
  GLboolean isResident[TEXTURENUM];
  if (glAreTexturesResident(TEXTURENUM, textureName, isResident))
  {
    printf("all texture is resident\n");
  }
  else
  {
    printf("texture resident is : %d %d %d", isResident[0], isResident[1], isResident[2]);
  }
  glPopMatrix();
  glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 1)
    h = 0;

  glViewport(0, 0, w, h);

  GLfloat aspect = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.5, aspect, 1.0, 150.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutPostRedisplay();
}

void SpecialKey(int value, int x, int y)
{
  if (value == GLUT_KEY_LEFT)
  {
    yRot += 0.5f;
  }

  if (value == GLUT_KEY_RIGHT)
  {
    yRot -= 0.5f;
  }

  if (value == GLUT_KEY_UP)
  {
    zPos += 0.5f;
  }

  if (value == GLUT_KEY_DOWN)
  {
    zPos -= 0.5f;
  }

  if (yRot > 365.5f)
  {
    yRot = 0.0f;
  }

  glutPostRedisplay();
}

int main(int arg, char **argv)
{
  glutInit(&arg, argv);
  glutInitDisplayMode(GL_RGB | GL_DOUBLE | GL_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("tunel");

  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  glutSpecialFunc(SpecialKey);
  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("GL_NEAREST", 0);
  glutAddMenuEntry("GL_LINEAR", 1);
  glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST", 2);
  glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", 3);
  glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", 4);
  glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", 5);
  glutAddMenuEntry("ANISOTROPIC", 6);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  SetupRC();
  glutMainLoop();
  ShutdownRC();
  return 0;
}