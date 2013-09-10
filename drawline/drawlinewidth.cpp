#include "headers.h"

static void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 1.0f, 0.0f);
  //获取直线的宽度
  GLfloat width[2];
  GLfloat step;
  glGetFloatv(GL_LINE_WIDTH_RANGE, width);
  glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &step);
  GLfloat y, curWidht;
  curWidht = width[0];
  curWidht += step;
  //画线
  for (y = 80.0f; y > -80.0f; y -= 10.0f)
  {
    glLineWidth(curWidht);
    curWidht += step;

    glBegin(GL_LINES);
      glVertex2f(-50.0f, y);
      glVertex2f(50.0f, y);
    glEnd();
  }
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


//MAIN_FUNC("draw line width")