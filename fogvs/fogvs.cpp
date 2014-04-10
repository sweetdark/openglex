#include "gltools.h"
#include "math3d.h"
#include "AntTweakBar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAX_LENGTH 1024

float g_Rotate[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightPos1[4] = {140.0f, 250.0f, 140.0f, 1.0f};
float cameraPos[3] = {0.0f, 0.0f, 10.0f};
float zTrans = -50.0f;
float yTrans = 0.0f;
float xTrans = 0.0f;
float density = 0.005f;
float fogColor[4] = {0.5f, 0.8f, 0.5f, 1.0f};

enum Shader_Name
{
  SN_FOGCOORD,
  SN_FOG,
  SN_MAX
};

enum Program_Name
{
  PN_FOGCOORD,
  PN_FOG,
  PN_MAX
};

GLchar *shaderSources[SN_MAX];
GLuint program[PN_MAX];
char *fileName[SN_MAX] = {"FOGCOORD", "FOG"};

GLint lightPosLocation1 = -1;
GLint camaraPosLocation = -1;
GLint densityLocation = -1;

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
  glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_DENSITY, density);
  glFogi(GL_FOG_MODE, GL_EXP2);
  glFogi(GL_FOG_COORD_SRC, GL_FOG_COORD);
  glEnable(GL_FOG);
  LoadShaders();
  glUseProgram(program[0]);

  camaraPosLocation = glGetUniformLocation(program[0], "camaraPos");
  lightPosLocation1 = glGetUniformLocation(program[0], "lightPos[0]");
  densityLocation = glGetUniformLocation(program[0], "density");

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
  if (lightPosLocation1 != -1)
  {
    glUniform3fv(lightPosLocation1, 1, lightPos1);
  }

  if (camaraPosLocation != -1)
  {
    glUniform3fv(camaraPosLocation, 1, cameraPos);
  }

  if (densityLocation != -1)
  {
    glUniform1f(densityLocation, density);
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
  TwTerminate();
}

void ProcessMenu(int value)
{
  glDisable(GL_FOG);
  if (value == 0)
  {
    glEnable(GL_FOG);
  }
  
  camaraPosLocation = glGetUniformLocation(program[value], "camaraPos");
  lightPosLocation1 = glGetUniformLocation(program[value], "lightPos[0]");
  densityLocation = glGetUniformLocation(program[value], "density");
  glutPostRedisplay();
}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("fog shader");

  TwInit(TW_OPENGL, NULL);
  TwBar *bar = TwNewBar("myBar");
  TwDefine("myBar size='200 500' color = '96 216 224'");

  TwAddVarRW(bar, "ObjectRotate", TW_TYPE_QUAT4F, &g_Rotate, "label='Object Rotation' opened=true");
  TwAddVarRW(bar, "density", TW_TYPE_FLOAT, &density, "label=density, max=0.01 min=0.005 step=0.001");
  TwAddVarRW(bar, "lightPos1", TW_TYPE_DIR3F, &lightPos1, "label='light Pos1' opened=true group=lights");
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
