#include "gltools.h"
#define gltPrintf(a, b, c, d)
#include <stdio.h>
#include "glframe.h"

GLFrame camera;

GLint windowWidth = 1024;
GLint windowHeight = 768;

GLint mainMenu, boxMenu;

GLboolean showMenu = GL_TRUE;
GLboolean occlusionDetection = GL_TRUE;
GLboolean showBoundingVolumn = GL_FALSE;
GLint boundingVolumn = 0;
GLuint queryIDs[27];

GLfloat ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat diffuseLgiht[] = {0.9f, 0.9f, 0.9f, 1.0f};
GLfloat lightPos[] = {100.0f, 300.0f, 100.0f, 1.0f};

GLfloat cameraPos[] = {200.0f, 300.0f, 400.0f, 1.0f};

static GLint occlucedCount = 0;

void DrawOccluder()
{
  glColor3f(0.5f, 0.25f, 0.0f);

  glPushMatrix();
  glScalef(30.0f, 30.0f, 1.0f);
  glTranslatef(0.0f, 0.0f, 50.0f);
  glutSolidCube(10.0f);
  glTranslatef(0.0f, 0.0f, -100.0f);
  glutSolidCube(10.0f);
  glPopMatrix();

  glPushMatrix();
  glScalef(30.0f, 1.0f, 30.0f);
  glTranslatef(0.0f, 50.0f, 0.0f);
  glutSolidCube(10.0f);
  glTranslatef(0.0f, -100.0f, 0.0f);
  glutSolidCube(10.0f);
  glPopMatrix();

  glPushMatrix();
  glScalef(1.0f, 30.0f, 30.0f);
  glTranslatef(50.0f, 0.0f, 0.0f);
  glutSolidCube(10.0f);
  glTranslatef(-100.0f, 0.0f, 0.0f);
  glutSolidCube(10.0f);
  glPopMatrix();
}

void DrawSphere(GLint sphereNum)
{
  GLuint param = 1;
  if (occlusionDetection)
  {
    glGetQueryObjectuiv(queryIDs[sphereNum], GL_QUERY_RESULT, &param);
  }
  if (param > 0)
  {
    glutSolidSphere(50.0f, 200, 200);
  }
  else
  {
    occlucedCount++;
  }
}

void DrawModels()
{
  GLint r, g, b;
  occlucedCount = 0;
  DrawOccluder();

  if (occlusionDetection)
  {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    for (r = 0; r < 3; r++)
    {
      for (g = 0; g < 3; g++)
      {
        for (b = 0; b < 3; b++)
        {
          glPushMatrix();
          glTranslatef(100.0f * r - 100.0f,
            100.0f * g - 100.0f,
            100.0f * b - 100.0f);
          glBeginQuery(GL_SAMPLES_PASSED, queryIDs[(r*9)+(g*3)+b]);
          glutSolidCube(100.0f);
          glEndQuery(GL_SAMPLES_PASSED);
          glPopMatrix();
        }
      }
    }
     

    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
  }

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_GEN_S);;
  glEnable(GL_TEXTURE_GEN_T);

  for(r = 0; r < 3; r++)
  {
    for (g = 0; g < 3; g++)
    {
      for (b = 0; b < 3; b++)
      {
        glColor3f(r * 1.0f/3, g * 1.0f/3, b * 1.0f/3);

        glPushMatrix();
        glTranslatef(100.0f * r - 100.0f,
          100.0f * g - 100.0f,
          100.0f * b - 100.0f);
        DrawSphere((r*9)+(g*3)+b);
        glPopMatrix();
      }
    }
  }
  printf("occluced count is %d\n", occlucedCount);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_S);
}


void RenderScene()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLdouble ar = (GLdouble)windowWidth/(GLdouble)windowHeight;
  gluPerspective(45.0, ar, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //试着使用GLFrame的方式替换它
  gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glViewport(0, 0, windowWidth, windowHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera.ApplyCameraTransform(true);
  DrawModels();

  glutSwapBuffers();
}

void SetupRC()
{
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLgiht);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_SPECULAR, diffuseLgiht);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  glEnable(GL_TEXTURE_2D);
  GLint iWidth, iHeight, iComponent;
  GLenum eFormat;
  GLvoid *pByte = gltLoadTGA("logo.tga", &iWidth, &iHeight, &iComponent, &eFormat);
  glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pByte);
  free(pByte);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  GLfloat plane[4] = {1.0f/50.0f, 0.0f, 0.0f, -0.5f};
  glTexGenfv(GL_S, GL_OBJECT_PLANE, plane);
  plane[0] = 0.0f;
  plane[2] = 1.0/50.0f;
  glTexGenfv(GL_T, GL_OBJECT_PLANE, plane);

  glGenQueries(27, queryIDs);
}

void ChangeSize(int w, int h)
{
  windowWidth = w;
  windowHeight = h;
}

void ProcessMenu(int value)
{
  switch(value)
  {
  case 1:
    occlusionDetection = !occlusionDetection;
    if (occlusionDetection)
    {
      glutChangeToMenuEntry(1, "Toogle occlusion culling(currently ON", 1);
    }
    else
    {
      glutChangeToMenuEntry(1, "Toogle occlusion culling(currently OFF", 1);
    }
    break;
  }

  glutPostRedisplay();
}

void SpecialKey(int value, int x, int y)
{
  if (value == GLUT_KEY_UP)
  {
    cameraPos[2] += 5.5f;
  }
  if (value == GLUT_KEY_DOWN)
  {
    cameraPos[2] -= 5.5f;
  }

  if (value == GLUT_KEY_LEFT)
  {
    camera.RotateLocalY(0.5f);
  }
  if (value == GLUT_KEY_RIGHT)
  {
    camera.RotateLocalY(-0.5f);
  }
  glutPostRedisplay();
}

int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Occlusion");

  glutDisplayFunc(RenderScene);
  SetupRC();
  glutReshapeFunc(ChangeSize);

  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("Toogle occlusion culling(currently OFF)", 1);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutSpecialFunc(SpecialKey);
  glutMainLoop();

  glDeleteQueries(27, queryIDs);
  return 0;
}