#include <gl/glut.h>
#include <gl/GL.h>
#include <math.h>
#define WIDTH 400
#define HEIGHT 400
static GLfloat angle = 0.0f;
static void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0f, 1.0f, 1.0f, 20.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 5.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  {
    GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat sun_light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat sun_light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
  }

  {
    GLfloat sun_mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat sun_mat_diffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat sun_mat_emision[] = {0.5f, 0.0f, 0.0f, 1.0f};
    GLfloat sun_mat_shininess = 0.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emision);
    glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

    glutSolidSphere(2.0, 40, 32);
  }

  // 定义地球的材质并绘制地球
  {
    GLfloat earth_mat_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat earth_mat_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
    GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat earth_mat_shininess  = 10.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT,   earth_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   earth_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  earth_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  earth_mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, earth_mat_shininess);

    glRotatef(angle, 0.0f, -1.0f, 0.0f);
    glTranslatef(5.0f, 0.0f, 0.0f);
    glutSolidSphere(2.0, 40, 32);
  }




  glutSwapBuffers();
}

static void myIdle(void)
{
  angle += 1.0f;
  if (angle >= 360.0f)
  {
    angle = 0.0f;
  }
  myDisplay();
}

//int main(int argc, char *argv[])
//{
//  glutInit(&argc, argv);
//  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//  glutInitWindowPosition(200, 200);
//  glutInitWindowSize(WIDTH, HEIGHT);
//  glutCreateWindow("光照");
//  glutDisplayFunc(&myDisplay);
//  glutIdleFunc(&myIdle);
//  glutMainLoop();
//  return 0;
//}