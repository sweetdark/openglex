#include "gltools.h"
#include "math3d.h"
#include "AntTweakBar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAX_LENGTH 1024

float g_Rotate[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightPos1[4] = {140.0f, 250.0f, 140.0f, 1.0f};
float lightPos2[4] = {20.0f, 150.0f, 100.0f, 1.0f};
float lightPos3[4] = {0.0f, 350.0f, 200.0f, 1.0f};
float cameraPos[3] = {0.0f, 0.0f, 10.0f};
float zTrans = -50.0f;
float yTrans = 0.0f;
float xTrans = 0.0f;

enum Shader_Name
{
  SN_MULTILIGHT,
  SN_MAX
};

enum Program_Name
{
  PN_MULTILIGHT,
  PN_MAX
};

GLchar *shaderSources[SN_MAX];
GLuint program[PN_MAX];
char *fileName[SN_MAX] = {"multilight"};

GLint lightPosLocation1 = -1;
GLint lightPosLocation2 = -1;
GLint lightPosLocation3 = -1;
GLint camaraPosLocation = -1;
GLint maxTexSize;
GLuint textures[3];

void CreateTexture(float r, float g, float b)
{
  GLfloat texels[512 * 4];
  GLint texSize = (maxTexSize > 512) ? 512 : maxTexSize;
  GLint x;

  for (x = 0; x < texSize; x++)
  {
    // Incoming N.H has been scaled by 8 and biased by -7 to take better
    // advantage of the texture space.  Otherwise, the texture will be
    // entirely zeros until ~7/8 of the way into it.  This way, we expand
    // the useful 1/8 of the range and get better precision.
    texels[x*4+0] = r * (float)pow(((double)x / (double)(texSize-1)) * 0.125f + 0.875f, 128.0);
    texels[x*4+1] = g * (float)pow(((double)x / (double)(texSize-1)) * 0.125f + 0.875f, 128.0);
    texels[x*4+2] = b * (float)pow(((double)x / (double)(texSize-1)) * 0.125f + 0.875f, 128.0);
    texels[x*4+3] = 1.0f;
  }
  // Make sure the first texel is exactly zero.  Most
  // incoming texcoords will clamp to this texel.
  texels[0] = texels[1] = texels[2] = 0.0f;

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16, texSize, 0, GL_RGBA, GL_FLOAT, texels);
}

void LoadShaders()
{
  for (int i = 0; i < SN_MAX; ++i)
  {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    char path[128];
    sprintf(path, "..\\shadersource\\%s.vs", fileName[i]);
    FILE *handle = fopen(path, "r");
    if (!handle)
    {
      continue;
    }
    char str[1024] = {0, };
    fread(str, 1024, 1, handle);
    fclose(handle);
    const GLchar *vSource[1] = {str};
    GLint success;
    glShaderSource(vs, 1, vSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      GLchar infoLog[MAX_LENGTH];
      glGetShaderInfoLog(vs, MAX_LENGTH, NULL, infoLog);
      printf(infoLog);
      getchar();
      exit(0);
    }

    program[i] = glCreateProgram();
    glAttachShader(program[i], vs);
    glLinkProgram(program[i]);
    glGetProgramiv(program[i], GL_LINK_STATUS, &success);
    if (!success)
    {
      GLchar infoLog[MAX_LENGTH];
      glGetProgramInfoLog(program[i], MAX_LENGTH, NULL, infoLog);
      printf(infoLog);
      getchar();
      exit(0);
    }

    glUseProgram(program[i]);
    glValidateProgram(program[i]);
    glGetProgramiv(program[i], GL_VALIDATE_STATUS, &success);
    if (!success)
    {
      GLchar infoLog[MAX_LENGTH];
      glGetProgramInfoLog(program[i], MAX_LENGTH, NULL, infoLog);
      printf(infoLog);
      getchar();
      exit(0);
    }
  }
}


void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  LoadShaders();
  glUseProgram(program[0]);

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
  glGenTextures(3, textures);
  glActiveTexture(GL_TEXTURE0);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
  glBindTexture(GL_TEXTURE_1D, textures[0]);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  CreateTexture(1.0f, 0.25f, 0.25f);
  glEnable(GL_TEXTURE_1D);

  glActiveTexture(GL_TEXTURE1);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
  glBindTexture(GL_TEXTURE_1D, textures[1]);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  CreateTexture(0.25, 1.0, 0.25);
  glEnable(GL_TEXTURE_1D);

  glActiveTexture(GL_TEXTURE2);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
  glBindTexture(GL_TEXTURE_1D, textures[2]);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  CreateTexture(0.25, 0.25, 1.0);
  glEnable(GL_TEXTURE_1D);

}

void DrawGround()
{
  glPushMatrix();
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_QUADS);
  glVertex3f(-20.0f, -6.0f, -20.0f);
  glVertex3f(-20.0f, -6.0f, 20.0f);
  glVertex3f(20.0f, -6.0f, 20.0f);
  glVertex3f(20.0f, -6.0f, -20.0f);
  glEnd();
  glPopMatrix();
}

void DrawObjects()
{
  glPushMatrix();
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidCube(3.0);

  glColor3f(0.0f, 0.0f, 1.0f);
  glTranslatef(-10.0f, 0.0f, 0.0f);
  glutSolidTorus(1.0, 3.0, 20, 20);

  glColor3f(1.0f, 1.0f, 0.0f);
  glTranslatef(20.0f, 0.0f, 0.0f);
  glutSolidTeapot(3.0);

  glColor3f(1.0f, 0.0f, 1.0f);
  glTranslatef(-10.0f, 0.0f, -10.0f);
  glutSolidSphere(3.0, 20, 20);

  glColor3f(0.0f, 1.0f, 1.0f);
  glTranslatef(0.0f, 0.0f, 20.0f);
  glScalef(3.0f, 3.0f, 3.0f);
  glutSolidOctahedron();
  glPopMatrix();
}
void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  lightPosLocation1 = glGetUniformLocation(program[0], "lightPos[0]");
  if (lightPosLocation1 != -1)
  {
    glUniform3fv(lightPosLocation1, 1, lightPos1);
  }
  lightPosLocation2 = glGetUniformLocation(program[0], "lightPos[1]");
  if (lightPosLocation2 != -1)
  {
    glUniform3fv(lightPosLocation2, 1, lightPos2);
  }

  lightPosLocation3 = glGetUniformLocation(program[0], "lightPos[2]");
  if (lightPosLocation3 != -1)
  {
    glUniform3fv(lightPosLocation3, 1, lightPos3);
  }

  camaraPosLocation = glGetUniformLocation(program[0], "camaraPos");
  if (camaraPosLocation != -1)
  {
    glUniform3fv(camaraPosLocation, 1, cameraPos);
  }

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glTranslatef(xTrans, yTrans, zTrans);
  float mat[4*4];
  ConvertQuaternionToMatrix(g_Rotate, mat);
  glMultMatrixf(mat);

  DrawGround();
  DrawObjects();
  glPopMatrix();
  TwDraw();
  glutSwapBuffers();
  glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
  if (h == 0)
  {
    h = 1;
  }
  float aspect = (GLfloat)w/(GLfloat)h;

  glViewport(0,0,w,h);
  TwWindowSize(w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(35.0, aspect, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Terminate()
{
  glDeleteTextures(3, textures);
  TwTerminate();
}

void ProcessMenu(int value)
{
  glutPostRedisplay();
}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("specular shader");

  TwInit(TW_OPENGL, NULL);
  TwBar *bar = TwNewBar("myBar");
  TwDefine("myBar size='200 500' color = '96 216 224'");

  TwAddVarRW(bar, "ObjectRotate", TW_TYPE_QUAT4F, &g_Rotate, "label='Object Rotation' opened=true");
  TwAddVarRW(bar, "lightPos1", TW_TYPE_DIR3F, &lightPos1, "label='light Pos1' opened=true group=lights");
  TwAddVarRW(bar, "lightPos2", TW_TYPE_DIR3F, &lightPos2, "label='light Pos2' opened=true group=lights");
  TwAddVarRW(bar, "lightPos3", TW_TYPE_DIR3F, &lightPos3, "label='light Pos3' opened=true group=lights");
  TwAddVarRW(bar, "xTrans", TW_TYPE_FLOAT, &xTrans, "group=translate");
  TwAddVarRW(bar, "yTrans", TW_TYPE_FLOAT, &yTrans, "group=translate");
  TwAddVarRW(bar, "zTrans", TW_TYPE_FLOAT, &zTrans, "group=translate");
  TwAddVarRW(bar, "camara", TW_TYPE_DIR3F, &cameraPos, "label=camarapos opened=true");
  glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
  glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
  glutCreateMenu(ProcessMenu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  for (int i = 0; i < SN_MAX; ++i)
  {
    glutAddMenuEntry(fileName[i], i);
  }
  TwGLUTModifiersFunc(glutGetModifiers);
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();
  Terminate();
  return 0;
}
