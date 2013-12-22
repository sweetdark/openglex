#include "gltools.h"
#include "math3d.h"
#include "glframe.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

#define SPHERE_NUM 20
#define GROUND_TEXTURE  0
#define TORUS_TEXTURE   1
#define SPHERE_TEXTURE  2
#define NUM_TEXTURES    3
GLuint  textureObjects[NUM_TEXTURES];

const char *szTextureFiles[] = {"..\\grass.tga", "..\\wood.tga", "..\\orb.tga"};

GLfloat fNoLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fDiffuseLight[] = { 0.75f, 0.75f, 0.75f, 1.0f};
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat fLightPos[] = {-100.0f, 100.0f, 50.0f, 1.0f};

GLFrame spheres[SPHERE_NUM];
GLFrame camara;

GLfloat zTran = 0.0f;
GLfloat yRot = 0.0f;

M3DMatrix44f mShadow;

static unsigned int spherelist_1;
static unsigned int spherelist_2;
static unsigned int torulist;
static unsigned int groundlist;


#define NORMALMODE 1
#define LISTMODE 2
static int iMode = NORMALMODE; 

void RenderSpehre(int isShadow);
void RenderGround();

static void SetupRC()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  //Òþ²ØÃæÏû³ý
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
  glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  glStencilOp(GL_INCR, GL_INCR, GL_INCR);
  glClearStencil(0);
  glStencilFunc(GL_EQUAL, 0x0, 0x01);

  glGenTextures(NUM_TEXTURES, textureObjects);

  GLint iWidth, iHeight, iComponents;
  GLenum eFormats;
  for (int i = 0; i < NUM_TEXTURES; ++i)
  {
    glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
    void *pBytes = gltLoadTGA(szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormats);
    //glTexImage2D(GL_TEXTURE_2D, 0, eFormats, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pBytes);
    gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormats, GL_UNSIGNED_BYTE, pBytes);
    glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    free(pBytes);
  }

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_2D);
  for (int i = 0; i < SPHERE_NUM; ++i)
  {
    spheres[i].SetOrigin(((float)((rand() % 400) - 200) * 0.1f), 0.0, (float)((rand() % 400) - 200) * 0.1f);
  }

  M3DVector3f vPoints[3] = {{ 0.0f, -0.4f, 0.0f },
  { 10.0f, -0.4f, 0.0f },
  { 5.0f, -0.4f, -5.0f }};
  M3DVector3f plane;
  m3dGetPlaneEquation(plane, vPoints[0], vPoints[1], vPoints[2]);
  m3dMakePlanarShadowMatrix(mShadow, plane, fLightPos);

  glEnable(GL_MULTISAMPLE);

  spherelist_1 = glGenLists(4);
  spherelist_2 = spherelist_1 + 1;
  torulist = spherelist_1 + 2;
  groundlist = spherelist_1 + 3;

  glNewList(spherelist_1, GL_COMPILE);
    glutSolidSphere(0.3, 20, 20);
  glEndList();

  glNewList(spherelist_2, GL_COMPILE);
    glutSolidSphere(0.1, 20, 20);
  glEndList();

  glNewList(torulist, GL_COMPILE);
    gltDrawTorus(0.25f, 0.15f, 25, 25);
  glEndList();

  glNewList(groundlist, GL_COMPILE);
    RenderGround();
  glEndList();
}

static void RenderSpehre(int isShadow)
{
  if (0 == isShadow)
  {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  }
  else
  {
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
  }

  glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
  for (int i = 0; i < SPHERE_NUM; ++i)
  {
    glPushMatrix();
    spheres[i].ApplyActorTransform();
    if (iMode == LISTMODE)
      glCallList(spherelist_1);
    else
      glutSolidSphere(0.3, 20, 20);

    glPopMatrix();
  }
  glPushMatrix();
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
  if (iMode == LISTMODE)
    glCallList(torulist);
  else
    gltDrawTorus(0.25f, 0.15f, 25, 25);
  glTranslatef(1.0f, 0.0f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
  if (iMode == LISTMODE)
    glCallList(spherelist_2);
  else
    glutSolidSphere(0.1, 20, 20);
  glPopMatrix();
}

static void RenderGround()
{
  GLfloat fExtent = 20.0f;
  GLfloat x, z;
  GLfloat y = -0.4f;
  GLfloat step = 2.0f;
  GLfloat s = 0.0f;
  GLfloat t = 0.0f;
  GLfloat texStep = 1.0f / (fExtent * .075f);

  glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

  for(z = -20.0f; z <= fExtent; z += step)
  {
    glBegin(GL_TRIANGLE_STRIP);
    for(x = -20.0f; x <= fExtent; x += step)
    {
      glTexCoord2f(s, t);
      glNormal3f(0.0f, 1.0f, 0.f);
      glVertex3f(x, y, z);

      glTexCoord2f(s + texStep, t);
      glNormal3f(0.0f, 1.0f, 0.f);
      glVertex3f(x, y, z+step);

      t += texStep;
    }
    glEnd();
    s += texStep;
  }
}

static void TimerFunc(int value)
{
  //glutPostRedisplay();
  //glutTimerFunc(50, TimerFunc, 1);
}

static void RenderScene()
{
  clock_t t = clock();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  yRot += 1.0f;
  if (yRot > 360.5f)
  {
    yRot = 0.0f;
  }
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -2.0f);
  camara.ApplyCameraTransform();  
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  if (iMode == LISTMODE)
  {
    glCallList(groundlist);
  }
  else
  {
    RenderGround();
  }

  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_STENCIL_TEST);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHT0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPushMatrix();
  glMultMatrixf(mShadow);
  RenderSpehre(1);
  glPopMatrix();
  glDisable(GL_BLEND);

  glDisable(GL_STENCIL_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  RenderSpehre(0);
  glPopMatrix();

  glutSwapBuffers();
  t = clock() - t;
  char buffer[128] = {0,};
  wsprintf(buffer, "render clicks is %d", t);
  glutSetWindowTitle(buffer);
}


static void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
    h = 1;

  glViewport(0, 0, w, h);
  GLfloat aspectR = (GLfloat)w/(GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0, aspectR, 1.0, 50.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutPostRedisplay();
}

static void ShutdownRC()
{
  glDeleteLists(spherelist_1, 4);
}

void SpecialKey(int value, int x, int y)
{
  if (value == GLUT_KEY_UP)
  {
    camara.MoveForward(1.0f);
  }

  if (value == GLUT_KEY_DOWN)
  {
    camara.MoveForward(-1.0f);
  }

  if (value == GLUT_KEY_LEFT)
  {
    camara.MoveRight(1.0f);
    zTran += 1.0f;
  }

  if (value == GLUT_KEY_RIGHT)
  {
    camara.MoveRight(-1.0f);
    zTran -= 1.0f;
  }

  glutPostRedisplay();
}


void ProcessMenu(int value)
{
  iMode = value;
}

int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(800, 600);
  glutCreateWindow("sphere world");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKey);
  SetupRC();
  glutTimerFunc(50, TimerFunc, 1);
  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("NORMAL", NORMALMODE);
  glutAddMenuEntry("LIST", LISTMODE);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutMainLoop();
  ShutdownRC();

  return 0;
}