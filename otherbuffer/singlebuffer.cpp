#include <gl/glut.h>
#include <gl/GL.h>
#include <math.h>

static void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

static void RenderScene()
{
    static GLdouble dRadius = 0.1;
    static GLdouble dAngle = 0.0;

    if (dAngle == 0.0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glColor3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_POINTS);
        glVertex2f(dRadius * cos(dAngle), dRadius * sin(dAngle));
    glEnd();
    dRadius *= 1.01;
    dAngle += 0.1;

    if (dAngle > 30.0)
    {
        dRadius = 0.1;
        dAngle = 0.0;
    }
    glFlush();
}
static void ChangeSize(int w, int h)
{
    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);


    // Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set 2D Coordinate system
    gluOrtho2D(-4.0, 4.0, -3.0, 3.0);

    // Modelview matrix reset
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//∂® ±ªÊ÷∆
static void Timer(int value)
{
    glutTimerFunc(50, Timer, 0);
    glutPostRedisplay();
}

//int main(int args, char* argv[])
//{
//    glutInit(&args, argv);
//    glutInitDisplayMode(GLUT_RGB);
//    glutInitWindowSize(200, 200);
//    glutCreateWindow("single buffer");
//    SetupRC();
//    glutDisplayFunc(RenderScene);
//    glutReshapeFunc(ChangeSize);
//    glutTimerFunc(50,Timer, 0);
//    glutMainLoop();
//    return 0;
//}