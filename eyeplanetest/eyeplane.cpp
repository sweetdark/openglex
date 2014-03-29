// shadowmap.cpp
// OpenGL SuperBible, Chapter 14
// Demonstrates shadow mapping
// Program by Benjamin Lipchak

#include "gltools.h"   // OpenGL toolkit
#include "math3d.h"
#include <stdio.h>

GLboolean ambientShadowAvailable = GL_FALSE;
GLboolean npotTexturesAvailable = GL_FALSE;
GLboolean controlCamera = GL_TRUE;      // xyz keys will control lightpos
GLboolean noShadows = GL_FALSE;         // normal lighting
GLboolean showShadowMap = GL_FALSE;     // show the shadowmap texture

GLfloat factor = 4.0f;                  // for polygon offset

GLint windowWidth = 1024;               // window size
GLint windowHeight = 512;

GLint shadowWidth = 1024;               // set based on window size
GLint shadowHeight = 512;
GLuint shadowTextureID;

GLint maxTexSize;                       // maximum allowed size for 1D/2D texture

GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f};
GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f};
GLfloat noLight[]      = { 0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightPos[]     = { 100.0f, 300.0f, 100.0f, 1.0f};

GLfloat cameraPos[]    = { 100.0f, 150.0f, 200.0f, 1.0f};
GLdouble cameraZoom = 0.3;

M3DMatrix44f textureMatrix;

// Called to draw scene objects
void DrawModels(GLboolean drawBasePlane)
{
  if (drawBasePlane)
  {
    // Draw plane that the objects rest on
    glColor3f(0.0f, 0.0f, 0.90f); // Blue
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, -25.0f, -100.0f);
    glVertex3f(-100.0f, -25.0f, 100.0f);
    glVertex3f(100.0f,  -25.0f, 100.0f);
    glVertex3f(100.0f,  -25.0f, -100.0f);
    glEnd();
  }

  // Draw red cube
  glColor3f(1.0f, 0.0f, 0.0f);
  glutSolidCube(48.0f);

  // Draw green sphere
  glColor3f(0.0f, 1.0f, 0.0f);
  glPushMatrix();
  glTranslatef(-60.0f, 0.0f, 0.0f);
  glutSolidSphere(25.0f, 50, 50);
  glPopMatrix();

  // Draw yellow cone
  glColor3f(1.0f, 1.0f, 0.0f);
  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  glTranslatef(60.0f, 0.0f, -24.0f);
  glutSolidCone(25.0f, 50.0f, 50, 50);
  glPopMatrix();

  // Draw magenta torus
  glColor3f(1.0f, 0.0f, 1.0f);
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 60.0f);
  glutSolidTorus(8.0f, 16.0f, 50, 50);
  glPopMatrix();

  // Draw cyan octahedron
  glColor3f(0.0f, 1.0f, 1.0f);
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -60.0f);
  glScalef(25.0f, 25.0f, 25.0f);
  glutSolidOctahedron();
  glPopMatrix();
}

// Called to regenerate the shadow map
void RegenerateShadowMap(void)
{
  GLfloat lightToSceneDistance, nearPlane, fieldOfView;
  GLfloat lightModelview[16], lightProjection[16];
  GLfloat sceneBoundingRadius = 95.0f; // based on objects in scene

  // Save the depth precision for where it's useful
  lightToSceneDistance = sqrt(lightPos[0] * lightPos[0] + 
    lightPos[1] * lightPos[1] + 
    lightPos[2] * lightPos[2]);
  nearPlane = lightToSceneDistance - sceneBoundingRadius;
  // Keep the scene filling the depth texture
  fieldOfView = (GLfloat)m3dRadToDeg(2.0f * atan(sceneBoundingRadius / lightToSceneDistance));

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
  //gluPerspective(fieldOfView, 1.0f, nearPlane, nearPlane + (2.0f * sceneBoundingRadius));
  glGetFloatv(GL_PROJECTION_MATRIX, lightProjection);
  // Switch to light's point of view
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(lightPos[0], lightPos[1], lightPos[2], 
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glGetFloatv(GL_MODELVIEW_MATRIX, lightModelview);


  // Restore normal drawing state
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  glColorMask(1, 1, 1, 1);
  glDisable(GL_POLYGON_OFFSET_FILL);

  // Set up texture matrix for shadow map projection,
  // which will be rolled into the eye linear
  // texture coordinate generation plane equations
  M3DMatrix44f tempMatrix;
  m3dLoadIdentity44(tempMatrix);
  m3dTranslateMatrix44(tempMatrix, 0.5f, 0.5f, 0.5f);
  m3dScaleMatrix44(tempMatrix, 0.5f, 0.5f, 0.5f);
  m3dMatrixMultiply44(textureMatrix, tempMatrix, lightProjection);
  m3dMatrixMultiply44(tempMatrix, textureMatrix, lightModelview);
  // transpose to get the s, t, r, and q rows for plane equations
  m3dTransposeMatrix44(textureMatrix, tempMatrix);
}

// Called to draw scene
void RenderScene(void)
{
  // Track camera angle
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //if (windowWidth > windowHeight)
  //{
  //  GLdouble ar = (GLdouble)windowWidth / (GLdouble)windowHeight;
  //  glFrustum(-ar * cameraZoom, ar * cameraZoom, -cameraZoom, cameraZoom, 1.0, 1000.0);
  //}
  //else
  //{
  //  GLdouble ar = (GLdouble)windowHeight / (GLdouble)windowWidth;
  //  glFrustum(-cameraZoom, cameraZoom, -ar * cameraZoom, ar * cameraZoom, 1.0, 1000.0);
  //}

  gluPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glViewport(0, 0, windowWidth, windowHeight);

  // Track light position
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (showShadowMap)
  {
    // Display shadow map for educational purposes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    // Show the shadowMap at its actual size relative to window
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(((GLfloat)shadowWidth/(GLfloat)windowWidth)*2.0f-1.0f, 
      -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(((GLfloat)shadowWidth/(GLfloat)windowWidth)*2.0f-1.0f, 
      ((GLfloat)shadowHeight/(GLfloat)windowHeight)*2.0f-1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, 
      ((GLfloat)shadowHeight/(GLfloat)windowHeight)*2.0f-1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
  }
  else if (noShadows)
  {
    // Set up some simple lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    // Draw objects in the scene including base plane
    DrawModels(GL_TRUE);
  }
  else
  {
 

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    glEnable(GL_TEXTURE_2D);
 
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);
    glTexGenfv(GL_S, GL_EYE_PLANE, &textureMatrix[0]);
    glTexGenfv(GL_T, GL_EYE_PLANE, &textureMatrix[4]);
    glTexGenfv(GL_R, GL_EYE_PLANE, &textureMatrix[8]);
    glTexGenfv(GL_Q, GL_EYE_PLANE, &textureMatrix[12]);

    // Draw objects in the scene, including base plane
    DrawModels(GL_TRUE);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
  }
  // Flush drawing commands
  glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
  fprintf(stdout, "Shadow Mapping Demo\n\n");

  // Make sure required functionality is available!
  if (!GLEE_VERSION_1_4 && !GLEE_ARB_shadow)
  {
    fprintf(stderr, "Neither OpenGL 1.4 nor GL_ARB_shadow"
      " extension is available!\n");
    Sleep(2000);
    exit(0);
  }

  // Check for optional extensions
  if (GLEE_ARB_shadow_ambient)
  {
    ambientShadowAvailable = GL_TRUE;
  }
  else
  {
    fprintf(stderr, "GL_ARB_shadow_ambient extension not available!\n");
    fprintf(stderr, "Extra ambient rendering pass will be required.\n\n");
    Sleep(2000);
  }

  if (GLEE_VERSION_2_0 || GLEE_ARB_texture_non_power_of_two)
  {
    npotTexturesAvailable = GL_TRUE;
  }
  else
  {
    fprintf(stderr, "Neither OpenGL 2.0 nor GL_ARB_texture_non_power_of_two extension\n");
    fprintf(stderr, "is available!  Shadow map will be lower resolution (lower quality).\n\n");
    Sleep(2000);
  }

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);

  fprintf(stdout, "Controls:\n");
  fprintf(stdout, "\tRight-click for menu\n\n");
  fprintf(stdout, "\tx/X\t\tMove +/- in x direction\n");
  fprintf(stdout, "\ty/Y\t\tMove +/- in y direction\n");
  fprintf(stdout, "\tz/Z\t\tMove +/- in z direction\n\n");
  fprintf(stdout, "\tf/F\t\tChange polygon offset factor +/-\n\n");
  fprintf(stdout, "\tq\t\tExit demo\n\n");

  // Black background
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

  // Hidden surface removal
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glPolygonOffset(factor, 0.0f);

  // Set up some lighting state that never changes
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHT0);

  // Set up some texture state that never changes
  glGenTextures(1, &shadowTextureID);
  glBindTexture(GL_TEXTURE_2D, shadowTextureID);

  GLint iWidth, iHeight, iCompnent;
  GLenum eFormat;

  GLbyte *pByte = gltLoadTGA("..\\stone.tga", &iWidth, &iHeight, &iCompnent, &eFormat);
  glTexImage2D(GL_TEXTURE_2D, 0, iCompnent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pByte);

  glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_REPLACE);



  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

  RegenerateShadowMap();
}

void ProcessMenu(int value)
{
  switch(value)
  {
  case 1:
    noShadows = !noShadows;
    showShadowMap = GL_FALSE;
    if (noShadows)
    {
      glutChangeToMenuEntry(1, "Toggle shadows (currently OFF)", 1);
    }
    else
    {
      glutChangeToMenuEntry(1, "Toggle shadows (currently ON)", 1);
    }
    glutChangeToMenuEntry(2, "Toggle show shadowmap (currently OFF)", 2);
    break;

  case 2:
    showShadowMap = !showShadowMap;
    noShadows = GL_FALSE;
    if (showShadowMap)
    {
      glutChangeToMenuEntry(2, "Toggle show shadowmap (currently ON)", 2);
    }
    else
    {
      glutChangeToMenuEntry(2, "Toggle show shadowmap (currently OFF)", 2);
    }
    glutChangeToMenuEntry(1, "Toggle shadows (currently ON)", 1);
    break;

  case 3:
    controlCamera = !controlCamera;
    if (controlCamera)
    {
      glutChangeToMenuEntry(3, "Switch to LIGHT control (currently CAMERA)", 3);
    }
    else
    {
      glutChangeToMenuEntry(3, "Switch to CAMERA control (currently LIGHT)", 3);
    }
    break;

  default:
    break;
  }

  // Refresh the Window
  glutPostRedisplay();
}

void KeyPressFunc(unsigned char key, int x, int y)
{
  static GLfloat backupCamera[3];
  switch (key)
  {
  case 's':
    backupCamera[0] = cameraPos[0];
    backupCamera[1] = cameraPos[1];
    backupCamera[2] = cameraPos[2];
    cameraPos[0] = lightPos[0];
    cameraPos[1] = lightPos[1];
    cameraPos[2] = lightPos[2];
    break;
  case 'b':
    cameraPos[0] = backupCamera[0];
    cameraPos[1] = backupCamera[1];
    cameraPos[2] = backupCamera[2];
    break;
  case 'f':
    factor++;
    glPolygonOffset(factor, 0.0f);
    RegenerateShadowMap();
    break;
  case 'F':
    factor--;
    glPolygonOffset(factor, 0.0f);
    RegenerateShadowMap();
    break;
  case 'x':
    if (controlCamera)
      cameraPos[0] += 5.0f;
    else
      lightPos[0] += 5.0f;
    break;
  case 'X':
    if (controlCamera)
      cameraPos[0] -= 5.0f;
    else
      lightPos[0] -= 5.0f;
    break;
  case 'y':
    if (controlCamera)
      cameraPos[1] += 5.0f;
    else
      lightPos[1] += 5.0f;
    break;
  case 'Y':
    if (controlCamera)
      cameraPos[1] -= 5.0f;
    else
      lightPos[1] -= 5.0f;
    break;
  case 'z':
    if (controlCamera)
      cameraPos[2] += 5.0f;
    else
      lightPos[2] += 5.0f;
    break;
  case 'Z':
    if (controlCamera)
      cameraPos[2] -= 5.0f;
    else
      lightPos[2] -= 5.0f;
    break;
  case 'q':
  case 'Q':
  case 27 : /* ESC */
    exit(0);
  }

  // We don't need to regenerate the shadow map
  // if only the camera angle changes
  if (!controlCamera)
    RegenerateShadowMap();

  // Refresh the Window
  glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_LEFT:
    if (controlCamera)
      cameraPos[0] -= 5.0f;
    else
      lightPos[0] -= 5.0f;
    break;
  case GLUT_KEY_RIGHT:
    if (controlCamera)
      cameraPos[0] += 5.0f;
    else
      lightPos[0] += 5.0f;
    break;
  case GLUT_KEY_UP:
    if (controlCamera)
      cameraPos[2] -= 5.0f;
    else
      lightPos[2] -= 5.0f;
    break;
  case GLUT_KEY_DOWN:
    if (controlCamera)
      cameraPos[2] += 5.0f;
    else
      lightPos[2] += 5.0f;
    break;
  default:
    break;
  }

  // We don't need to regenerate the shadow map
  // if only the camera angle changes
  if (!controlCamera)
    RegenerateShadowMap();

  // Refresh the Window
  glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
  GLint i;

  windowWidth = shadowWidth = w;
  windowHeight = shadowHeight = h;

  if (!npotTexturesAvailable)
  {
    // Find the largest power of two that will fit in window.

    // Try each width until we get one that's too big
    i = 0;
    while ((1 << i) <= shadowWidth)
      i++;
    shadowWidth = (1 << (i-1));

    // Now for height
    i = 0;
    while ((1 << i) <= shadowHeight)
      i++;
    shadowHeight = (1 << (i-1));
  }

  if (shadowWidth > maxTexSize)
  {
    shadowWidth = maxTexSize;
  }
  if (shadowHeight > maxTexSize)
  {
    shadowHeight = maxTexSize;
  }

  RegenerateShadowMap();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("Shadow Mapping Demo");
  glutReshapeFunc(ChangeSize);
  glutKeyboardFunc(KeyPressFunc);
  glutSpecialFunc(SpecialKeys);
  glutDisplayFunc(RenderScene);

  // Create the Menu
  glutCreateMenu(ProcessMenu);
  glutAddMenuEntry("Toggle shadows (currently ON)", 1);
  glutAddMenuEntry("Toggle show shadowmap (currently OFF)", 2);
  glutAddMenuEntry("Switch to LIGHT control (currently CAMERA)", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  SetupRC();

  glutMainLoop();
  return 0;
}
