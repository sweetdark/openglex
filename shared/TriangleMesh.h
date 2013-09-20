/*
 *  TriangleMesh.h
 *  OpenGL SuperBible
 *
 *  Copyright 2007 Richard S. Wright Jr.. All rights reserved.
 *  This class allows you to simply add triangles as if this class were a 
 *  container. The AddTriangle() function searches the current list of triangles
 *  and determines if the vertex/normal/texcoord is a duplicate. If so, it addes
 *  an entry to the index array instead of the list of vertices.
 *  When finished, call EndMesh() to free up extra unneeded memory that is reserved
 *  as workspace when you call BeginMesh().
 *
 *  This class can easily be extended to contain other vertex attributes, and to 
 *  save itself and load itself from disk (thus forming the beginnings of a custom
 *  model file format).
 */
 
#include "gltools.h"
#include "math3d.h"


class CTriangleMesh
    {
    public:
        CTriangleMesh(void);
        ~CTriangleMesh(void);
        
        // Use these three functions to add triangles
        void BeginMesh(GLuint nMaxVerts);
        void AddTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3]);
        void EndMesh(void);

        // Useful for statistics
        inline GLuint GetIndexCount(void) { return nNumIndexes; }
        inline GLuint GetVertexCount(void) { return nNumVerts; }
        
        // In place scale of the vertices
        void Scale(GLfloat fScaleValue) {
            for(int i = 0; i < nNumVerts; i++)
                m3dScaleVector3(pVerts[i], fScaleValue);
                }
        
        // Draw - make sure you call glEnableClientState for these arrays
        void Draw(void) {
                // Here's where the data is now
            glVertexPointer(3, GL_FLOAT,0, pVerts);
            glNormalPointer(GL_FLOAT, 0, pNorms);
            glTexCoordPointer(2, GL_FLOAT, 0, pTexCoords);

            // Draw them
            glDrawElements(GL_TRIANGLES, nNumIndexes, GL_UNSIGNED_SHORT, pIndexes);
            }
        
    protected:
        GLushort  *pIndexes;          // Array of indexes
        M3DVector3f *pVerts;        // Array of vertices
        M3DVector3f *pNorms;        // Array of normals
        M3DVector2f *pTexCoords;    // Array of texture coordinates
        
        GLuint nMaxIndexes;         // Maximum workspace
        GLuint nNumIndexes;         // Number of indexes currently used
        GLuint nNumVerts;           // 
    };
