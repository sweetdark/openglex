// Planets.c
// OpenGL SuperBible, 3rd Edition
// Richard S. Wright Jr.
// rwright@starstonesoftware.com

#include "gltools.h"	// OpenGL toolkit
#include <math.h>

struct Rectangle
{
  int top;
  int bottom;
  int left;
  int right;
};

///////////////////////////
// Object Names
#define TORUS	1
#define SPHERE	2

struct Rectangle boundingRect;	// Bounding rectangle
GLuint selectedObject = 0;		// Who is selected
float fAspect;					// Display aspect ratio


#if !defined(M_PI)
#define M_PI 3.14159265f
#endif

///////////////////////////////////////////////////////////
// Draw a torus (doughnut)  
// at z = 0... torus aligns with xy plane
void DrawTorus(int numMajor, int numMinor)
{
  float majorRadius = 0.35F;
  float minorRadius = 0.15F;
  double majorStep = 2.0F*M_PI / numMajor;
  double minorStep = 2.0F*M_PI / numMinor;
  int i, j;
  float c, r, z;

  glEnable(GL_NORMALIZE);
  for (i=0; i<numMajor; ++i) 
  {
    double a0 = i * majorStep;
    double a1 = a0 + majorStep;
    GLfloat x0 = (GLfloat) cos(a0);
    GLfloat y0 = (GLfloat) sin(a0);
    GLfloat x1 = (GLfloat) cos(a1);
    GLfloat y1 = (GLfloat) sin(a1);

    glBegin(GL_TRIANGLE_STRIP);
    for (j=0; j<=numMinor; ++j) 
    {
      double b = j * minorStep;
      c = (float) cos(b);
      r = minorRadius * c + majorRadius;
      z = minorRadius * (GLfloat) sin(b);

      glTexCoord2f((float)i/(float)numMajor, (float)j/(float)numMinor);
      glNormal3f(x0*c, y0*c, z/minorRadius);
      glVertex3f(x0*r, y0*r, z);

      glTexCoord2f((float)(i+1)/(float)numMajor, (float)j/(float)numMinor);
      glNormal3f(x1*c, y1*c, z/minorRadius);
      glVertex3f(x1*r, y1*r, z);
    }
    glEnd();
  }
  glDisable(GL_NORMALIZE);
}


///////////////////////////////////////////////////////////
// Just draw a sphere of some given radius
void DrawSphere(float radius)
{
  GLUquadricObj *pObj;
  pObj = gluNewQuadric();
  gluQuadricNormals(pObj, GLU_SMOOTH);
  gluSphere(pObj, radius, 26, 13);
  gluDeleteQuadric(pObj);
}


///////////////////////////////////////////////////////////
// Render the torus and sphere
void DrawObjects(void)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glTranslatef(-0.75f, 0.0f, -2.5f);	

  //初始化名称栈 
  glInitNames();
  glPushName(0);

  //圆环
  glColor3f(1.0f, 1.0f, 0.0f);
  //圆环的名称
  glLoadName(TORUS);
  //设置一个自定义的过渡标记，与圆环的名称相同，以便后面的解析判断
  glPassThrough((GLfloat)TORUS);
  DrawTorus(40, 20);


  //球体
  glColor3f(0.5f, 0.0f, 0.0f);
  glTranslatef(1.5f, 0.0f, 0.0f);
  //球体的名称
  glLoadName(SPHERE);
  //球体的过渡标记
  glPassThrough((GLfloat)SPHERE);	
  DrawSphere(0.5f);

  glPopMatrix();	
}

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //绘制物体
  DrawObjects();

  //选中了某个物体,绘制矩形边框
  if(selectedObject != 0)
  {
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    //重新设置投影矩阵为正交投影，映射可视区域匹配窗体坐标 
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(viewport[0], viewport[2], viewport[3], viewport[1], -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.0f, 0.0f);
    //画矩形边框
    glBegin(GL_LINE_LOOP);
    glVertex2i(boundingRect.left, boundingRect.top);
    glVertex2i(boundingRect.left, boundingRect.bottom);
    glVertex2i(boundingRect.right, boundingRect.bottom);
    glVertex2i(boundingRect.right, boundingRect.top);
    glEnd();
    glEnable(GL_LIGHTING);
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glutSwapBuffers();
}



///////////////////////////////////////////////////////////
// Go into feedback mode and draw a rectangle around the object
#define FEED_BUFF_SIZE	32768
void MakeSelection(int nChoice)
{
  //选择缓冲区
  static GLfloat feedBackBuff[FEED_BUFF_SIZE];

  int size,i,j,count;

  //初始化边界的最大和最小值
  boundingRect.right = boundingRect.bottom = -999999.0f;
  boundingRect.left = boundingRect.top =  999999.0f;

  //设置反馈缓冲区
  glFeedbackBuffer(FEED_BUFF_SIZE,GL_2D, feedBackBuff);

  //进入反馈模式
  glRenderMode(GL_FEEDBACK);

  //绘制物体
  DrawObjects();

  //离开反馈模式
  size = glRenderMode(GL_RENDER);

  
  //解析反馈的数据，获得被选择物体的最大的和最小的窗口坐标 
  i = 0;
  while(i < size)
  {
    // 搜索用户自定义的标记
    if(feedBackBuff[i] == GL_PASS_THROUGH_TOKEN)
      //判断标记是否与我们选择的物体名称相同
      if(feedBackBuff[i+1] == (GLfloat)nChoice)
      {
        i+= 2;
        while(i < size && feedBackBuff[i] != GL_PASS_THROUGH_TOKEN)
        {
          //多边形的标记
          if(feedBackBuff[i] == GL_POLYGON_TOKEN)
          {
            //获得所有多边形的反馈信息
            count = (int)feedBackBuff[++i];//有多少个顶点 
            i++;

            for(j = 0; j < count; j++)	//遍历每一个顶点 
            {
              //取得x的最大最小值
              if(feedBackBuff[i] > boundingRect.right)
                boundingRect.right = feedBackBuff[i];

              if(feedBackBuff[i] < boundingRect.left)
                boundingRect.left = feedBackBuff[i];
              i++;

              //取得y的最大最小值
              if(feedBackBuff[i] > boundingRect.bottom)
                boundingRect.bottom = feedBackBuff[i];

              if(feedBackBuff[i] < boundingRect.top)
                boundingRect.top = feedBackBuff[i];
              i++;
            }
          }
          else
            i++;	
        }
        break;
      }
      i++;
  }
}


///////////////////////////////////////////////////////////
// Process the selection, which is triggered by a right mouse
// click at (xPos, yPos).
#define BUFFER_LENGTH 64
void ProcessSelection(int xPos, int yPos)
{
  // 选择缓冲区
  static GLuint selectBuff[BUFFER_LENGTH];

  GLint hits, viewport[4];

  //设置选择缓冲区 
  glSelectBuffer(BUFFER_LENGTH, selectBuff);

  glGetIntegerv(GL_VIEWPORT, viewport);

  //切换到投影矩阵
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  //进入选择模式
  glRenderMode(GL_SELECT);

  glLoadIdentity();
  //在鼠标位置下，创建一个挑选矩阵
  gluPickMatrix(xPos, viewport[3] - yPos + viewport[1], 2,2, viewport);
  gluPerspective(60.0f, fAspect, 1.0, 425.0);

  //绘制物体
  DrawObjects();

  //收集点击记录
  hits = glRenderMode(GL_RENDER);

  // 
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);

  // 如果选中了一个物体，处理选择的信息
  if(hits == 1)
  {
    MakeSelection(selectBuff[3]);
    if(selectedObject == selectBuff[3])
      selectedObject = 0;
    else
      selectedObject = selectBuff[3];
  }

  glutPostRedisplay();
}



///////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
{
  // Lighting values
  GLfloat  dimLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
  GLfloat  sourceLight[] = { 0.65f, 0.65f, 0.65f, 1.0f };
  GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

  // Light values and coordinates
  glEnable(GL_DEPTH_TEST);	// Hidden surface removal
  glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
  //glEnable(GL_CULL_FACE);		// Do not calculate insides

  // Enable lighting
  glEnable(GL_LIGHTING);

  // Setup and enable light 0
  glLightfv(GL_LIGHT0,GL_AMBIENT,dimLight);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
  glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
  glEnable(GL_LIGHT0);

  // Enable color tracking
  glEnable(GL_COLOR_MATERIAL);

  // Set Material properties to follow glColor values
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  // Gray background
  glClearColor(0.60f, 0.60f, 0.60f, 1.0f );
  glLineWidth(2.0f);
}


///////////////////////////////////////////////////////////
// Set viewport and projection
void ChangeSize(int w, int h)
{
  // Prevent a divide by zero
  if(h == 0)
    h = 1;

  // Set Viewport to window dimensions
  glViewport(0, 0, w, h);

  // Calculate aspect ratio of the window
  fAspect = (GLfloat)w/(GLfloat)h;

  // Set the perspective coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Field of view of 45 degrees, near and far planes 1.0 and 425
  gluPerspective(60.0f, fAspect, 1.0, 425.0);

  // Modelview matrix reset
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

///////////////////////////////////////////////////////////
// Process the mouse click
void MouseCallback(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    ProcessSelection(x, y);
}

///////////////////////////////////////////////////////////
// Program entry point
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800,600);
  glutCreateWindow("Select an Object");
  glutReshapeFunc(ChangeSize);
  glutMouseFunc(MouseCallback);
  glutDisplayFunc(RenderScene);
  SetupRC();
  glutMainLoop();

  return 0;
}
