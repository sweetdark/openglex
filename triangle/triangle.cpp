#include "headers.h"

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static bool bDepth = false;
static bool bCull = false;
static bool bOutLine = false;

static void PopMenu(int key)
{
  if (key == 1)
  {
    bDepth = !bDepth;
  }

  if (key == 2)
  {
    bCull = !bCull;
  }

  if (key == 3)
  {
    bOutLine = !bOutLine;
  }
    
  glutPostRedisplay();
}
static void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat x, y, z, angle;
  bool color;
	glColor3f(1.0f, 1.0f, 0.0f);

  if (bDepth)
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }

  if (bCull)
  {
    glEnable(GL_CULL_FACE);
  }
  else
  {
    glDisable(GL_CULL_FACE);
  }
  
  if (bOutLine)
  {
    glPolygonMode(GL_BACK, GL_LINE) ;
  }
  else
  {
    glPolygonMode(GL_BACK, GL_FILL);
  }
  

  glPushMatrix();
  glRotatef(xRot, 1.0f, 0.0f, 0.0f);
  glRotatef(yRot, 0.0f, 1.0f, 0.0f);

  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 70.0f);

    for (angle = 0.0f; angle <= (2 * GL_PI); angle += (GL_PI / 8))
    {
  		x = 50.0f * sin(angle);
  		y = 50.0f * cos(angle);
  		color = !color;
  	
      glVertex3f(x, y, 0.0f);
  		if(color)
  			glColor3f(1.0f, 0.0f, 0.0f);
  		else
  			glColor3f(0.0f, 1.0f, 1.0f);
    }
  glEnd();

	glBegin(GL_TRIANGLE_FAN);
  	glVertex2f(0.0f, 0.0f);

  	for (angle = 0.0f; angle <= (2 * GL_PI); angle += (GL_PI / 8))
  	{
  		x = 50.0f * sin(angle);
  		y = 50.0f * cos(angle);
  		color = !color;
  	
  		glVertex2f(x, y);

  		if(color)
  			glColor3f(1.0f, 0.0f, 0.0f);
  		else
  			glColor3f(0.0f, 1.0f, 1.0f);
  	}
	glEnd();

  glPopMatrix();
	glutSwapBuffers();
}

static void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CW);
	glShadeModel(GL_FLAT);
  glCullFace(GL_BACK);
}



void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;
	if (h == 0)
	{
		h = 1;
	}
	//设置视口
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//设置可视区域
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		glOrtho(-nRange, nRange, -nRange/aspect, nRange/aspect, -nRange, nRange);
	}
	else
	{
		glOrtho(-nRange*aspect, nRange*aspect, -nRange, nRange, -nRange, nRange);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void SpecialKey(int key, int x, int y)
{
  if (key == GLUT_KEY_UP)
  {
    xRot += 2.0f;
  }
  if (key == GLUT_KEY_DOWN)
  {
    xRot -= 2.0f;
  }
  if (key == GLUT_KEY_LEFT)
  {
    yRot += 2.0f;
  }
  if (key == GLUT_KEY_RIGHT)
  {
    yRot -= 2.0f;
  }

  xRot = (GLint)xRot % 360;
  yRot = (GLint)yRot % 360;
  glutPostRedisplay();
}
int main(int args, char **argv)
{
	glutInit(&args, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(200, 200);
	glutInitWindowSize(200, 200);
	glutCreateWindow("draw points");
  SetupRC();

  glutCreateMenu(PopMenu);
  glutAddMenuEntry("深度缓冲", 1);
  glutAddMenuEntry("裁剪面", 2);
  glutAddMenuEntry("背面画线", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(RenderScene);	
	glutReshapeFunc(ChangeSize);
  glutSpecialFunc(SpecialKey);
	glutMainLoop();
	return 0;
}

