#include <gl\glut.h>
#include <gl\GL.h>
#include <math.h>

#define MAIN_FUNC(WINDOW_CAPTION) \
int main(int args, char *argv[]) \
{																\
	glutInit(&args, argv);			\
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);			\
	glutInitWindowSize(200, 200);			\
	glutCreateWindow(WINDOW_CAPTION);			\
  SetupRC();			\
	glutDisplayFunc(RenderScene);				\
	glutReshapeFunc(ChangeSize);			\
	glutMainLoop();			\
	return 0; 				\
}				\

static GLfloat GL_PI = 3.14159f;


