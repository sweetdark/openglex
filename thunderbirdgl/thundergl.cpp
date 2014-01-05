// Cover.cpp
// OpenGL SuperBible
// Demonstrates everything but the kitchen sink...
// Program by Richard S. Wright Jr.

#include "gltools.h"	// OpenGL toolkit
#include "glframe.h"   // Camera class
#include "trianglemesh.h"
#include <math.h>

GLFrame    frameCamera;             // The camera

// Thunderbird body
extern short face_indicies[3704][9];
extern GLfloat vertices [1898][3];
extern GLfloat normals [2716][3];
extern GLfloat textures [2925][2];

// Glass cock-pit
extern short face_indiciesGlass[352][9];
extern GLfloat verticesGlass[197][3];
extern GLfloat normalsGlass [227][3];
extern GLfloat texturesGlass [227][2];


CTriangleMesh   thunderBirdBody;
CTriangleMesh   thunderBirdGlass;

// Storeage for two texture objects
GLuint      textureObjects[3];
#define CUBE_MAP        0
#define BODY_TEXTURE    1
#define GLASS_TEXTURE   2

// Six sides of a cube map
const char *szCubeFaces[6] = { "pos_x.tga", "neg_x.tga", "pos_y.tga", "neg_y.tga", "pos_z.tga", "neg_z.tga" };

GLenum  cube[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };


//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
  GLfloat fScale = 0.01f;
  GLbyte *pBytes;
  GLint iWidth, iHeight, iComponents;
  GLenum eFormat;
  int i;

  // Cull backs of polygons
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glGenTextures(2, textureObjects);

  // Set up texture maps   
  // Cube Map
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjects[CUBE_MAP]);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);        

  // Load Cube Map images
  for(i = 0; i < 6; i++)
  {        
    // Load this texture map
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    pBytes = gltLoadTGA(szCubeFaces[i], &iWidth, &iHeight, &iComponents, &eFormat);
    glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);
  }


  // Load the body texture
  glBindTexture(GL_TEXTURE_2D, textureObjects[BODY_TEXTURE]);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  pBytes = gltLoadTGA("BODY.tga", &iWidth, &iHeight, &iComponents, &eFormat);    
  glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void *)pBytes);
  free(pBytes);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  GLfloat fLargest;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);


  glBindTexture(GL_TEXTURE_2D, textureObjects[GLASS_TEXTURE]);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

  pBytes = gltLoadTGA("glass.tga", &iWidth, &iHeight, &iComponents, &eFormat);    
  glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void *)pBytes);
  free(pBytes);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


  /////////////////////////////////////////////////////////////////////
  // Set up the texture units

  // First texture unit contains the color map
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   // Decal tarnish

  // Second texture unit contains the cube map
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjects[CUBE_MAP]);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glEnable(GL_TEXTURE_CUBE_MAP);

  // Multiply this texture by the one underneath
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // Load Thunderbird body and canopy
  // Temporary workspace
  M3DVector3f vVerts[3];
  M3DVector3f vNorms[3];
  M3DVector2f vTex[3];

  // Start assembling the body mesh, set maximum size
  thunderBirdBody.BeginMesh(3704*3);

  // Loop through all the faces
  for(int iFace = 0; iFace < 3704; iFace++)
  {
    // Assemble the triangle
    for(int iPoint = 0; iPoint < 3; iPoint++)
    {
      memcpy(&vVerts[iPoint][0], &vertices[face_indicies[iFace][iPoint]][0], sizeof(M3DVector3f));
      memcpy(&vNorms[iPoint][0], &normals[face_indicies[iFace][iPoint+3]][0], sizeof(M3DVector3f)); 
      memcpy(&vTex[iPoint][0], &textures[face_indicies[iFace][iPoint+6]][0], sizeof(M3DVector2f));
    }

    thunderBirdBody.AddTriangle(vVerts, vNorms, vTex);
  }

  // Close the mesh and scale it (it's a little BIG in it's original format)
  thunderBirdBody.EndMesh();
  thunderBirdBody.Scale(fScale);

  // Now do the same for the canopy
  thunderBirdGlass.BeginMesh(352*3);

  for(int iFace = 0; iFace < 352; iFace++)
  {
    // Assemble the triangle
    for(int iPoint = 0; iPoint < 3; iPoint++)
    {
      memcpy(&vVerts[iPoint][0], &verticesGlass[face_indiciesGlass[iFace][iPoint]][0], sizeof(M3DVector3f));
      memcpy(&vNorms[iPoint][0], &normalsGlass[face_indiciesGlass[iFace][iPoint+3]][0], sizeof(M3DVector3f)); 
      memcpy(&vTex[iPoint][0], &texturesGlass[face_indiciesGlass[iFace][iPoint+6]][0], sizeof(M3DVector2f));
    }

    thunderBirdGlass.AddTriangle(vVerts, vNorms, vTex);
  }

  thunderBirdGlass.EndMesh();
  thunderBirdGlass.Scale(fScale);

  GLfloat fAmbLight[] =   { 0.075f, 0.075f, 0.075f, 0.0f };
  GLfloat fDiffLight[] =  { 1.0f, 1.0f, 1.0f, 0.0f };
  GLfloat fSpecLight[] = { 0.25f, 0.25f, 0.25f, 0.0f };
  GLfloat lightPos[] = { -50.0f, 100.0f, 100.0f, 1.0f };

  // Set up lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, fDiffLight);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecLight);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fAmbLight);
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

  // Light never changes, put it here
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  frameCamera.MoveUp(20.0f);
}


///////////////////////////////////////////////////////////
// Cleanup
void ShutdownRC(void)
{
  glDeleteTextures(2, textureObjects);
}


///////////////////////////////////////////////////////////
// Draw the skybox. This is just six quads, with texture
// coordinates set to the corners of the cube map
void DrawSkyBox(void)
{
  GLfloat fExtent = 50.0f;

  glBegin(GL_QUADS);
  //////////////////////////////////////////////
  // Negative X
  // Note, we must now use the multi-texture version of glTexCoord
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
  glVertex3f(-fExtent, -fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
  glVertex3f(-fExtent, -fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
  glVertex3f(-fExtent, fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
  glVertex3f(-fExtent, fExtent, fExtent);


  ///////////////////////////////////////////////
  //  Postive X
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
  glVertex3f(fExtent, -fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
  glVertex3f(fExtent, -fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
  glVertex3f(fExtent, fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
  glVertex3f(fExtent, fExtent, -fExtent);


  ////////////////////////////////////////////////
  // Negative Z 
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
  glVertex3f(-fExtent, -fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
  glVertex3f(fExtent, -fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
  glVertex3f(fExtent, fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
  glVertex3f(-fExtent, fExtent, -fExtent);


  ////////////////////////////////////////////////
  // Positive Z 
  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
  glVertex3f(fExtent, -fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
  glVertex3f(-fExtent, -fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
  glVertex3f(-fExtent, fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
  glVertex3f(fExtent, fExtent, fExtent);


  //////////////////////////////////////////////////
  // Positive Y
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, 1.0f);
  glVertex3f(-fExtent, fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, 1.0f, -1.0f);
  glVertex3f(-fExtent, fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, -1.0f);
  glVertex3f(fExtent, fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
  glVertex3f(fExtent, fExtent, fExtent);


  ///////////////////////////////////////////////////
  // Negative Y
  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, -1.0f);
  glVertex3f(-fExtent, -fExtent, -fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, -1.0f, -1.0f, 1.0f);
  glVertex3f(-fExtent, -fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, 1.0f);
  glVertex3f(fExtent, -fExtent, fExtent);

  glMultiTexCoord3f(GL_TEXTURE1, 1.0f, -1.0f, -1.0f);
  glVertex3f(fExtent, -fExtent, -fExtent);
  glEnd();
}


/////////////////////////////////////////////////////////////////////////////
// Draw the thunderBirds
void DrawThunderBird(void)
{
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE0);

  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, textureObjects[BODY_TEXTURE]);
  thunderBirdBody.Draw();
  glPopMatrix();

  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
  glBindTexture(GL_TEXTURE_2D, textureObjects[GLASS_TEXTURE]);

  glTranslatef(0.0f, 0.132f, 0.555f);

  glFrontFace(GL_CW);
  thunderBirdGlass.Draw();
  glFrontFace(GL_CCW);
  thunderBirdGlass.Draw();
  glDisable(GL_BLEND);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


// Called to draw scene
void RenderScene(void)
{
  // Clear the window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix(); 
  frameCamera.ApplyCameraTransform(); // Move the camera about

  // Sky Box is manually textured
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);

  glEnable(GL_TEXTURE_CUBE_MAP);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);     
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  DrawSkyBox();

  // Use texgen to apply cube map
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glDisable(GL_TEXTURE_CUBE_MAP);

  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);


  glActiveTexture(GL_TEXTURE1);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();

  // Invert camera matrix (rotation only) and apply to 
  // texture coordinates
  M3DMatrix44f m, invert;
  frameCamera.GetCameraOrientation(m);
  m3dInvertMatrix44(invert, m);
  glMultMatrixf(invert);
  glActiveTexture(GL_TEXTURE0);
  glMatrixMode(GL_MODELVIEW);

  // This displays a rotating ThunderBird Model
  static GLfloat yRot = 0.0f;
  yRot += 0.1f;
  glTranslatef(0.0f, 19.6f, -3.0f);
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  DrawThunderBird();

  // This shows four ThunderBirds flying in formation
  // Coment out the above section, and uncomment
  // the following section to see the four ThunderBirds
  // in formation.
  /*        
  glPushMatrix();
  glTranslatef(0.0f, 21.5f, -7.0f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
  DrawThunderBird();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.85f, 20.75f, -6.5f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
  DrawThunderBird();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-1.0f, 19.75f, -7.0f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
  DrawThunderBird();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.15f, 19.0f, -6.5f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
  DrawThunderBird();
  glPopMatrix();
  */

  glMatrixMode(GL_TEXTURE);
  glActiveTexture(GL_TEXTURE1);
  glPopMatrix();
  glActiveTexture(GL_TEXTURE0);
  glMatrixMode(GL_MODELVIEW);



  glPopMatrix();

  // Do the buffer Swap
  glutSwapBuffers();
}



// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
{
  if(key == GLUT_KEY_UP)
    frameCamera.MoveForward(0.1f);

  if(key == GLUT_KEY_DOWN)
    frameCamera.MoveForward(-0.1f);

  if(key == GLUT_KEY_LEFT)
    frameCamera.RotateLocalY(0.1);

  if(key == GLUT_KEY_RIGHT)
    frameCamera.RotateLocalY(-0.1);

  if(key == GLUT_KEY_PAGE_UP)
    frameCamera.RotateLocalX(0.1);

  if(key == GLUT_KEY_PAGE_DOWN)
    frameCamera.RotateLocalX(-0.1);

  // Refresh the Window
  glutPostRedisplay();
}


///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{
  // Redraw the scene with new coordinates
  glutPostRedisplay();
  glutTimerFunc(3,TimerFunction, 1);
}

void ChangeSize(int w, int h)
{
  GLfloat fAspect;

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  fAspect = (GLfloat)w / (GLfloat)h;

  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the clipping volume
  gluPerspective(35.0f, fAspect, 1.0f, 1000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();    
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(800,600);
  glutCreateWindow("OpenGL ThunderBird");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  glutSpecialFunc(SpecialKeys);

  SetupRC();
  glutTimerFunc(33, TimerFunction, 1);

  glutMainLoop();

  ShutdownRC();

  return 0;
}
