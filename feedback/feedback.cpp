#include "gltools.h"
#include <stdio.h>

static GLfloat yRot = 0.0f;

struct Rect 
{
  GLfloat left;
  GLfloat right;
  GLfloat bottom;
  GLfloat top;

  Rect() : left(-1), right(-1), bottom(-1), top(-1) {}
};

static Rect boundRect;

static GLfloat nChoice;

#define SUN 1
#define EARTH 2
#define MOON 3

#define BUFFER_LENGTH 128
void SetupRC()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);

}

void RenderSphere()
{
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -10.0f);

  glInitNames();
  glPushName(0);
  glColor3f(1.0f, 0.0f, 0.0f);
  glLoadName(SUN);
  glPassThrough((GLfloat)SUN);
  glutSolidSphere(1.0, 26, 26);

  glRotatef(yRot, 0.0f, 1.0f, 0.0f);
  glTranslatef(2.0f, 0.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glPushName(EARTH);
  glPassThrough((GLfloat)EARTH);
  glutSolidSphere(0.3, 26, 26);

  glTranslatef(1.0f, 0.0f, 0.0f);
  glColor3f(0.25f, 0.25f, 0.75f);
  glPushName(MOON);
  glPassThrough((GLfloat)MOON);
  glutSolidSphere(0.1, 26, 26);
  glPopMatrix();
}
void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RenderSphere();
  glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  GLfloat fAspect = (GLfloat)w / (GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void TimerFunc(int value)
{
  yRot += 0.5;
  if (yRot > 360.0f)
  {
    yRot = 0.0f;
  }
  glutPostRedisplay();
  glutTimerFunc(50, TimerFunc, 1);
}

void GetName(GLuint flag, char *str)
{
  if (str == NULL)
  {
    return;
  }

  if (flag == SUN)
  {
    strcat(str, " SUM");
  }

  if (flag == EARTH)
  {
    strcat(str, " EARTH");
  }

  if (flag == MOON)
  {
    strcat(str, " MOON");
  }
}

#define FEEDBACK_SIZE 5600
void ProcessBuffer(GLuint *buf)
{
  char *str = (char*)malloc(30);
  ::memset(str, 0, 30);

  printf("%d", buf[0]);
  GetName(buf[3], str);
  nChoice = (GLfloat)buf[3];
  glutSetWindowTitle(str);

  static GLfloat feedbackBuffer[FEEDBACK_SIZE];

  glFeedbackBuffer(FEEDBACK_SIZE, GL_2D, feedbackBuffer);

  boundRect.right = boundRect. bottom = -99999.0f;
  boundRect.left = boundRect.top = 99999.0f;

  glRenderMode(GL_FEEDBACK);

  RenderSphere();
  int size = glRenderMode(GL_RENDER);
  int i = 0;
  int count = 0;
  while(i < size)
  {
    if (feedbackBuffer[i] == GL_PASS_THROUGH_TOKEN)
    {
      if (feedbackBuffer[i+1] == nChoice)
      {
        i += 2;
        while (i < size && feedbackBuffer[i] != GL_PASS_THROUGH_TOKEN)
        {
          if (feedbackBuffer[i] == GL_POLYGON_TOKEN)
          {
            count = (int)feedbackBuffer[++i];
            ++i;
            for (int j = 0; j < count; ++j)
            {
              GLfloat xCoord = feedbackBuffer[i++];
              GLfloat yCoord = feedbackBuffer[i++];

              if (boundRect.left > xCoord)
                boundRect.left = xCoord;

              if (boundRect.bottom > yCoord)
                boundRect.bottom = yCoord;

              if (boundRect.right < xCoord)
                boundRect.right = xCoord;

              if (boundRect.top < yCoord)
                boundRect.top = yCoord;
            }
          }
        }
      }
    }
  }
}

void ProcessSelection(int x, int y)
{
  GLint viewport[4], hits;

  static GLuint selectBuffer[BUFFER_LENGTH];

  glSelectBuffer(BUFFER_LENGTH, selectBuffer);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glGetIntegerv(GL_VIEWPORT, viewport);
  glRenderMode(GL_SELECT);
  GLfloat aspect = (GLfloat)viewport[2]/(GLfloat)viewport[3];

  gluPickMatrix(x, viewport[3]-y+viewport[1], 2, 2, viewport);

  gluPerspective(35.0, aspect, 1.0, 200.0);

  RenderSphere();

  hits = glRenderMode(GL_RENDER);
  if (hits == 1)
  {
    ProcessBuffer(selectBuffer);
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
}

void MouseCallBack(int key, int state, int x, int y)
{
  if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    ProcessSelection(x, y);
  }
}

int main(int args, char* argv[])
{
  glutInit(&args, argv);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GL_DEPTH);
  glutCreateWindow("selection");
  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutTimerFunc(50, TimerFunc, 1);
  glutMouseFunc(MouseCallBack);
  glutMainLoop();
  return 0;
}