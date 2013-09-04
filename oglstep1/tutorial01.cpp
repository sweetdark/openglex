#include <gl/glut.h>
#include <gl/GL.h>

static GLfloat x1 = 0.0f;
static GLfloat y1 = 0.0f;

static GLfloat rsize = 25.0f;
static GLfloat xstep = 1.0f;
static GLfloat ystep = 1.0f;

static GLsizei xWindow = 0;
static GLsizei yWindow = 0;

static void RenderScenceCB() {
	
	glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 1.0f, 0.0f);
  glRectf(x1, y1, x1 + rsize, y1 - rsize);
  glutSwapBuffers();
}

static void CreateWindow() {
	//glutInitWindowPosition(200, 200);
	glutInitWindowSize(200, 200);
	glutCreateWindow("opengl step 1");
}

static void TimerFunc(int value)
{
 /* if (x1 > xWindow - rsize || x1 < -xWindow )
  {
    xstep = -xstep;
  }

  if (y1 > yWindow || y1 < -yWindow + rsize)
  {
    ystep = -ystep;
  }

  x1 += xstep;
  y1 += ystep;*/

  //if (x1 > xWindow - rsize + xstep)
  //{
  //  x1 = xWindow - rsize - 1;
  //}
  //else if (x1 < -xWindow)
  //{
  //  x1 = -xWindow + 1;
  //}

  RenderScenceCB();
  glutTimerFunc(33, TimerFunc, 0);
}
static void ChangeSize(GLsizei w, GLsizei h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (h == 0)
  {
    h = 1;
  }

  if (rsize > w)
  {
    rsize = w/2;
  }

  if (rsize > h)
  {
    rsize = h/2;
  }

  xWindow = w/2;
  yWindow = h/2;
  GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
  if (w <= h)
  {
    glOrtho(-100.0f, 100.0f, -100.0f/aspectRatio, 100.0f/aspectRatio, 1.0, -1.0);
  }
  else
  {
    glOrtho(-100.0f*aspectRatio, 100.0f*aspectRatio, -100.0f, 100.0f, 1.0, -1.0);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
int main(int args, char *argc[]) {
	glutInit(&args, argc);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	CreateWindow();
	glutDisplayFunc(RenderScenceCB);
  glutReshapeFunc(ChangeSize);
  glutTimerFunc(33, TimerFunc, 0);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glutMainLoop();

  
	return 0;
}
