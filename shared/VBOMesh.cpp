/*
 *  VBOMesh.cpp
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
 *
 *  Very similiar to CTriangleMesh, except the final arrays are stored in Vertex buffer objects
 *  Before anybody flames me, bear in mind this is not a book on object oriented programming, 
 *  and I'm trying to be nice to the C people ;-)
 *  Yes, I know all about inheritance, and it's all good....
 */

#include "VBOMesh.h"


///////////////////////////////////////////////////////////
// Constructor, does what constructors do... set everything to zero or NULL
CVBOMesh::CVBOMesh(void)
    {
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;
    
    nMaxIndexes = 0;
    nNumIndexes = 0;
    nNumVerts = 0;
    }
    
////////////////////////////////////////////////////////////
// Free any dynamically allocated memory. For those C programmers
// coming to C++, it is perfectly valid to delete a NULL pointer.
CVBOMesh::~CVBOMesh(void)
    {
    // Just in case these still are allocated when the object is destroyed
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;
    
    // Delete buffer objects
    glDeleteBuffers(4, bufferObjects);
    }
    
////////////////////////////////////////////////////////////
// Start assembling a mesh. You need to specify a maximum amount
// of indexes that you expect. The EndMesh will clean up any uneeded
// memory. This is far better than shreading your heap with STL containers...
// At least that's my humble opinion.
void CVBOMesh::BeginMesh(GLuint nMaxVerts)
    {
    // Just in case this gets called more than once...
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;
    
    nMaxIndexes = nMaxVerts;
    nNumIndexes = 0;
    nNumVerts = 0;
    
    // Allocate new blocks
    pIndexes = new GLushort[nMaxIndexes];
    pVerts = new M3DVector3f[nMaxIndexes];
    pNorms = new M3DVector3f[nMaxIndexes];
    pTexCoords = new M3DVector2f[nMaxIndexes];
    }
  
/////////////////////////////////////////////////////////////////
// Add a triangle to the mesh. This searches the current list for identical
// (well, almost identical - these are floats you know...) verts. If one is found, it
// is added to the index array. If not, it is added to both the index array and the vertex
// array grows by one as well.
void CVBOMesh::AddTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3])
    {
    const  float e = 0.000001; // How small a difference to equate

    // First thing we do is make sure the normals are unit length!
    // It's almost always a good idea to work with pre-normalized normals
    m3dNormalizeVector(vNorms[0]);
    m3dNormalizeVector(vNorms[1]);
    m3dNormalizeVector(vNorms[2]);


    // Search for match - triangle consists of three verts
    for(GLuint iVertex = 0; iVertex < 3; iVertex++)
        {
        GLuint iMatch = 0;
        for(iMatch = 0; iMatch < nNumVerts; iMatch++)
            {
            // If the vertex positions are the same
            if(m3dCloseEnough(pVerts[iMatch][0], verts[iVertex][0], e) &&
               m3dCloseEnough(pVerts[iMatch][1], verts[iVertex][1], e) &&
               m3dCloseEnough(pVerts[iMatch][2], verts[iVertex][2], e) &&
                   
               // AND the Normal is the same...
               m3dCloseEnough(pNorms[iMatch][0], vNorms[iVertex][0], e) &&
               m3dCloseEnough(pNorms[iMatch][1], vNorms[iVertex][1], e) &&
               m3dCloseEnough(pNorms[iMatch][2], vNorms[iVertex][2], e) &&
                   
                // And Texture is the same...
                m3dCloseEnough(pTexCoords[iMatch][0], vTexCoords[iVertex][0], e) &&
                m3dCloseEnough(pTexCoords[iMatch][1], vTexCoords[iVertex][1], e))
                {
                // Then add the index only
                pIndexes[nNumIndexes] = iMatch;
                nNumIndexes++;
                break;
                }
            }
            
        // No match for this vertex, add to end of list
        if(iMatch == nNumVerts)
            {
            memcpy(pVerts[nNumVerts], verts[iVertex], sizeof(M3DVector3f));
            memcpy(pNorms[nNumVerts], vNorms[iVertex], sizeof(M3DVector3f));
            memcpy(pTexCoords[nNumVerts], &vTexCoords[iVertex], sizeof(M3DVector2f));
            pIndexes[nNumIndexes] = nNumVerts;
            nNumIndexes++; 
            nNumVerts++;
            }   
        }
    }
    


//////////////////////////////////////////////////////////////////
// Compact the data. This is a nice utility, but you should really
// save the results of the indexing for future use if the model data
// is static (doesn't change).
void CVBOMesh::EndMesh(void)
    {
    // Create the buffer objects
    glGenBuffers(4, bufferObjects);
    
    // Copy data to video memory
    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pVerts, GL_STATIC_DRAW);
    
    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pNorms, GL_STATIC_DRAW);
    
    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*2, pTexCoords, GL_STATIC_DRAW);
    
    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*nNumIndexes, pIndexes, GL_STATIC_DRAW);
    
    // Free older, larger arrays
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;

    // Reasign pointers so they are marked as unused
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;
    }

//////////////////////////////////////////////////////////////////////////
// Draw - make sure you call glEnableClientState for these arrays
void CVBOMesh::Draw(void) {
    // Here's where the data is now
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glVertexPointer(3, GL_FLOAT,0,  0);
    
    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glNormalPointer(GL_FLOAT, 0, 0);
           
    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
    glDrawElements(GL_TRIANGLES, nNumIndexes, GL_UNSIGNED_SHORT, 0);
    }

///////////////////////////////////////////////////////////////////////////
// Scale of the vertices. The only way to do this is to map the VBO back
// into client memory, then back again
void CVBOMesh::Scale(GLfloat fScaleValue) 
    {
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    M3DVector3f *pVertexData = (M3DVector3f *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    
    if(pVertexData != NULL)
        {
        for(int i = 0; i < nNumVerts; i++)
            m3dScaleVector3(pVertexData[i], fScaleValue);
    
        glUnmapBuffer(GL_ARRAY_BUFFER);
        }
    }

