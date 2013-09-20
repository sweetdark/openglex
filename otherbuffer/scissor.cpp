#include <gl/glut.h>
#include <gl/GL.h>
#include <math.h>

static void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

static void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //…Ë÷√≤√ºÙøÚ
    glScissor(100, 100, 400, 400);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    //∆Ù”√≤√ºÙπ¶ƒ‹
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(200, 200, 200, 200);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDisable(GL_SCISSOR_TEST);
    glFlush();
}

//int main(int args, char* argv[])
//{
//    glutInit(&args, argv);
//    glutInitDisplayMode(GLUT_RGB);
//    glutInitWindowSize(600, 600);
//    glutCreateWindow("scissor");
//    SetupRC();
//    glutDisplayFunc(RenderScene);
//    glutMainLoop();
//    return 0;
//}