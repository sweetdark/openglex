// ThunderBird.cpp
// OpenGL SuperBible
// Demonstrates rendering a sample model with indexed vertex arrays
// Program by Richard S. Wright Jr.

#include "gltools.h"	// OpenGL toolkit
#include <math.h>
#include "math3d.h"

// Thunderbird body
extern short face_indicies[3704][9];
extern GLfloat vertices [1898][3];
extern GLfloat normals [2716][3];
extern GLfloat textures [2925][2];

// Glass cock-pit
extern short face_indiciesGlass[352][9];
extern GLfloat verticesGlass[197][3];
extern GLfloat normalsGlass [227][3];
extern GLfloat texturesGlass [227][2];


#define BODY_TEXTURE    0
#define GLASS_TEXTURE   1
GLuint  textureObjects[2];

GLuint bodyList, glassList;
        
/////////////////////////////////////////////////////////
// Draw the Thunderbirds body
void DrawBody(void)
    {
    int iFace, iPoint;
    glBegin(GL_TRIANGLES);
        for(iFace = 0; iFace < 3704; iFace++)  // Each new triangle starts here
            for(iPoint = 0; iPoint < 3; iPoint++) // Each vertex specified here
                {
                // Lookup the texture value
                glTexCoord2fv(textures[face_indicies[iFace][iPoint+6]]);

                // Lookup the normal value
                glNormal3fv(normals[face_indicies[iFace][iPoint+3]]);

                // Lookup the vertex value
                glVertex3fv(vertices[face_indicies[iFace][iPoint]]);
                }
    glEnd();
    }

//////////////////////////////////////////////////////////
// Draw the Thunderbird's canopy
void DrawGlass(void)
    {
    int iFace, iPoint;
    glBegin(GL_TRIANGLES);
        for(iFace = 0; iFace < 352; iFace++)  // Each new triangle starts here
            for(iPoint = 0; iPoint < 3; iPoint++) // Each vertex specified here
                {
                // Lookup the texture value
                glTexCoord2fv(texturesGlass[face_indiciesGlass[iFace][iPoint+6]]);

                // Lookup the normal value
                glNormal3fv(normalsGlass[face_indiciesGlass[iFace][iPoint+3]]);

                // Lookup the vertex value
                glVertex3fv(verticesGlass[face_indiciesGlass[iFace][iPoint]]);
                }
    glEnd();
    }


        
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
    {
    GLint iWidth, iHeight,iComponents;
    GLenum eFormat;
    GLbyte *pBytes;
    
    GLfloat fAmbLight[] =   { 0.1f, 0.1f, 0.1f, 0.0f };
    GLfloat fDiffLight[] =  { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat fSpecLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat lightPos[] = { -100.0f, 100.0f, 100.0f, 1.0f };
    GLfloat fScale = 0.01f;

    // Bluish background
    glClearColor(0.0f, 0.0f, .50f, 1.0f );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
        
    // Lit texture environment
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glGenTextures(2, textureObjects);
    
    // Load the body texture
    glBindTexture(GL_TEXTURE_2D, textureObjects[BODY_TEXTURE]);
    pBytes = gltLoadTGA("..\\images\\BODY.tga", &iWidth, &iHeight, &iComponents, &eFormat);    
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void *)pBytes);
    free(pBytes);
    
    GLfloat fLargest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glBindTexture(GL_TEXTURE_2D, textureObjects[GLASS_TEXTURE]);
    pBytes = gltLoadTGA("..\\images\\glass.tga", &iWidth, &iHeight, &iComponents, &eFormat);    
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void *)pBytes);
    free(pBytes);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glEnable(GL_TEXTURE_2D);

    // Set up lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fDiffLight);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecLight);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    // Light never changes, put it here
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    glEnable(GL_RESCALE_NORMAL);
    
    bodyList = glGenLists(2);
    glassList = bodyList + 1;
    
    glNewList(bodyList, GL_COMPILE);
        DrawBody();
    glEndList();
    
    glNewList(glassList, GL_COMPILE);
        DrawGlass();
    glEndList();
    }
    
/////////////////////////////////////////////////////////
void ShutdownRC(void)
    {
    glDeleteLists(bodyList, 2);
    glGenTextures(2, textureObjects);
    }
    
    
// Called to draw scene
void RenderScene(void)
    {
    static GLfloat yRot = 0.0f;
    yRot += 0.5f;
        
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        
    glTranslatef(0.0f, 0.0f, -4.0f);
    glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, textureObjects[BODY_TEXTURE]);
        glScalef(.01, .01, .01);
        glCallList(bodyList); //DrawBody();
    glPopMatrix();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTranslatef(0.0f, 0.132f, 0.555f);
    glBindTexture(GL_TEXTURE_2D, textureObjects[GLASS_TEXTURE]);
    
    glScalef(0.01, 0.01, 0.01);
    glFrontFace(GL_CW);
    glCallList(glassList); //DrawGlass();
    glFrontFace(GL_CCW);
    glCallList(glassList); //DrawGlass();
    glDisable(GL_BLEND);
    
    
    glPopMatrix();
    
    // Do the buffer Swap
    glutSwapBuffers();
    }


///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
    {
    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(5,TimerFunction, 1);
    }


//////////////////////////////////////////////////////////
// Window has changed size
void ChangeSize(int w, int h)
	{
	GLfloat fAspect;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);
        
    fAspect = (GLfloat)w / (GLfloat)h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the clipping volume
	gluPerspective(35.0f, fAspect, 1.0f, 1000.0f);
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }

int main(int argc, char* argv[])
	{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
	glutCreateWindow("F-16 Thunderbird");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	SetupRC();
    glutTimerFunc(5, TimerFunction, 1);

	glutMainLoop();

    ShutdownRC();
    
	return 0;
	}
