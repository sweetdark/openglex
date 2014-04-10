#include "gltools.h"
#include "anttweakbar.h"
#include "math3d.h"
#include <stdio.h>
#define  MAX_INFO_LEN 1024
float objectRotate[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightPos[4] = {0.0f, 100.0f, 230.0f};
float cameraPos[3] = {0.0f, 0.0f, 10.0f};
float xTrans;
float yTrans;
float zTrans;

GLuint program;
GLint lightPosLocation = -1;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  const char fileName[] = {"../shadersource/pointsize.vs"};
  FILE *fileHandle = fopen(fileName, "r");
  if (!fileHandle)
  {
    printf("not valid file");
    getchar();
    return;
  }
  char vsStr[1024] = {0,};
  fread(vsStr, 1024, 1, fileHandle);
  printf("vs source\n");
  printf(vsStr);
  fclose(fileHandle);
  const char *vsSource[1] = {vsStr};
  glShaderSource(vs, 1, &vsSource[0], NULL);
  glCompileShader(vs);
  GLint success;
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LEN] = {0,};
    glGetShaderInfoLog(vs, MAX_INFO_LEN, NULL, infoLog);
    printf("compile error\n");
    printf(infoLog);
    getchar();
    exit(0);
  }

  program = glCreateProgram();
  glAttachShader(program, vs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LEN] = {0,};
    glGetProgramInfoLog(program, MAX_INFO_LEN, NULL, infoLog);
    printf("link error\n");
    printf(infoLog);
    getchar();
    exit(0);
  }

  glValidateProgram(program);
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LEN] = {0, };
    glGetProgramInfoLog(program, MAX_INFO_LEN, NULL, infoLog);
    printf("validate error\n");
    printf(infoLog);
    getchar();
    exit(0);
  }
  glUseProgram(program);
  lightPosLocation = glGetUniformLocation(program, "lightPos");
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
  if (lightPosLocation != -1)
  {
    glUniform3fv(lightPosLocation, 1, lightPos);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0, 0, 0, 0, 1, 0);
  glPushMatrix();
    glTranslatef(xTrans, yTrans, zTrans);
    float matrix[4*4];
    ConvertQuaternionToMatrix(objectRotate, matrix);
    glMultMatrixf(matrix);
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
  glViewport(0, 0, w, h);
  TwWindowSize(w, h);
  float aspect = (float)w/(float)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, aspect, 1.0, 200.0);

}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Point Size");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  TwInit(TW_OPENGL, NULL);
  TwBar *bar = TwNewBar("TwBar");
  TwDefine("TwBar size='200 500' color = '96 216 224'");
  TwAddVarRW(bar, "Object Rotate", TW_TYPE_QUAT4F, &objectRotate, "opened=true group='object'");
  TwAddVarRW(bar, "xTrans", TW_TYPE_FLOAT, &xTrans, "label='X Translate' group='object'");
  TwAddVarRW(bar, "yTrans", TW_TYPE_FLOAT, &yTrans, "label='y Translate' group='object'");
  TwAddVarRW(bar, "zTrans", TW_TYPE_FLOAT, &zTrans, "label='z Translate' group='object'");
  TwAddVarRW(bar, "lightPos", TW_TYPE_DIR3F, &lightPos, "label='light pos' opened=true");
  TwAddVarRW(bar, "camaraPos", TW_TYPE_DIR3F, &cameraPos, "label='camara pos' opened=true");
  glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
  glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  TwGLUTModifiersFunc(glutGetModifiers);
  
  glutMainLoop();
  TwTerminate();
  return 0;
}