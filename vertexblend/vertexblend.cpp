#include "gltools.h"
#include "anttweakbar.h"
#include "math3d.h"
#include <stdio.h>
#define MAX_INFO_LEN 256

GLfloat elbowBend = 45.0f;
GLfloat sphereOfInfluence = 0.5f;
GLboolean useBlending = true;
float objRotate[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightPos[] = {100.0f, 200.0f, 300.0f, 1.0f};
float cameraPos[] = {50.0f, 75.0f, 50.0f, 1.0f};
float cameraZoom = 0.4f;
float xTrans = 0.0;
float yTrans = 0.0;
float zTrans = 0.0;

GLuint program;
GLuint vsShader;
GLint lightPosLocation, mv2Loc, mv2ITLoc, weightLoc;
GLboolean showBones = false;

GLint maxTexSize;
GLint windowHeight = 600;
GLint windowWidth = 800;

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

void SetupRC()
{
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  glShadeModel(GL_SMOOTH);
  glPointSize(10.0f);
  glLineWidth(5.0f);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
  glBindTexture(GL_TEXTURE_1D, 0);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  CreateTexture(1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_1D);

  vsShader = glCreateShader(GL_VERTEX_SHADER);
  FILE *file = fopen("../shadersource/vertexblend.vs", "r");
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
  mv2Loc = glGetUniformLocation(program, "mv2");
  mv2ITLoc = glGetUniformLocation(program, "mv2IT");
  weightLoc = glGetAttribLocation(program, "weight");
  
}


// Draw a cylinder with supplied dimensions
void DrawCylinder(GLfloat length, GLfloat diameter1, GLfloat diameter2, 
                  GLfloat startWeight, GLfloat endWeight)
{
  int numFacets = 50;
  int numSections = 50;
  GLfloat angleIncr = (2.0f * 3.14159f) / (float)numFacets;
  GLfloat sectionLength = length / numSections;
  GLfloat wEnd, influenceStart;
  int i, j;

  // Determine where our influence starts for this limb
  if (startWeight == 0.5f)
  {
    influenceStart = sphereOfInfluence;
  }
  else
  {
    influenceStart = 1.0f - sphereOfInfluence;
  }

  // Skin
  for (i = 0; i < numFacets; i++)
  {
    // Calculate geometry for this facet
    GLfloat angle1 = i * angleIncr;
    GLfloat angle2 = ((i+1)%numFacets) * angleIncr;
    GLfloat y1AtFirstEnd = sin(angle1) * diameter1;
    GLfloat y1AtOtherEnd = sin(angle1) * diameter2;
    GLfloat z1AtFirstEnd = cos(angle1) * diameter1;
    GLfloat z1AtOtherEnd = cos(angle1) * diameter2;
    GLfloat y2AtFirstEnd = sin(angle2) * diameter1;
    GLfloat y2AtOtherEnd = sin(angle2) * diameter2;
    GLfloat z2AtFirstEnd = cos(angle2) * diameter1;
    GLfloat z2AtOtherEnd = cos(angle2) * diameter2;
    GLfloat n1y = y1AtFirstEnd;
    GLfloat n1z = z1AtFirstEnd;
    GLfloat n2y = y2AtFirstEnd;
    GLfloat n2z = z2AtFirstEnd;

    // One strip per facet
    glBegin(GL_QUAD_STRIP);

    glVertexAttrib1f(weightLoc, useBlending ? startWeight : 1.0f);
    glNormal3f(0.0f, n1y, n1z);
    glVertex3f(0.0f, y1AtFirstEnd, z1AtFirstEnd);
    glNormal3f(0.0f, n2y, n2z);
    glVertex3f(0.0f, y2AtFirstEnd, z2AtFirstEnd);

    for (j = 0; j < numSections; j++)
    {
      // Calculate geometry for end of this quad
      GLfloat paramEnd = (float)(j+1) / (float)numSections;
      GLfloat lengthEnd = paramEnd * length;
      GLfloat y1End = y1AtFirstEnd + (paramEnd * (y1AtOtherEnd - y1AtFirstEnd));
      GLfloat z1End = z1AtFirstEnd + (paramEnd * (z1AtOtherEnd - z1AtFirstEnd));
      GLfloat y2End = y2AtFirstEnd + (paramEnd * (y2AtOtherEnd - y2AtFirstEnd));
      GLfloat z2End = z2AtFirstEnd + (paramEnd * (z2AtOtherEnd - z2AtFirstEnd));

      // Calculate vertex weights
      if (!useBlending)
      {
        wEnd = 1.0f;
      }
      else if (paramEnd <= influenceStart)
      {
        // Map params [0,influenceStart] to weights [0,1]
        GLfloat p = (paramEnd * (1.0f/influenceStart));

        // We're in the first half of the cylinder: startWeight -> 1
        wEnd = startWeight + p * (1.0f - startWeight);
      }
      else
      {
        // Map params [influenceStart,1] to weights [0,1]
        GLfloat p = (paramEnd-influenceStart) * (1.0f/(1.0f-influenceStart));

        // We're in the second half of the cylinder: 1 -> endWeight
        wEnd = 1.0f + p * (endWeight - 1.0f);
      }

      glVertexAttrib1f(weightLoc, wEnd);
      glNormal3f(0.0f, n1y, n1z);
      glVertex3f(lengthEnd, y1End, z1End);
      glNormal3f(0.0f, n2y, n2z);
      glVertex3f(lengthEnd, y2End, z2End);
    }

    // End facet strip
    glEnd();
  }

  if (showBones)
  {
    // Skeleton
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertexAttrib1f(weightLoc, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
  }
}

void DrawModels(void)
{
    M3DVector3f lightPosEye;
    M3DMatrix44f mv, mv2;
    M3DMatrix44f mv2IT_4x4;
    GLfloat mv2IT_3x3[9];
    GLint i;

    // Transform light position to eye space
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    m3dTransformVector3(lightPosEye, lightPos, mv);

    glUniform3fv(lightPosLocation, 1, lightPosEye);

    // Setup modelview matrices for upper arm
    glPushMatrix();
    glRotatef(elbowBend, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, 0.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, mv2);
    glPopMatrix();
    glTranslatef(-50.0f, 0.0f, 0.0f);

    glUniformMatrix4fv(mv2Loc, 1, GL_FALSE, mv2);

    m3dInvertMatrix44(mv2IT_4x4, mv2);

    // Take upper left 3x3 for 2nd normal matrix
    for (i = 0; i < 9; i++)
    {
        mv2IT_3x3[i] = mv2IT_4x4[((i/3)*4)+(i%3)];
    }
    glUniformMatrix3fv(mv2ITLoc, 1, GL_TRUE, mv2IT_3x3);

    // Draw upper arm cylinder
    glColor3f(0.0f, 0.0f, 0.90f);      // Blue
    // 50 long, 10 shoulder, 9 elbow, with weights applied to second half
    DrawCylinder(50.0f, 15.0f, 9.0f, 1.0f, 0.5f);

    // Setup modelview matrices for forearm
    glTranslatef(50.0f, 0.0f, 0.0f);
    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, mv2);
    glPopMatrix();
    glRotatef(elbowBend, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(mv2Loc, 1, GL_FALSE, mv2);

    m3dInvertMatrix44(mv2IT_4x4, mv2);

    // Take upper left 3x3 for 2nd normal matrix
    for (i = 0; i < 9; i++)
    {
        mv2IT_3x3[i] = mv2IT_4x4[((i/3)*4)+(i%3)];
    }
    glUniformMatrix3fv(mv2ITLoc, 1, GL_TRUE, mv2IT_3x3);

    // Draw forearm cylinder
    glColor3f(0.9f, 0.0f, 0.0f);       // Red
    // 40 long, 9 elbow, 5 wrist, with weights applied to first half
    DrawCylinder(40.0f, 9.0f, 5.0f, 0.5f, 1.0f);
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = (float)windowWidth/(float)windowHeight;
  glFrustum(-cameraZoom, cameraZoom, -cameraZoom/aspect, cameraZoom/aspect, 1.0f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glPushMatrix();

  float mat[4*4];
  ConvertQuaternionToMatrix(objRotate, mat);
  glMultMatrixf(mat);
  glTranslatef(xTrans, yTrans, zTrans);
  DrawModels();
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
  windowHeight = h;
  windowWidth = w;
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  TwWindowSize(w, h);
}

void TW_CALL ShowBones(void *data)
{
  showBones = !showBones;
}

void TW_CALL Blend(void *data)
{
  useBlending = !useBlending;
}

int main(int args, char **argv)
{
  glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("vertex blend");

  TwInit(TW_OPENGL, NULL);
  TwBar *bar = TwNewBar("TwBar");
  TwDefine("TwBar size='200 400' color='200 120 200'");
  TwAddVarRW(bar, "objectRotate", TW_TYPE_QUAT4F, objRotate, "opened=true group=object");
  TwAddVarRW(bar, "xTrans", TW_TYPE_FLOAT, &xTrans, "group=object");
  TwAddVarRW(bar, "yTrans", TW_TYPE_FLOAT, &yTrans, "group=object");
  TwAddVarRW(bar, "zTrans", TW_TYPE_FLOAT, &zTrans, "group=object");
  TwAddVarRW(bar, "lightPos", TW_TYPE_DIR3F, lightPos, "opened=true group=light");

  TwBar *bar2 = TwNewBar("Bar2");
  TwDefine("Bar2 size='200 400' color='100 120 100' position='500 20'");
  TwAddVarRW(bar2, "zoom", TW_TYPE_FLOAT, &cameraZoom, "max=1.5 min=0.3 step=0.1");
  TwAddVarRW(bar2, "influence", TW_TYPE_FLOAT, &sphereOfInfluence, "max=1.0 min=0.0 step=0.1");
  TwAddVarRW(bar2, "elbowBend", TW_TYPE_FLOAT, &elbowBend, "max=150 min=-150 step=5.0");
  TwAddVarRW(bar2, "cameraPos", TW_TYPE_DIR3F, &cameraPos, "opened=true");
  TwAddButton(bar2, "showBone", ShowBones, NULL, "label='Bone'");
  TwAddButton(bar2, "useBlend", Blend, NULL, "label='Blend'");
  

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