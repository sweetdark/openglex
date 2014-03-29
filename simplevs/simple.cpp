#include "gltools.h"
#include "math3d.h"
#include "AntTweakBar.h"
#define  MAX_LENGTH 1024

float g_Rotate[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float g_lightPos[4] = {20.0f, 10.0f, 20.0f,1.0f};
#define FIX_FUNCTION 1

char vsChar[] = {
  "#version 120\n"
  "uniform vec3 lightPos;\n"
  "void main(void)"
  "{"
  "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
  "  vec3 N = normalize(gl_NormalMatrix * gl_Normal);"
  "  vec4 V = gl_ModelViewMatrix * gl_Vertex;"
#if FIX_FUNCTION
  " vec3 L = normalize(gl_LightSource[0].position.xyz - V.xyz);"
#else
  "  vec3 L = normalize(lightPos - V.xyz);"
#endif
  "  float NdotL = dot(N, L);"
  "  gl_FrontColor = gl_Color * vec4(max(0.0, NdotL));"
  "}"
};

GLint lightPosLocation1 = -1;

void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  GLint success;
  const GLchar* vsSource[1];
  vsSource[0] = vsChar; //这里的vsChar就是着色器代码字符串
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, vsSource, NULL);
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

  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_LENGTH];
    glGetProgramInfoLog(program, MAX_LENGTH, NULL, infoLog);
    printf(infoLog);
    getchar();
    exit(0);
  }

  glValidateProgram(program);
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[MAX_LENGTH];
    glGetProgramInfoLog(program, MAX_LENGTH, NULL, infoLog);
    printf(infoLog);
    getchar();
    exit(0);
  }
  glUseProgram(program);
#if FIX_FUNCTION
  glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
#else
  lightPosLocation1 = glGetUniformLocation(program, "lightPos");
  if (lightPosLocation1 != -1)
  {
    glUniform3fv(lightPosLocation1, 1, g_lightPos);
  }
#endif

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
#if FIX_FUNCTION
  glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);
#else
  if (lightPosLocation1 != -1)
  {
    //glUniformMatrix4fv(lightPosLocation, 1, GL_FALSE, lightPos);
    glUniform3fv(lightPosLocation1, 1, g_lightPos);
  }
#endif
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0f, 0.0f, -50.0f);
    float mat[4*4];
    ConvertQuaternionToMatrix(g_Rotate, mat);
  
    /*float invmat[4*4];
    m3dInvertMatrix44(invmat, mat);
    if (lightPosLocation != -1)
    {
      glUniformMatrix4fv(lightPosLocation, 1, GL_FALSE, mat);
    }*/
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

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("simple vertex shader");

  TwInit(TW_OPENGL, NULL);
  TwBar *bar = TwNewBar("myBar");
  TwDefine("myBar size='200 400' color = '96 216 224'");

  TwAddVarRW(bar, "ObjectRotate", TW_TYPE_QUAT4F, &g_Rotate, "label='Object Rotation' opened=true");
  TwAddVarRW(bar, "LightDir", TW_TYPE_DIR3F, &g_lightPos, "label='light Pos' opened=true");
  glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
  glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
  glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
  TwGLUTModifiersFunc(glutGetModifiers);
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();
  Terminate();
  return 0;
}