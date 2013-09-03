#include <gl/glut.h>
#include <gl/GL.h>

static void RenderScenceCB() {
	
	glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 1.0f, 0.0f);
  glRectf(-25.0f, 25.0f, 25.0f, -25.0f);
  glFlush();
}

static void CreateWindow() {
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("opengl step 1");
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	CreateWindow();
	glutDisplayFunc(RenderScenceCB);
  glutReshapeFunc(ChangeSize);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glutMainLoop();

  
	return 0;
}
