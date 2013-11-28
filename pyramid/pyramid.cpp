#include "gltools.h"
#include "math3d.h"
#include <stdio.h>

static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;
static GLfloat xRot, yRot;

static GLfloat noLight[4] = {0.0f, 0.0f, 0.0f, 1.0f};
static GLfloat ambientLight[4] = {0.3f, 0.3f, 0.3f, 1.0f};
static GLfloat diffuseLight[4] = {0.7f, 0.7f, 0.7f, 1.0f};
static GLfloat brightLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};

//光的位置在右上角
static GLfloat lightPos[] = { 5.0f, 5.0f, 5.0f, 1.0f};

void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  //设置光照环境
  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, brightLight);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHT0);
  
  //开启颜色追踪
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, brightLight);

  //镜面光加亮的范围设置大一点
  glMateriali(GL_FRONT, GL_SHININESS, 30);

  //读取图像文件
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	void *pImage = NULL;
	pImage = gltLoadTGA("..\\stone.tga", &iWidth, &iHeight, &iComponents, &eFormat);
  printf("image size is : %d\n", iWidth * iHeight * 3);

	if (pImage)
	{
    //加载纹理，然后释放临时的内存
		//glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
    GLint flag;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &flag);
    printf("compress flag : %d\n", flag);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &flag);
    printf("compress size : %d\n", flag);
		free(pImage);
		pImage = NULL;
	}

  //设置纹理过滤
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //设置纹理环境
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*
  金字塔顶点下标为0,底部坐标如下
    1______2
    |      |
    |______|
    3      4
  */
  //金字塔顶点数组
	M3DVector3f vertices[5] = {{0.0f, 0.8f, 0.0f},
		{-.50f, 0.0f, -.50f},
		{.50f, 0.0f, -.50f},
		{-.50f, 0.0f, .50f},
		{.50f, 0.0f, .50f}};

  //表面法线向量
  M3DVector3f normal;

	glPushMatrix();

    //先往里和往下平移一点
    glTranslatef(0.0f, -0.3f, -4.0f);
    if (xRot > 360.5f)
    {
      xRot = 0.0f;
    }

    if (yRot > 360.5f)
    {
      yRot = 0.0f;
    }
    
    //进行旋转
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    xRot += 0.5f;
    yRot += 0.5f;

		glBegin(GL_TRIANGLES);

      //底面的四方形由两个三角形组成
      glColor3f(1.0f, 0.0f, 0.0f);

      //注意法线和纹理都要在顶点之前设置
  		glNormal3f(0.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
  		glVertex3fv(vertices[1]);
      glTexCoord2f(1.0f, 1.0f);
  		glVertex3fv(vertices[2]);
      glTexCoord2f(1.0f, 0.0f);
  		glVertex3fv(vertices[4]);

      glTexCoord2f(1.0f, 0.0f);
  		glVertex3fv(vertices[4]);
      glTexCoord2f(0.0f, 0.0f);
  		glVertex3fv(vertices[3]);
      glTexCoord2f(0.0f, 1.0f);
  		glVertex3fv(vertices[1]);

      //前面
      glColor3f(0.0f, 1.0f, 0.0f);
  		m3dFindNormal(normal, vertices[0], vertices[3], vertices[4]);
      glNormal3fv(normal);
      glTexCoord2f(0.5f, 0.5f);
  		glVertex3fv(vertices[0]);
      glTexCoord2f(0.0f, 0.0f);
  		glVertex3fv(vertices[3]);
      glTexCoord2f(1.0f, 0.0f);
  		glVertex3fv(vertices[4]);

      //左侧面
      glColor3f(0.0f, 0.0f, 1.0f);
      m3dFindNormal(normal, vertices[1], vertices[3], vertices[0]);
      glNormal3fv(normal);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(vertices[1]);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(vertices[3]);
      glTexCoord2f(0.5f, 0.5f);
      glVertex3fv(vertices[0]);

      //右侧面
      glColor3f(0.0f, 1.0f, 1.0f);
      m3dFindNormal(normal, vertices[0], vertices[4], vertices[2]);
      glNormal3fv(normal);
      glTexCoord2f(0.5f, 0.5f);
      glVertex3fv(vertices[0]);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(vertices[4]);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(vertices[2]);

      //后面
      glColor3f(1.0f, 0.0f, 1.0f);
      m3dFindNormal(normal, vertices[0], vertices[2], vertices[1]);
      glNormal3fv(normal);
      glTexCoord2f(0.5f, 0.5f);
      glVertex3fv(vertices[0]);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3fv(vertices[2]);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3fv(vertices[1]);

		glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	float fAspect = (GLfloat)w/(GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35.0, fAspect, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void TimerFunc(int value)
{
  glutPostRedisplay();
  glutTimerFunc(60, TimerFunc, 1);
}


int main(int args, char *arv[])
{
	glutInit(&args, arv);
	glutInitDisplayMode(GL_RGB | GL_DOUBLE | GL_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("pyramid");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();

  glutTimerFunc(50, TimerFunc, 1);

	glutMainLoop();

	return 0;
}

