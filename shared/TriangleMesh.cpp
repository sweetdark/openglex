/*
 *  TriangleMesh.cpp
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

#include "TriangleMesh.h"


///////////////////////////////////////////////////////////
// Constructor, does what constructors do... set everything to zero or NULL
CTriangleMesh::CTriangleMesh(void)
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
CTriangleMesh::~CTriangleMesh(void)
    {
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;
    }
    
////////////////////////////////////////////////////////////
// Start assembling a mesh. You need to specify a maximum amount
// of indexes that you expect. The EndMesh will clean up any uneeded
// memory. This is far better than shreading your heap with STL containers...
// At least that's my humble opinion.
void CTriangleMesh::BeginMesh(GLuint nMaxVerts)
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
void CTriangleMesh::AddTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3])
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
void CTriangleMesh::EndMesh(void)
    {
    // Allocate smaller arrays
    GLushort *pPackedIndexes = new GLushort[nNumIndexes];
    M3DVector3f *pPackedVerts = new M3DVector3f[nNumVerts];
    M3DVector3f *pPackedNorms = new M3DVector3f[nNumVerts];
    M3DVector2f *pPackedTex = new M3DVector2f[nNumVerts];
    
    // Copy data to smaller arrays
    memcpy(pPackedIndexes, pIndexes, sizeof(GLushort)*nNumIndexes);
    memcpy(pPackedVerts, pVerts, sizeof(M3DVector3f)*nNumVerts);
    memcpy(pPackedNorms, pNorms, sizeof(M3DVector3f)*nNumVerts);
    memcpy(pPackedTex, pTexCoords, sizeof(M3DVector2f)*nNumVerts);
    
    // Free older, larger arrays
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;

    // Reasign pointers
    pIndexes = pPackedIndexes;
    pVerts = pPackedVerts;
    pNorms = pPackedNorms;
    pTexCoords = pPackedTex;
    }

