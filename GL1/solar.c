//#include <gl/glut.h>
//#include <gl/GL.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//
//const GLfloat yearDays = 360.0f;
//const GLdouble factor = 200000000;
//static int day = 200;
//
//double CallFrequency()
//{
//  static int count;
//  static double save;
//  static clock_t last, current;
//  double timegap;
//
//  ++count;
//  if (count <= 50)
//  {
//    return save;
//  }
//  count = 0;
//  last = current;
//  current = clock();
//  timegap = (current - last)/(double)CLK_TCK;
//  save = 50 / timegap;
//  return save;
//}
//
//void myDisplay()
//{
//  double FPS = CallFrequency();
//  printf("FPS is : %lf\n", FPS);
//  glEnable(GL_DEPTH_TEST);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  gluPerspective(100, 1, 1, 400000000);
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);
//
//  // 绘制红色的“太阳”
//  glColor3f(1.0f, 0.0f, 0.0f);
//  glutSolidSphere(69600000, 100, 100);
//
//  glColor3f(0.0f, 0.0f, 1.0f);
//  glRotatef(day/360.0*360.0, 0.0f, 0.0f, -1.0f);
//  glTranslatef(150000000, 0.0f, 0.0f);
//  glutSolidSphere(15945000, 120, 120);
//  // 绘制黄色的“月亮”
//  glColor3f(1.0f, 1.0f, 0.0f);
//  glRotatef(day/30.0*360.0 - day/360.0*360.0, 0.0f, 0.0f, -1.0f);
//  glTranslatef(38000000, 0.0f, 0.0f);
//  glutSolidSphere(4345000, 120, 120);
//
//  glFlush();
//  glutSwapBuffers();
//
//}
//void myIdle()
//{
//  ++day;
//  if (day > 360)
//  {
//    day = 0;
//  }
//  myDisplay();
//}
//
////int main(int args, char *argv[])
////{
////  glutInit(&args, argv);
////  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
////  glutInitWindowPosition(200, 200);
////  glutInitWindowSize(400, 400);
////  glutCreateWindow("solar system");
////  glutDisplayFunc(myDisplay);
////  glutIdleFunc(myIdle);
////  glutMainLoop();
////  return 0;
////}