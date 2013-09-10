#include "headers.h"

static void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 1.0f, 0.0f);
  GLfloat x, y, z, angle;
  z = -50.0f;
  //画线
  glBegin(GL_LINES);
  glPushMatrix();  
  for (angle = 0.0f; angle <= 2*GL_PI*3; angle +=0.5f)
  {
    x = 50.0f * cos(angle);
    y = 50.0f * sin(angle);
    //设置两个顶点
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(x, y, z);
    z += 0.5f;
  }

  glEnd();
  glPopMatrix();
  glutSwapBuffers();
}

static void SetupRC()
{
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

static void ChangeSize(GLsizei w, GLsizei h)
{
  if (h == 0)
  {
    h = 1;
  }
  glViewport(0, 0, w, h);
  GLfloat nRange = 100.0f;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat aspect = (GLfloat)w/(GLfloat)h;
  if (w <= h)
  {
    glOrtho(-nRange, nRange, -nRange/aspect, nRange/aspect, -nRange, nRange);
  }
  else
  {
    glOrtho(-nRange * aspect, nRange * aspect, -nRange, nRange, -nRange, nRange);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


//MAIN_FUNC("draw line")