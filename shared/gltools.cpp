/*
 *  gltools.c
 *  Block
 *
 *  Created by Richard Wright on 10/16/06.
 *  OpenGL SuperBible, 4rth Edition
 *
 */

#include "gltools.h"
#include "math3d.h"
#include <stdio.h>
#include <assert.h>


///////////////////////////////////////////////////////////////////////////////
// Get the OpenGL version number
bool gltGetOpenGLVersion(int &nMajor, int &nMinor)
	{
	const char *szVersionString = (const char *)glGetString(GL_VERSION);
	if(szVersionString == NULL)
		{
		nMajor = 0;
		nMinor = 0;
		return false;
		}
	
	// Get major version number. This stops at the first non numeric character
	nMajor = atoi(szVersionString);
	
	// Get minor version number. Start past the first ".", atoi terminates on first non numeric char.
	nMinor = atoi(strstr(szVersionString, ".")+1);
	
	return true;
	}

///////////////////////////////////////////////////////////////////////////////
// This function determines if the named OpenGL Extension is supported
// Returns 1 or 0
int gltIsExtSupported(const char *extension)
	{
	GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;
	
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;
	
	extensions = (GLubyte *)glGetString(GL_EXTENSIONS);
	
	start = extensions;
	for (;;) 
		{
		where = (GLubyte *) strstr((const char *) start, extension);
		
		if (!where)
			break;
		
		terminator = where + strlen(extension);
		
		if (where == start || *(where - 1) == ' ') 
			{
			if (*terminator == ' ' || *terminator == '\0') 
				return 1;
			}
		start = terminator;
		}
	return 0;
	}

#ifdef _WIN32
///////////////////////////////////////////////////////////////////////////////
// Win32 Only, check for WGL extension
#include "wglext.h"
int gltIsWGLExtSupported(HDC hDC, const char *szExtension)
	{
	GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
	
    // Just look or the entry point
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
    if(wglGetExtensionsStringARB == NULL)
        return 0;
	
	where = (GLubyte *) strchr(szExtension, ' ');
	if (where || *szExtension == '\0')
		return 0;
	
	extensions = (GLubyte *)wglGetExtensionsStringARB(hDC);
	
	start = extensions;
	for (;;) 
		{
		where = (GLubyte *) strstr((const char *) start, szExtension);
		
		if (!where)
			break;
		
		terminator = where + strlen(szExtension);
		
		if (where == start || *(where - 1) == ' ') 
			{
			if (*terminator == ' ' || *terminator == '\0') 
				return 1;
			}
		start = terminator;
		}
	return 0;
	}
  #endif


/////////////////////////////////////////////////////////////
// Get a pointer to an OpenGL extension
// Note on the Mac, this does a lot of work that could be saved
// if you call this function repeatedly. Write your own function that
// gets the bundle once, gets all the function pointers, then releases
// the bundle.
void *gltGetExtensionPointer(const char *szExtensionName)
	{
#ifdef WIN32
    // Well, this one is simple. An OpenGL context must be
    // current first. Returns NULL if extension not supported
    return (void *)wglGetProcAddress(szExtensionName);
#endif
	
#ifdef linux
    // Pretty much ditto above
    return (void *)glXGetProcAddress(szExtensionName);
#endif

    
#ifdef __APPLE__
    // Mac is a bit more tricky.
    // First we need the bundle
    CFBundleRef openGL = 0;
    SInt16      fwVersion = 0;
    SInt32      fwDir = 0;
    
    if(FindFolder(kSystemDomain, kFrameworksFolderType, kDontCreateFolder, &fwVersion, &fwDir) != noErr)
        return NULL;
	
    FSSpec fSpec;
    FSRef  fRef;
    if(FSMakeFSSpec(fwVersion, fwDir, "\pOpenGL.framework", &fSpec) != noErr)
        return NULL;
	
    FSpMakeFSRef(&fSpec, &fRef);
    CFURLRef url = CFURLCreateFromFSRef(kCFAllocatorDefault, &fRef);
    if(!url)
        return NULL;
	
    openGL = CFBundleCreate(kCFAllocatorDefault, url);
    CFRelease(url);
    
    // Then load the function pointer from the bundle
    CFStringRef string = CFStringCreateWithCString(kCFAllocatorDefault, szExtensionName, kCFStringEncodingMacRoman);
    void *pFunc = CFBundleGetFunctionPointerForName(openGL, string);
    
    // Release the bundle and string
    CFRelease(string);
    CFRelease(openGL);
    
    // Return the function ponter
    return pFunc;
#endif    
    }


///////////////////////////////////////////////////////////////////
// Draw the unit axis. A small white sphere represents the origin
// and the three axes are colored Red, Green, and Blue, which 
// corresponds to positive X, Y, and Z respectively. Each axis has
// an arrow on the end, and normals are provided should the axes
// be lit. These are all built using the quadric shapes. For best
// results, put this in a display list.
void gltDrawUnitAxes(void)
	{
	GLUquadricObj *pObj;	// Temporary, used for quadrics
	
	// Measurements
	float fAxisRadius = 0.025f;
	float fAxisHeight = 1.0f;
	float fArrowRadius = 0.06f;
	float fArrowHeight = 0.1f;
	
	// Setup the quadric object
	pObj = gluNewQuadric();
	gluQuadricDrawStyle(pObj, GLU_FILL);
	gluQuadricNormals(pObj, GLU_SMOOTH);
	gluQuadricOrientation(pObj, GLU_OUTSIDE);
	gluQuadricTexture(pObj, GLU_FALSE);
	
	///////////////////////////////////////////////////////
	// Draw the blue Z axis first, with arrowed head
	glColor3f(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fAxisRadius, fAxisRadius, fAxisHeight, 10, 1);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fArrowRadius, 0.0f, fArrowHeight, 10, 1);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(pObj, fAxisRadius, fArrowRadius, 10, 1);
	glPopMatrix();
	
	///////////////////////////////////////////////////////
	// Draw the Red X axis 2nd, with arrowed head
    glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(pObj, fAxisRadius, fAxisRadius, fAxisHeight, 10, 1);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fArrowRadius, 0.0f, fArrowHeight, 10, 1);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	gluDisk(pObj, fAxisRadius, fArrowRadius, 10, 1);
	glPopMatrix();
	glPopMatrix();
	
	///////////////////////////////////////////////////////
	// Draw the Green Y axis 3rd, with arrowed head
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, fAxisRadius, fAxisRadius, fAxisHeight, 10, 1);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fArrowRadius, 0.0f, fArrowHeight, 10, 1);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(pObj, fAxisRadius, fArrowRadius, 10, 1);
	glPopMatrix();
	glPopMatrix();
	
	////////////////////////////////////////////////////////
	// White Sphere at origin
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(pObj, 0.05f, 15, 15);
	
	// Delete the quadric
	gluDeleteQuadric(pObj);
	}


// For best results, put this in a display list
// Draw a torus (doughnut)  at z = fZVal... torus is in xy plane
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor)
	{
    M3DVector3f vNormal;
    double majorStep = 2.0f*M3D_PI / numMajor;
    double minorStep = 2.0f*M3D_PI / numMinor;
    int i, j;
	
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
			GLfloat c = (GLfloat) cos(b);
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat) sin(b);
			
			// First point
			glTexCoord2f((float)(i)/(float)(numMajor), (float)(j)/(float)(numMinor));
			vNormal[0] = x0*c;
			vNormal[1] = y0*c;
			vNormal[2] = z/minorRadius;
			m3dNormalizeVector(vNormal);
			glNormal3fv(vNormal);
			glVertex3f(x0*r, y0*r, z);
			
			glTexCoord2f((float)(i+1)/(float)(numMajor), (float)(j)/(float)(numMinor));
			vNormal[0] = x1*c;
			vNormal[1] = y1*c;
			vNormal[2] = z/minorRadius;
			m3dNormalizeVector(vNormal);
			glNormal3fv(vNormal);
			glVertex3f(x1*r, y1*r, z);
			}
		glEnd();
		}
	}

// For best results, put this in a display list
// Draw a sphere at the origin
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks)
	{
    GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat) iStacks;
    GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat) iSlices;
	GLfloat ds = 1.0f / (GLfloat) iSlices;
	GLfloat dt = 1.0f / (GLfloat) iStacks;
	GLfloat t = 1.0f;	
	GLfloat s = 0.0f;
    GLint i, j;     // Looping variables
	
	for (i = 0; i < iStacks; i++) 
		{
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));
		
        // Many sources of OpenGL sphere drawing code uses a triangle fan
        // for the caps of the sphere. This however introduces texturing 
        // artifacts at the poles on some OpenGL implementations
		glBegin(GL_TRIANGLE_STRIP);
        s = 0.0f;
		for ( j = 0; j <= iSlices; j++) 
			{
			GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));
			
			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;
            
            glTexCoord2f(s, t);
            glNormal3f(x, y, z);
            glVertex3f(x * fRadius, y * fRadius, z * fRadius);
			
            x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;
			glTexCoord2f(s, t - dt);
            s += ds;
            glNormal3f(x, y, z);
            glVertex3f(x * fRadius, y * fRadius, z * fRadius);
			}
        glEnd();

        t -= dt;
        }
    }


// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)


////////////////////////////////////////////////////////////////////
// Capture the current viewport and save it as a targa file.
// Be sure and call SwapBuffers for double buffered contexts or
// glFinish for single buffered contexts before calling this function.
// Returns 0 if an error occurs, or 1 on success.
GLint gltWriteTGA(const char *szFileName)
	{
    FILE *pFile;                // File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;   // Size in bytes of image
    GLbyte	*pBits = NULL;      // Pointer to bits
    GLint iViewport[4];         // Viewport in pixels
    GLenum lastBuffer;          // Storage for the current read buffer setting
    
	// Get the viewport dimensions
	glGetIntegerv(GL_VIEWPORT, iViewport);
	
    // How big is the image going to be (targas are tightly packed)
	lImageSize = iViewport[2] * 3 * iViewport[3];	
	
    // Allocate block. If this doesn't work, go home
    pBits = (GLbyte *)malloc(lImageSize);
    if(pBits == NULL)
        return 0;
	
    // Read bits from color buffer
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    
    // Get the current read buffer setting and save it. Switch to
    // the front buffer and do the read operation. Finally, restore
    // the read buffer state
    glGetIntegerv(GL_READ_BUFFER, (GLint *)&lastBuffer);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, iViewport[2], iViewport[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, pBits);
    glReadBuffer(lastBuffer);
    
    // Initialize the Targa header
    tgaHeader.identsize = 0;
    tgaHeader.colorMapType = 0;
    tgaHeader.imageType = 2;
    tgaHeader.colorMapStart = 0;
    tgaHeader.colorMapLength = 0;
    tgaHeader.colorMapBits = 0;
    tgaHeader.xstart = 0;
    tgaHeader.ystart = 0;
    tgaHeader.width = iViewport[2];
    tgaHeader.height = iViewport[3];
    tgaHeader.bits = 24;
    tgaHeader.descriptor = 0;
    
    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif
    
    // Attempt to open the file
    pFile = fopen(szFileName, "wb");
    if(pFile == NULL)
		{
        free(pBits);    // Free buffer and return error
        return 0;
		}
	
    // Write the header
    fwrite(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
    
    // Write the image data
    fwrite(pBits, lImageSize, 1, pFile);
	
    // Free temporary buffer and close the file
    free(pBits);    
    fclose(pFile);
    
    // Success!
    return 1;
	}


////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
	{
    FILE *pFile;			// File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth;
    GLbyte	*pBits = NULL;          // Pointer to bits
    
    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;
    
    // Attempt to open the fil
    pFile = fopen(szFileName, "rb");
    if(pFile == NULL)
        return NULL;
	
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
    
    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif
	
	
    // Get width, height, and depth of texture
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
	
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    // Allocate memory and check for success
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if(pBits == NULL)
        return NULL;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other 
    // weird formats that I don't want to recognize
    if(fread(pBits, lImageSize, 1, pFile) != 1)
		{
        free(pBits);
        return NULL;
		}
    
    // Set OpenGL format expected
    switch(sDepth)
		{
        case 3:     // Most likely case
            *eFormat = GL_BGR_EXT;
            *iComponents = GL_RGB8;
            break;
        case 4:
            *eFormat = GL_BGRA_EXT;
            *iComponents = GL_RGBA8;
            break;
        case 1:
            *eFormat = GL_LUMINANCE;
            *iComponents = GL_LUMINANCE8;
            break;
		};
	
    
    // Done with File
    fclose(pFile);
	
    // Return pointer to image data
    return pBits;
	}




// Rather than malloc/free a block everytime a shader must be loaded,
// I will dedicate a single 4k page for reading in shaders. Thanks to
// modern OS design, this page will be swapped out to disk later if never
// used again after program initialization. Where-as mallocing different size
// shader blocks could lead to heap fragmentation, which would actually be worse.
//#define MAX_SHADER_LENGTH   4096  -> This is defined in gltools.h
static GLubyte shaderText[MAX_SHADER_LENGTH];

////////////////////////////////////////////////////////////////
// Load the shader from the specified file. Returns false if the
// shader could not be loaded
bool bLoadShaderFile(const char *szFile, GLhandleARB shader)
	{
    GLint shaderLength = 0;
    FILE *fp;
    GLcharARB *fsStringPtr[1];
	
    // Open the shader file
    fp = fopen(szFile, "r");
    if(fp != NULL)
		{
        // See how long the file s
        while (fgetc(fp) != EOF)
            shaderLength++;
		
        // Allocate a block of memory to send in the shader
        assert(shaderLength < MAX_SHADER_LENGTH);   // make me bigger!
        if(shaderLength > MAX_SHADER_LENGTH)
			{
            fclose(fp);
            return false;
			}
		
        // Go back to beginning of file
        rewind(fp);
		
        // Read the whole file in
        if (shaderText != NULL)
            fread(shaderText, 1, shaderLength, fp);
		
        // Make sure it is null terminated and close the file
        shaderText[shaderLength] = '\0';
        fclose(fp);
		}
    else
        return false;
    
	
    // Load the string
    fsStringPtr[0] = (GLcharARB *)shaderText;
    glShaderSourceARB(shader, 1, (const GLcharARB **)fsStringPtr, NULL);
    
    return true;
	}   


/////////////////////////////////////////////////////////////////
// Load a pair of shaders, compile, and link together. Specify the complete
// path and file name of each ASCII shader file. Note, there is no support for
// just loading say a vertex program... you have to do both.
GLhandleARB gltLoadShaderPair(const char *szVertexProg, const char *szFragmentProg)
	{
    // Temporary Shader objects
    GLhandleARB hVertexShader;
    GLhandleARB hFragmentShader; 
    GLhandleARB hReturn = 0;   
    GLint testVal;
	
    // Create shader objects
    hVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    hFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
    // Load them. If fail clean up and return null
    if(bLoadShaderFile(szVertexProg, hVertexShader) == false)
		{
        glDeleteObjectARB(hVertexShader);
        glDeleteObjectARB(hFragmentShader);
        return NULL;
		}
	
    if(bLoadShaderFile(szFragmentProg, hFragmentShader) == false)
		{
        glDeleteObjectARB(hVertexShader);
        glDeleteObjectARB(hFragmentShader);
        return NULL;
		}
    
    // Compile them
    glCompileShaderARB(hVertexShader);
    glCompileShaderARB(hFragmentShader);
    
    // Check for errors
    glGetObjectParameterivARB(hVertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &testVal);
    if(testVal == GL_FALSE)
		{
        glDeleteObjectARB(hVertexShader);
        glDeleteObjectARB(hFragmentShader);
        return NULL;
		}
    
    glGetObjectParameterivARB(hFragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &testVal);
    if(testVal == GL_FALSE)
		{
        glDeleteObjectARB(hVertexShader);
        glDeleteObjectARB(hFragmentShader);
        return NULL;
		}
    
    // Link them - assuming it works...
    hReturn = glCreateProgramObjectARB();
    glAttachObjectARB(hReturn, hVertexShader);
    glAttachObjectARB(hReturn, hFragmentShader);
    glLinkProgramARB(hReturn);
	
    // These are no longer needed
    glDeleteObjectARB(hVertexShader);
    glDeleteObjectARB(hFragmentShader);  
    
    return hReturn;  
	}   


