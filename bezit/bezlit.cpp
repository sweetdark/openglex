#include "gltools.h"
#include "math3d.h"

//控制点 
GLint nNumPoints = 3;

GLfloat ctrlPoints[3][3][3]= {{{  -4.0f, 0.0f, 4.0f},	
{ -2.0f, 4.0f, 4.0f},	
{  4.0f, 0.0f, 4.0f }},

{{  -4.0f, 0.0f, 0.0f},	
{ -2.0f, 4.0f, 0.0f},	
{  4.0f, 0.0f, 0.0f }},

{{  -4.0f, 0.0f, -4.0f},	
{ -2.0f, 4.0f, -4.0f},	
{  4.0f, 0.0f, -4.0f }}};


//画控制点 
void DrawPoints(void)
{
  int i,j;	

  glColor3f(1.0f, 0.0f, 0.0f);
  //把点放大一点，看得更清楚 
  glPointSize(5.0f);

  glBegin(GL_POINTS);
  for(i = 0; i < nNumPoints; i++)
    for(j = 0; j < 3; j++)
      glVertex3fv(ctrlPoints[i][j]);
  glEnd();
}




void RenderScene(void)
{
  // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 保存模型视图矩阵
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  //旋转一定的角度方便观察
  glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(60.0f, 1.0f, 0.0f, 0.0f);


  glColor3f(0.0f, 0.0f, 1.0f);
  //设置映射方式，只需要设置一次可以在SetupRC中调用。
  glMap2f(GL_MAP2_VERTEX_3,	//生成的数据类型
    0.0f,						// u的下界
    10.0f,						//u的上界
    3,              //数据中点的间隔
    3,              //u方向上的阶
    0.0f,           //v的下界
    10.0f,          //v的上界
    9,							// 控制点之间的间隔
    3,							// v方向上的阶
    &ctrlPoints[0][0][0]);		//控制点数组

  //启用求值器 
  glEnable(GL_MAP2_VERTEX_3);


  //从0到10映射一个包含10个点的网格 
  glMapGrid2f(10,0.0f,10.0f,10,0.0f,10.0f);

  // 计算网格
  glEvalMesh2(GL_FILL,0,10,0,10);

  //画控制点
  DrawPoints();
  
  glPopMatrix();

  glutSwapBuffers();
}

void SetupRC()
{
  // 设置光源
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // 开启颜色追踪
  glEnable(GL_COLOR_MATERIAL);

  // 设置材料属性
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  // 自动生成法线
  glEnable(GL_AUTO_NORMAL);

}


void ChangeSize(int w, int h)
{
  // Prevent a divide by zero
  if(h == 0)
    h = 1;

  // Set Viewport to window dimensions
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);

  // Modelview matrix reset
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Lit 3D Bezier Surface");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();

  return 0;
}