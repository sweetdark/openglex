#include "gltools.h"
#include <stdio.h>
#include <stdlib.h>

#define  MAX_INFO_LOG_SIZE 1024
GLint windowWidth = 1024;
GLint windowHeight = 768;

GLfloat flickerFactor = 1.0f;
GLuint vShader, fShader, progObj;

GLfloat cameraPos[] = {100.0f, 150.0f, 200.0f, 1.0f};

GLboolean needsValidation = GL_TRUE;
GLint flickerLocation = -1;
GLboolean isFlicker = false;


void Link(GLboolean firstTime)
{
  GLint success;
  glLinkProgram(progObj);
  glGetProgramiv(progObj, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LOG_SIZE];
    glGetProgramInfoLog(progObj, MAX_INFO_LOG_SIZE, NULL, infoLog);
    printf(infoLog);
    getchar();
    exit(0);
  }
  flickerLocation = glGetUniformLocation(progObj, "flickerFactor");
  if (firstTime)
  {
    glUseProgram(progObj);
  }
}

void Compile()
{
  vShader = glCreateShader(GL_VERTEX_SHADER);
  fShader = glCreateShader(GL_FRAGMENT_SHADER);

  if (vShader == 0 || fShader == 0)
  {
    fprintf(stderr, "shader can not be created\n");
    getchar();
    exit(0);
  }

  const GLchar *vertexStr[1];
  vertexStr[0] = "void main()"
  "\n{"
  "vec4 clipCoord = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
  "gl_Position = clipCoord;\n"
  "gl_FrontColor = gl_Color;\n"
  "vec4 ndc = vec4(clipCoord.xyz, 0)/clipCoord.w;\n"
  "gl_FrontSecondaryColor = (ndc * 0.5) + 0.5;\n"
  "}\n";

  GLint success;

  glShaderSource(vShader, 1, vertexStr, NULL);
  glCompileShader(vShader);
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LOG_SIZE];
    glGetShaderInfoLog(vShader, MAX_INFO_LOG_SIZE, NULL, infoLog);
    fprintf(stderr, infoLog);
    getchar();
    exit(0);
  }

  const GLchar *fragmentStr[1];
  fragmentStr[0] = 
    "uniform float flickerFactor = 1.0f;"
    "void main()"
    "\n{"
    "vec4 temp = mix(gl_Color, vec4(vec3(gl_SecondaryColor),1.0),0.5);\n"
    "gl_FragColor = temp * flickerFactor;"
    "}\n";

  glShaderSource(fShader, 1, fragmentStr, NULL);
  glCompileShader(fShader);
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_INFO_LOG_SIZE];
    glGetShaderInfoLog(fShader, MAX_INFO_LOG_SIZE, NULL, infoLog);
    printf(infoLog);
    getchar();
    exit(0);
  }
  progObj = glCreateProgram();
  glAttachShader(progObj, vShader);
  glAttachShader(progObj, fShader);

  Link(GL_TRUE);
}
void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  Compile();
}
void DrawModel()
{
  glColor3f(0.0f, 1.0f, 0.0f);
  glPushMatrix();
  glBegin(GL_QUADS);
    glVertex3f(-100.0f, -25.0f, -100.0f);
    glVertex3f(-100.0f, -25.0f, 100.0f);
    glVertex3f(100.0f,  -25.0f, 100.0f);
    glVertex3f(100.0f,  -25.0f, -100.0f);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidCube(10.0f);
  glPopMatrix();

  glPopMatrix();
  glColor3f(0.0f, 1.0f, 1.0f);
  glTranslatef(20.0f, 0.0f, 0.0f);
  glutSolidSphere(10.0f, 20, 20);
  glPopMatrix();
}

void RenderScene()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = (GLfloat)windowWidth/(GLfloat)windowHeight;
  gluPerspective(35.0, aspect, 1.0, 1000.0);

  glViewport(0, 0, windowWidth, windowHeight);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0, 0, 0, 0, 1, 0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (isFlicker)
  {
    flickerFactor += ((((GLfloat)rand())/((GLfloat)RAND_MAX)) - 0.5f) * 0.1f;
    if (flickerFactor > 1.0f) flickerFactor = 1.0f;
    if (flickerFactor < 0.0f) flickerFactor = 0.0f;
    glUniform1f(flickerLocation, flickerFactor);
  }

  if (needsValidation)
  {
    GLint success;
    glValidateProgram(progObj);
    glGetProgramiv(progObj, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
      GLchar infoLog[MAX_INFO_LOG_SIZE];
      glGetProgramInfoLog(progObj, MAX_INFO_LOG_SIZE, NULL, infoLog);
      printf(infoLog);
      getchar();
      exit(0);
    }
    needsValidation = GL_FALSE;
  }
  DrawModel();
  glutSwapBuffers();

}

void ReLink()
{
  Link(GL_FALSE);
}

void ChangeSize(int w, int h)
{
  if (h == 0)
  {
    h = 1;
  }
  windowHeight = h;
  windowWidth = w;

}

void SpecialKey(int value, int x, int y)
{
  if (value == GLUT_KEY_UP)
  {
    cameraPos[2] += 2.0f;
  }
  if (value == GLUT_KEY_DOWN)
  {
    cameraPos[2] -= 2.0f;
  }

  glutPostRedisplay();
}

void TimerFunc(int value)
{
  glutTimerFunc(50, TimerFunc, 1);
  glutPostRedisplay();
}

void ProcessMenu(int value)
{
  if (value == 1)
  {
    isFlicker = !isFlicker;
    if (isFlicker)
    {
      glutChangeToMenuEntry(1, "flicker (current on)", 1);
    }
    else
    {
      glutChangeToMenuEntry(1, "flicker (current off)", 1);
    }
  }

  glutPostRedisplay();
}

int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("Shader");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutTimerFunc(50, TimerFunc, 1);
  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("flick (current off)", 1);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutSpecialFunc(SpecialKey);
  glutMainLoop();

  return 0;
}