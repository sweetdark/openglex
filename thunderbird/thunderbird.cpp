// ThunderBird.cpp
// OpenGL SuperBible
// Demonstrates rendering a sample model with indexed vertex arrays
// Program by Richard S. Wright Jr.

#include "gltools.h"	// OpenGL toolkit
#include <math.h>
#include "math3d.h"
#include <time.h>
#include <stdio.h>

// Thunderbird body
extern short face_indicies[3704][9];
extern GLfloat vertices [1898][3];
extern GLfloat normals [2716][3];
extern GLfloat textures [2925][2];

// Glass cock-pit
extern short face_indiciesGlass[352][9];
extern GLfloat verticesGlass[197][3];
extern GLfloat normalsGlass [227][3];
extern GLfloat texturesGlass [227][2];


#define BODY_TEXTURE    0
#define GLASS_TEXTURE   1
GLuint  textureObjects[2];

GLuint bodyList, glassList;

static GLfloat fScale = 0.01f;


void DrawBody(void)
{
  glBegin(GL_TRIANGLES);
  for(int iFace = 0; iFace < 3074; iFace++)
  {
    for (int iPoint = 0; iPoint < 3; iPoint++)
    {
      glTexCoord2fv(textures[face_indicies[iFace][iPoint+6]]);
      glNormal3fv(normals[face_indicies[iFace][iPoint+3]]);
      glVertex3fv(vertices[face_indicies[iFace][iPoint]]);
    }
  }
  glEnd();
}

void DrawGlass()
{
  glBegin(GL_TRIANGLES);
  for (int iFace = 0; iFace < 352; iFace++)
  {
    for (int iPoint = 0; iPoint < 3; iPoint++)
    {
      glTexCoord2fv(texturesGlass[face_indiciesGlass[iFace][iPoint+6]]);
      glNormal3fv(normalsGlass[face_indiciesGlass[iFace][iPoint+3]]);
      glVertex3fv(verticesGlass[face_indiciesGlass[iFace][iPoint]]);
    }
  }
  glEnd();
}

void SetupRC()
{
  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;
  GLbyte *pBytes;

  GLfloat fAmbLight[] = {0.1f, 0.1f, 0.1f, 0.0f};
  GLfloat fDiffLight[] = {1.0f, 1.0f, 1.0f, 0.0f};
  GLfloat fSpecLight[] = {0.5f, 0.5f, 0.5f, 0.0f};
  GLfloat fPosition[] = {-100.0f, 100.0f, 100.0f, 1.0f};


  glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  //设置光照
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecLight);
  glLightfv(GL_LIGHT0, GL_POSITION, fPosition);
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

  //设置材料属性，使用颜色追踪
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, fDiffLight);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  //生成纹理对象，设置纹理参数
  glGenTextures(2, textureObjects);
  glBindTexture(GL_TEXTURE_2D, textureObjects[BODY_TEXTURE]);

  pBytes = gltLoadTGA("..\\images\\body.tga", &iWidth, &iHeight, &iComponents, &eFormat);

  glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void*)pBytes);
  free(pBytes);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  //座舱盖
  glBindTexture(GL_TEXTURE_2D, textureObjects[GLASS_TEXTURE]);

  pBytes = gltLoadTGA("..\\images\\glass.tga", &iWidth, &iHeight, &iComponents, &eFormat);

  glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void*)pBytes);
  free(pBytes);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_RESCALE_NORMAL);

  //设置显示列表
  bodyList = glGenLists(2);
  glassList = bodyList + 1;

  glNewList(bodyList, GL_COMPILE);
  DrawBody();
  glEndList();

  glNewList(glassList, GL_COMPILE);
  DrawGlass();
  glEndList();
}

/////////////////////////////////////////////////////////
void ShutdownRC(void)
{
  glDeleteLists(bodyList, 2);
  glDeleteTextures(2, textureObjects);
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  static GLfloat yRot = 0.0f;
  yRot += 0.5f;

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -4.0f);
  glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);

  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textureObjects[BODY_TEXTURE]);
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  //对坐标进行缩放
  glScalef(fScale, fScale, fScale);
  glCallList(bodyList);
  glPopMatrix();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_2D, textureObjects[GLASS_TEXTURE]);
  glTranslatef(0.0f, 0.132f, 0.555f);
  glScalef(fScale, fScale, fScale);

  glFrontFace(GL_CW);
  glCallList(glassList);

  glFrontFace(GL_CCW);
  glCallList(glassList);
  glPopMatrix();

  glDisable(GL_BLEND);

  glutSwapBuffers();

}


////
/////////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{
  // Redraw the scene with new coordinates
  glutPostRedisplay();
  glutTimerFunc(5,TimerFunction, 1);
}


//////////////////////////////////////////////////////////
// Window has changed size
void ChangeSize(int w, int h)
{
  GLfloat fAspect;

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  fAspect = (GLfloat)w / (GLfloat)h;

  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the clipping volume
  gluPerspective(35.0f, fAspect, 1.0f, 1000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800,600);
  glutCreateWindow("F-16 Thunderbird");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);

  SetupRC();
  glutTimerFunc(5, TimerFunction, 1);
  glutMainLoop();

  ShutdownRC();

  return 0;
}
