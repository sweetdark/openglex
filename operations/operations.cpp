#include "gltools.h"
#include <math.h>

static GLbyte *pImage = NULL;
static GLint iRenderMode = 0;
static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;

void ProcessMenu(int value)
{
	if (value == 1)
	{

    //保存图像的快照
		gltWriteTGA("screenshot.tga");
	}
	else
	{
		iRenderMode = value;
	}

	glutPostRedisplay();

}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

	if (!pImage)
	{
		return;
	}

  //设置光栅位置
  glRasterPos2i(0, 0);

  //修改的图像，和用于反转的映射数组
  GLbyte *pModifyImage = NULL;
  GLfloat invertMap[256];

	switch (iRenderMode)
	{
  case 2:

    //图像倒转
    glPixelZoom(-1.0f, -1.0f);

    //倒转后的图像的,渲染方向也倒转过来了，变成了从右上角开始往左下角渲染，所以设置倒转后的光栅位置为图像的宽高。
    glRasterPos2i(iWidth, iHeight);
    break;
  case 3:

    //让图像填充满屏幕
    GLint viewport[4];

    //取得视口的大小
    glGetIntegerv(GL_VIEWPORT, viewport);

    //按比例缩放
    glPixelZoom((GLfloat)viewport[2]/iWidth, (GLfloat)viewport[3]/iHeight);
    break;

  case 4:

    //只保留红色
    glPixelTransferf(GL_RED_SCALE, 1.0f);
    glPixelTransferf(GL_GREEN_SCALE, 0.0f);
    glPixelTransferf(GL_BLUE_SCALE, 0.0f);
    break;
  case 5:
    
    //只保留绿色
    glPixelTransferf(GL_RED_SCALE, 0.0f);
    glPixelTransferf(GL_GREEN_SCALE, 1.0f);
    glPixelTransferf(GL_BLUE_SCALE, 0.0f);
    break;
  case 6:

    //只保留蓝色
    glPixelTransferf(GL_RED_SCALE, 0.0f);
    glPixelTransferf(GL_GREEN_SCALE, 0.0f);
    glPixelTransferf(GL_BLUE_SCALE, 1.0f);
    break;
  case 7:
    
    //先渲染图像到颜色缓冲区中
    glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

    //NTSC标准,转成黑白图像 
    glPixelTransferf(GL_RED_SCALE, 0.3f);
    glPixelTransferf(GL_GREEN_SCALE, 0.59f);
    glPixelTransferf(GL_BLUE_SCALE, 0.11f);

    //申请临时空间来保存修改后的图像
    pModifyImage = (GLbyte *)malloc(iWidth * iHeight * 3);
    if (!pModifyImage)
    {
      return;
    }

    //从颜色缓冲区中读取图像的亮度数据
    glReadPixels(0, 0, iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, pModifyImage);

    //还原
    glPixelTransferf(GL_RED_SCALE, 1.0f);
    glPixelTransferf(GL_GREEN_SCALE, 1.0f);
    glPixelTransferf(GL_BLUE_SCALE, 1.0f);
    break;
  case 8:

    //设置反转的颜色映射
    invertMap[0] = 1.0f;
    for(int i = 0; i < 256; ++i)
    {
      invertMap[i] = 1.0f - (1.0f / 255.0f * (GLfloat)i);
    }

    //映射
    glPixelMapfv(GL_PIXEL_MAP_R_TO_R, 255, invertMap);
    glPixelMapfv(GL_PIXEL_MAP_G_TO_G, 255, invertMap);
    glPixelMapfv(GL_PIXEL_MAP_B_TO_B, 255, invertMap);

    //开启颜色映射
    glPixelTransferi(GL_MAP_COLOR, GL_TRUE);
	default:
			break;
	}

  if (pModifyImage)
  {

    //画黑白图像
    glDrawPixels(iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, pModifyImage);
    free(pModifyImage);
  }
  else
  {
  	glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);
  }

  //还原
  glPixelTransferf(GL_RED_SCALE, 1.0f);
  glPixelTransferf(GL_GREEN_SCALE, 1.0f);
  glPixelTransferf(GL_BLUE_SCALE, 1.0f);
  glPixelZoom(1.0f, 1.0f);
  glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glutSwapBuffers();
}

void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //设置像素的存储格式
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //加载图像数据
  pImage = gltLoadTGA("horse.tga", &iWidth, &iHeight, &iComponents, &eFormat);

}

//释放图像数据占用的内存空间
void ShutdownRC()
{
	if (pImage)
	{
		free(pImage);
		pImage = NULL;
	}
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

int main(int args, char **argv)
{
	glutInit(&args, argv);
	glutInitDisplayMode(GL_RGB | GL_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("pixel operation");
	
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

  //设置菜单
	int menuID = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Save Image", 1);
	glutAddMenuEntry("Flip", 2);
	glutAddMenuEntry("zoom pixel fill window", 3);
  glutAddMenuEntry("Just Red", 4);
  glutAddMenuEntry("Just Green", 5);
  glutAddMenuEntry("Just Blue", 6);
  glutAddMenuEntry("black & white", 7);
  glutAddMenuEntry("invert map", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	SetupRC();
	glutMainLoop();
	ShutdownRC();

	return 0;
}
