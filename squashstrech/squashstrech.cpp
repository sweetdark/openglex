#include "gltools.h"
#include "anttweakbar.h"
#include "math3d.h"
#include <stdio.h>
#define MAX_INFO_LEN 256

float objRotate[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightPos[3] = {100, 200, 300};
float cameraPos[3] = {0.0, 0.0, 10.0};
float xTrans = 0.0;
float yTrans = 0.0;
float zTrans = 0.0;

GLuint program;
GLuint vsShader;
GLint lightPosLocation = -1;
GLint squashLocation = -1;
float squash = 1.0f;


void SetupRC()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);
  FILE *file = fopen("../shadersource/squash.vs", "r");
  if (!file)
  {
    return;
  }

  char str[1024] = {0,};
  fread(str, 1024, 1, file);
  fclose(file);
  const GLchar *source[1] = {str};
  glShaderSource(vsShader, 1, &source[0], NULL);
  glCompileShader(vsShader);
  GLint success;
  glGetShaderiv(vsShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LEN] = {0,};
    glGetShaderInfoLog(vsShader, MAX_INFO_LEN, NULL, infoLog);
    printf(infoLog);
    getchar();
    exit(0);
  }

  program = glCreateProgram();
  glAttachShader(program, vsShader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LEN] = {0,};
    glGetProgramInfoLog(program, MAX_INFO_LEN, NULL, infoLog);
    printf(infoLog);
    getchar();
    exit(0);
  }
  
  glValidateProgram(program);
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LEN] = {0,};
    glGetProgramInfoLog(program, MAX_INFO_LEN, NULL, infoLog);
    printf(infoLog);
    getchar();
    exit(0);
  }
  glUseProgram(program);
  lightPosLocation = glGetUniformLocation(program, "lightPos");
  squashLocation = glGetUniformLocation(program, "squashStrech");
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
  glMatrixMode(GL_MODELVIEW);
  if (lightPosLocation != -1)
  {
    glUniform3fv(lightPosLocation, 1, lightPos);
  }
  if (squashLocation != -1)
  {
    glUniform1f(squashLocation, squash);
  }
  glLoadIdentity();
  gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glPushMatrix();
    float mat[4*4];
    ConvertQuaternionToMatrix(objRotate, mat);
    glMultMatrixf(mat);
    glTranslatef(xTrans, yTrans, zTrans);
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
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  TwWindowSize(w, h);

  float aspect = (GLfloat)w/(GLfloat)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, aspect, 1.0, 100.0);

}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("squash strech");

  TwInit(TW_OPENGL, NULL);
  TwBar *bar = TwNewBar("TwBar");
  TwDefine("TwBar size='200 400' color='200 120 200'");
  TwAddVarRW(bar, "objectRotate", TW_TYPE_QUAT4F, objRotate, "opened=true group=object");
  TwAddVarRW(bar, "xTrans", TW_TYPE_FLOAT, &xTrans, "group=object");
  TwAddVarRW(bar, "yTrans", TW_TYPE_FLOAT, &yTrans, "group=object");
  TwAddVarRW(bar, "zTrans", TW_TYPE_FLOAT, &zTrans, "group=object");
  TwAddVarRW(bar, "lightPos", TW_TYPE_DIR3F, lightPos, "opened=true group=light");
  TwAddVarRW(bar, "squash", TW_TYPE_FLOAT, &squash, "label='squash strech' max=10 min=0.1 step=0.1");

  glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
  glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  TwGLUTModifiersFunc(glutGetModifiers);

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();
  if (glDeleteProgram && glDeleteShader)
  {
    glDeleteProgram(program);
    glDeleteShader(vsShader);
  }
  TwTerminate();
  return 0;
}