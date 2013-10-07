// GLTools.h
// OpenGL SuperBible
// Copyright 1998 - 2003 Richard S. Wright Jr.
// Code by Richard S. Wright Jr.
// All Macros prefixed with GLT_, all functions prefixed with glt... This
// should avoid most namespace problems
// Some of these functions allocate memory. Use CRT functions to free
// Report bugs to rwright@starstonesoftware.com

#ifndef __GLTOOLS__LIBRARY
#define __GLTOOLS__LIBRARY

#define FREEGLUT_STATIC

// Bring in OpenGL 
// Windows
#ifdef WIN32
#include <windows.h>		// Must have for Windows platform builds
#include "glee.h"			// OpenGL Extension "autoloader"
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glut.h"			// Glut (Free-Glut on Windows)
#endif

// Mac OS X
#ifdef __APPLE__
#include <Carbon/Carbon.h>	// Brings in most Apple specific stuff
#include "glee.h"			// OpenGL Extension "autoloader"
#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
#include <OpenGL/glu.h>		// OpenGL Utilities
#include <Glut/glut.h>		// Apples Implementation of GLUT

// Just ignore sleep on Apple
#define Sleep(x)

#endif

#ifndef WIN32
typedef GLvoid (*CallBack)(...);            // Visual C++ style
#else
typedef GLvoid (_stdcall *CallBack)();      // XCode (GNU) style
#endif

// Needed for NURBS callbacks... VC++ vs. GNU
/*#ifndef WIN32
#define CALLBACK (GLvoid (*)(...))
#else
#define CALLBACK (GLvoid (__stdcall*)())
#endif
*/

// Universal includes
#include <math.h>



// There is a static block allocated for loading shaders to prevent heap fragmentation
#define MAX_SHADER_LENGTH   8192

    
///////////////////////////////////////////////////////
// Macros for big/little endian happiness
// These are intentionally written to be easy to understand what they 
// are doing... no flames please on the inefficiency of these.
#ifdef __BIG_ENDIAN__
///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_WORD(void *pWord)
	{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;
    
    temp = pBytes[0];
    pBytes[0] = pBytes[1];
    pBytes[1] = temp;
	}

///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_DWORD(void *pWord)
	{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;
    
    // Swap outer bytes
    temp = pBytes[3];
    pBytes[3] = pBytes[0];
    pBytes[0] = temp;
    
    // Swap inner bytes
    temp = pBytes[1];
    pBytes[1] = pBytes[2];
    pBytes[2] = temp;
	}
#else

// Define them away on little endian systems
#define LITTLE_ENDIAN_WORD 
#define LITTLE_ENDIAN_DWORD 
#endif


///////////////////////////////////////////////////////////////////////////////
//         THE LIBRARY....
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Load a .TGA file
GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

// Capute the frame buffer and write it as a .tga
GLint gltWriteTGA(const char *szFileName);

// Draw a Torus
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);

// Just draw a simple sphere with normals and texture coordinates
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);

// Draw a 3D unit Axis set
void gltDrawUnitAxes(void);

// Shader loading support
bool bLoadShaderFile(const char *szFile, GLhandleARB shader);
GLhandleARB gltLoadShaderPair(const char *szVertexProg, const char *szFragmentProg);

// Get the OpenGL version, returns fals on error
bool gltGetOpenGLVersion(int &nMajor, int &nMinor);

// Check to see if an exension is supported
int gltIsExtSupported(const char *szExtension);

// Get the function pointer for an extension
void *gltGetExtensionPointer(const char *szFunctionName);

///////////////////////////////////////////////////////////////////////////////
// Win32 Only
#ifdef WIN32
int gltIsWGLExtSupported(HDC hDC, const char *szExtension);
#endif


#endif