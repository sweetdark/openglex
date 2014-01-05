
#include "png/png.h"
#include "gltools.h"
#include <stdlib.h>
#include <stdio.h>

extern "C" { FILE _iob[3] = {__iob_func()[0], __iob_func()[1], __iob_func()[2]}; }

#pragma (lib, "freeglut.lib")
#pragma (lib, "gltools.lib")

static GLfloat cube[]={-1.0f, -1.0f, -5.0f, //前面的正方形
1.0f, -1.0f,-5.0f,
1.0f, 1.0f, -5.0f, 
-1.0f, 1.0f, -5.0f,
-1.0f, -1.0f, -10.0f,//背面的正方形
1.0f, -1.0f, -10.0f,
1.0f, 1.0f, -10.0f,
-1.0f, 1.0f, -10.0f};

static GLubyte index[]={0, 1, 2, 3, //前面
0, 3, 7, 4, //左面
5, 6, 2, 1, //右面
7, 6, 5, 4, //后面
3, 2, 6, 7, //上面
1, 0, 4, 5 //地面
};

bool LoadPng(const char *name, int &iWidth, int &iHeight, bool &hasAlpha, GLubyte **outData)
{
  FILE *fp = NULL;
  fp = fopen(name, "rb");
  if (fp == NULL)
  {
    return false;
  }

  png_structp png_ptr;
  png_infop info_ptr;
  unsigned int sig_read = 0;
  int color_type, interlace_type;
  
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (png_ptr == NULL)
  {
    fclose(fp);
    return false;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL)
  {
    fclose(fp);
    png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    fclose(fp);
    return false;
  }

  png_init_io(png_ptr, fp);

  png_set_sig_bytes(png_ptr, sig_read);

  png_read_png(png_ptr, info_ptr, /*PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING |*/ PNG_TRANSFORM_EXPAND, png_voidp_NULL);

  iWidth = info_ptr->width;
  iHeight = info_ptr->height;
  switch (info_ptr->color_type)
  {
  case PNG_COLOR_TYPE_RGBA:
    hasAlpha = true;
  	break;
  case PNG_COLOR_TYPE_RGB:
    hasAlpha = false;
    break;
  default:
    printf("color type %d is not supported \n ", info_ptr->color_type);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return false;
  }

  unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
  
  *outData = (unsigned char*)malloc(row_bytes * iHeight);

  png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

  for (int i = 0; i < iHeight; ++i)
  {
    memcpy(*outData + (row_bytes * (iHeight - 1- i)), row_pointers[i], row_bytes);
  }

  png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

  fclose(fp);
  return true;
}


void SetupRC()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  int iWidth, iHeight;
  bool hasAlpha;
  GLubyte *pByte;
  if (LoadPng("1.png", iWidth, iHeight, hasAlpha, &pByte))
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, iWidth, iHeight, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pByte);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
  }

}

void RenderQuad()
{
  glPushMatrix();
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, -1.0f, -5.0f);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, -1.0f, -5.0f);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(1.0f, 1.0f, -5.0f);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-1.0f, 1.0f, -5.0f);
    glEnd();
  glPopMatrix();
}

void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT);

  //glColor3f(0.0f, 0.0f, 1.0f);

  glPushMatrix();
  /*glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cube);

  glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, index);

  glDisableClientState(GL_VERTEX_ARRAY);*/
  RenderQuad();
  glPopMatrix();
  glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
  if(h == 0)
    h = 1;

  glViewport(0, 0, w, h);

  GLfloat fAspect = (GLfloat)w / (GLfloat)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(35.0f, fAspect, 1.0f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


int main(int args, char *argv[])
{
  glutInit(&args, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("png");

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ChangeSize);
  SetupRC();
  glutMainLoop();

  return 0;
}