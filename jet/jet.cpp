
#include "gltools.h"	// System and OpenGL Stuff


// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


// Called to draw scene
void RenderScene(void)
{
  // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Save matrix state and do the rotation
  glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    //机头
      glVertex3f(-15.0f, 0.0f, 30.0f);
      glVertex3f(15.0f, 0.0f, 30.0f);
      glVertex3f(0.0f, 0.0f, 60.0f);

      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3f(-15.0f, 0.0f, 30.0f);
      glVertex3f(0.0f, 0.0f, 60.0f);
      glVertex3f(0.0f, 15.0f, 30.0f);

      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 60.0f);
      glVertex3f(15.0f, 0.0f, 30.0f);
      glVertex3f(0.0f, 15.0f, 30.0f);
      //机身
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3f(-15.0f, 0.0f, 30.0f);
      glVertex3f(0.0f, 15.0f, 30.0f);
      glVertex3f(0.0f, 0.0f, -53.0f);

      glColor3f(1.0f, 1.0f, 0.0f);
      glVertex3f(0.0f, 15.0f, 30.0f);
      glVertex3f(15.0f, 0.0f, 30.0f);
      glVertex3f(0.0f, 0.0f, -53.0f);

      glColor3f(0.5f, 0.5f, 0.3f);
      glVertex3f(15.0f, 0.0f, 30.0f);
      glVertex3f(-15.0f, 0.0f, 30.0f);
      glVertex3f(0.0f, 0.0f, -53.0f);

      //机尾巴
      glColor3f(0.3f, 1.0f, 0.2f);
      glVertex3f(0.0f, 0.0f, -53.0f);
      glVertex3f(0.0f, 0.0f, -70.0f);
      glVertex3f(0.0f, 15.0f, -70.0f);

      glColor3f(0.7f, 0.5f, 0.3f);
      glVertex3f(-15.0f, 0.0f, -70.0f);
      glVertex3f(0.0f, 0.0f, -53.0f);
      glVertex3f(0.0f, 0.0f, -70.0f);

      glColor3f(0.2f, 0.2f, 0.8f);
      glVertex3f(15.0f, 0.0f, -70.0f);
      glVertex3f(0.0f, 0.0f, -70.0f);
      glVertex3f(0.0f, 0.0f, -53.0f);

      //由于背面被消除，在背面画一次
      glColor3f(0.3f, 1.0f, 0.2f);
      glVertex3f(0.0f, 0.0f, -70.0f);
      glVertex3f(0.0f, 0.0f, -53.0f);
      glVertex3f(0.0f, 15.0f, -70.0f);

      
      glColor3f(0.7f, 0.5f, 0.3f);
      glVertex3f(0.0f, 0.0f, -53.0f);
      glVertex3f(-15.0f, 0.0f, -70.0f);
      glVertex3f(0.0f, 0.0f, -70.0f);

      glColor3f(0.2f, 0.2f, 0.8f);
      glVertex3f(0.0f, 0.0f, -70.0f);
      glVertex3f(15.0f, 0.0f, -70.0f);
      glVertex3f(0.0f, 0.0f, -53.0f);

      // 左翼 
      glColor3ub(128,128,128);
      glVertex3f(0.0f,2.0f,27.0f);
      glVertex3f(-60.0f, 2.0f, -8.0f);
      glVertex3f(60.0f, 2.0f, -8.0f);

      glColor3ub(64,64,64);
      glVertex3f(60.0f, 2.0f, -8.0f);
      glVertex3f(0.0f, 7.0f, -8.0f);
      glVertex3f(0.0f,2.0f,27.0f);

      glColor3ub(192,192,192);
      glVertex3f(60.0f, 2.0f, -8.0f);
      glVertex3f(-60.0f, 2.0f, -8.0f);
      glVertex3f(0.0f,7.0f,-8.0f);

      //右翼 
      glColor3ub(64,64,64);
      glVertex3f(0.0f,2.0f,27.0f);
      glVertex3f(0.0f, 7.0f, -8.0f);
      glVertex3f(-60.0f, 2.0f, -8.0f);
    glEnd();
  glPopMatrix();

  // Display the results
  glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
  GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glEnable(GL_DEPTH_TEST);	// Hidden surface removal
  glEnable(GL_CULL_FACE);		// Do not calculate inside of jet
  glFrontFace(GL_CCW);		// Counter clock-wise polygons face out

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
  
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  // Nice light blue
  glClearColor(0.0f, 0.0f, 05.f,1.0f);
}

void SpecialKeys(int key, int x, int y)
{
  if(key == GLUT_KEY_UP)
    xRot-= 5.0f;

  if(key == GLUT_KEY_DOWN)
    xRot += 5.0f;

  if(key == GLUT_KEY_LEFT)
    yRot -= 5.0f;

  if(key == GLUT_KEY_RIGHT)
    yRot += 5.0f;

  if(key > 356.0f)
    xRot = 0.0f;

  if(key < -1.0f)
    xRot = 355.0f;

  if(key > 356.0f)
    yRot = 0.0f;

  if(key < -1.0f)
    yRot = 355.0f;

  // Refresh the Window
  glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
  GLfloat nRange = 80.0f;
  // Prevent a divide by zero
  if(h == 0)
    h = 1;

  // Set Viewport to window dimensions
  glViewport(0, 0, w, h);

  // Reset coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Establish clipping volume (left, right, bottom, top, near, far)
  if (w <= h) 
    glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
  else 
    glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800,600);
  glutCreateWindow("Jet");
  glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKeys);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();

  return 0;
}
