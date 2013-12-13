#include "gltools.h"	

static GLUnurbsObj *pNurb = NULL;

GLint nNumPoints = 4; // 4 X 4

//                 u  v  (x,y,z)	
GLfloat ctrlPoints[4][4][3]= {{{  -6.0f, -6.0f, 0.0f},	// u = 0,	v = 0
{	  -6.0f, -2.0f, 0.0f},	//			v = 1
{   -6.0f,  2.0f, 0.0f},	//			v = 2	
{   -6.0f,  6.0f, 0.0f}}, //			v = 3

{{  -2.0f, -6.0f, 0.0f},	// u = 1	v = 0
{   -2.0f, -2.0f, 8.0f},	//			v = 1
{   -2.0f,  2.0f, 8.0f},	//			v = 2
{   -2.0f,  6.0f, 0.0f}},	//			v = 3

{{   2.0f, -6.0f, 0.0f }, // u =2		v = 0
{    2.0f, -2.0f, 8.0f }, //			v = 1
{    2.0f,  2.0f, 8.0f },	//			v = 2
{    2.0f,  6.0f, 0.0f }},//			v = 3

{{   6.0f, -6.0f, 0.0f},	// u = 3	v = 0
{    6.0f, -2.0f, 0.0f},	//			v = 1
{    6.0f,  2.0f, 0.0f},	//			v = 2
{    6.0f,  6.0f, 0.0f}}};//			v = 3

// Knot sequence for the NURB
GLfloat Knots[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};


void DrawPoints(void)
{
  glColor3ub(255, 0, 0);
  glPointSize(5.0f);
  glBegin(GL_POINTS);
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        glVertex3fv(ctrlPoints[i][j]);
      }
    }
  glEnd();
}

// NURBS 出错时的回调函数 
void CALLBACK NurbsErrorHandler(GLenum nErrorCode)
{
  char cMessage[100] = {0,};

  strcpy(cMessage, "NURBS error : ");
  strcat(cMessage, (char*)gluErrorString(nErrorCode));

  glutSetWindowTitle(cMessage);
}


void RenderScene(void)
{
  glColor3ub(0,0,220);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glRotatef(330.0f, 1.0f, 0.0f, 0.0f);
  //修剪框是一个闭合的环
  //外修剪框
  GLfloat outSidePts[5][2] = 
  {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}};
  //内修剪框
  GLfloat inSidePts[4][2] =
  {{0.25f, 0.25f}, { 0.5f, 0.5f}, {0.75f, 0.25f},{0.25f, 0.25f} };

  gluBeginSurface(pNurb);
  //定义NURBS表面
  gluNurbsSurface(pNurb,
    8, Knots, //u定义域内的结点个数，以及结点序列
    8, Knots,//v定义域内的结点个数，以及结点序列
    4 * 3,  //u方向上的控制点间隔
    3,  //v方向上的控制点间隔
    &ctrlPoints[0][0][0], //控制点数组
    4, 4, //u v的次数
    GL_MAP2_VERTEX_3);//产生的类型

  //开始修剪
  gluBeginTrim(pNurb);
  gluPwlCurve(pNurb,
    5,  //修剪点的个数
    &outSidePts[0][0], //修剪点数组
    2, //点之间的间隔
    GLU_MAP1_TRIM_2);//修剪的类型
  gluEndTrim(pNurb);

  gluBeginTrim(pNurb);
  gluPwlCurve(pNurb, 4, &inSidePts[0][0], 2, GLU_MAP1_TRIM_2);
  gluEndTrim(pNurb);
  gluEndSurface(pNurb);

  DrawPoints();
  glPopMatrix();

  glutSwapBuffers();
}

void SetupRC()
{
  GLfloat  whiteLight[] = {0.7f, 0.7f, 0.7f, 1.0f };
  GLfloat  specular[] = { 0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat  shine[] = { 100.0f };

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f );

  //开启光照
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // 开启颜色追踪
  glEnable(GL_COLOR_MATERIAL);

  //设置材料属性
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, shine);

  //自动生成法线
  glEnable(GL_AUTO_NORMAL);

  //创建NURBS渲染器
  pNurb = gluNewNurbsRenderer();
  //设置NURBS出错处理函数
  gluNurbsCallback(pNurb, GLU_ERROR, (CallBack)NurbsErrorHandler);
  //设置采样容差
  gluNurbsProperty(pNurb, GLU_SAMPLING_TOLERANCE, 25.0f);
  //设置显示模式
  gluNurbsProperty(pNurb, GLU_DISPLAY_MODE, GLU_FILL);
}


void ChangeSize(int w, int h)
{
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective (45.0f, (GLdouble)w/(GLdouble)h, 1.0, 40.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef (0.0f, 0.0f, -20.0f);
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("NURBS Surface");
  glutReshapeFunc(ChangeSize);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();

  return 0;
}
