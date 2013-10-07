// GLFrustum.h
// Code by Richard S. Wright Jr.
// Encapsulates a frustum... works in conjunction
// with GLFrame

#include <math3d.h>
#include <GLFrame.h>

#ifndef __GL_FRAME_CLASS
#define __GL_FRAME_CLASS


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class GLFrustum
    {
    public:
        GLFrustum(void)       // Set some Reasonable Defaults
            { Set(30.0f, 1.0, 1.0, 10.0); }

        // Set the View Frustum
        GLFrustum(float fFov, float fAspect, float fNear, float fFar)
            { Set(fFov, fAspect, fNear, fFar); }
        
        // Calculates the corners of the Frustum and sets the projection matrix.
        // Switches to projection matrix before returning
        void Set(float fFov, float fAspect, float fNear, float fFar)
            {
            float xmin, xmax, ymin, ymax;       // Dimensions of near clipping plane
            float xFmin, xFmax, yFmin, yFmax;   // Dimensions of far clipping plane

            // Do the Math for the near clipping plane
            ymax = fNear * float(tan( fFov * M3D_PI / 360.0 ));
            ymin = -ymax;
            xmin = ymin * fAspect;
            xmax = -xmin; 

            // Do the Math for the far clipping plane
            yFmax = fFar * float(tan(fFov * M3D_PI / 360.0));
            yFmin = -yFmax;
            xFmin = yFmin * fAspect;
            xFmax = -xFmin;

            // Do the GL
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum( xmin, xmax, ymin, ymax, fNear, fFar );
            glMatrixMode(GL_MODELVIEW);


            // Fill in values for untransformed Frustum corners
            // Near Upper Left
            nearUL[0] = xmin; nearUL[1] = ymax; nearUL[2] = -fNear; nearUL[3] = 1.0f;

            // Near Lower Left
            nearLL[0] = xmin; nearLL[1] = ymin; nearLL[2] = -fNear; nearLL[3] = 1.0f;

            // Near Upper Right
            nearUR[0] = xmax; nearUR[1] = ymax; nearUR[2] = -fNear; nearUR[3] = 1.0f;

            // Near Lower Right
            nearLR[0] = xmax; nearLR[1] = ymin; nearLR[2] = -fNear; nearLR[3] = 1.0f;

            // Far Upper Left
            farUL[0] = xFmin; farUL[1] = yFmax; farUL[2] = -fFar; farUL[3] = 1.0f;

            // Far Lower Left
            farLL[0] = xFmin; farLL[1] = yFmin; farLL[2] = -fFar; farLL[3] = 1.0f;

            // Far Upper Right
            farUR[0] = xFmax; farUR[1] = yFmax; farUR[2] = -fFar; farUR[3] = 1.0f;

            // Far Lower Right
            farLR[0] = xFmax; farLR[1] = yFmin; farLR[2] = -fFar; farLR[3] = 1.0f;
            }


        // Builds a transformation matrix and transforms the corners of the Frustum,
        // then derives the plane equations
        void Transform(GLFrame& Camera)
            {
            // Workspace
   			M3DMatrix44f rotMat;
            M3DVector3f vForward, vUp, vCross;
            M3DVector3f   vOrigin;

            ///////////////////////////////////////////////////////////////////
            // Create the transformation matrix. This was the trickiest part
            // for me. The default view from OpenGL is down the negative Z
            // axis. However, building a transformation axis from these 
            // directional vectors points the frustum the wrong direction. So
            // You must reverse them here, or build the initial frustum
            // backwards - which to do is purely a matter of taste. I chose to
            // compensate here to allow better operability with some of my other
            // legacy code and projects. RSW
            Camera.GetForwardVector(vForward);
            vForward[0] = -vForward[0];
            vForward[1] = -vForward[1];
            vForward[2] = -vForward[2];

            Camera.GetUpVector(vUp);
            Camera.GetOrigin(vOrigin);
   
	   		// Calculate the right side (x) vector
            m3dCrossProductf(vCross, vUp, vForward);

            // The Matrix
   			// X Column
	   		memcpy(rotMat, vCross, sizeof(float)*3);
            rotMat[3] = 0.0f;
           
            // Y Column
		   	memcpy(&rotMat[4], vUp, sizeof(float)*3);
            rotMat[7] = 0.0f;       
                                    
            // Z Column
		   	memcpy(&rotMat[8], vForward, sizeof(float)*3);
            rotMat[11] = 0.0f;

            // Translation
			rotMat[12] = vOrigin[0];
            rotMat[13] = vOrigin[1];
            rotMat[14] = vOrigin[2];
            rotMat[15] = 1.0f;

            ////////////////////////////////////////////////////
            // Transform the frustum corners
            m3dTransformVector4f(nearULT, rotMat, nearUL);
            m3dTransformVector4f(nearLLT, rotMat, nearLL);
            m3dTransformVector4f(nearURT, rotMat, nearUR);
            m3dTransformVector4f(nearLRT, rotMat, nearLR);
            m3dTransformVector4f(farULT, rotMat, farUL);
            m3dTransformVector4f(farLLT, rotMat, farLL);
            m3dTransformVector4f(farURT, rotMat, farUR);
            m3dTransformVector4f(farLRT, rotMat, farLR);

            ////////////////////////////////////////////////////
            // Derive Plane Equations from points... Points given in
            // counter clockwise order to make normals point inside 
            // the Frustum
            // Near and Far Planes
            m3dGetPlaneEquationf(nearPlane, nearULT, nearLLT, nearLRT);
            m3dGetPlaneEquationf(farPlane, farULT, farURT, farLRT);
            
            // Top and Bottom Planes
            m3dGetPlaneEquationf(topPlane, nearULT, nearURT, farURT);
            m3dGetPlaneEquationf(bottomPlane, nearLLT, farLLT, farLRT);

            // Left and right planes
            m3dGetPlaneEquationf(leftPlane, nearLLT, nearULT, farULT);
            m3dGetPlaneEquationf(rightPlane, nearLRT, farLRT, farURT);
            }

        

        // Allow expanded version of sphere test
        bool TestSphere(float x, float y, float z, float fRadius)
            {
            M3DVector3f vPoint;
            vPoint[0] = x;
            vPoint[1] = y;
            vPoint[2] = z;

            return TestSphere(vPoint, fRadius);
            }

        // Test a point against all frustum planes. A negative distance for any
        // single plane means it is outside the frustum. The radius value allows
        // to test for a point (radius = 0), or a sphere. Possibly there might
        // be some gain in an alternative function that saves the addition of 
        // zero in this case.
        // Returns false if it is not in the frustum, true if it intersects
        // the Frustum.
        bool TestSphere(M3DVector3f vPoint, float fRadius)
            {
            float fDist;

            // Near Plane - See if it is behind me
            fDist = m3dGetDistanceToPlanef(vPoint, nearPlane);
            if(fDist + fRadius <= 0.0)
                return false;

            // Distance to far plane
            fDist = m3dGetDistanceToPlanef(vPoint, farPlane);
            if(fDist + fRadius <= 0.0)
                return false;

            fDist = m3dGetDistanceToPlanef(vPoint, leftPlane);
            if(fDist + fRadius <= 0.0)
                return false;

            fDist = m3dGetDistanceToPlanef(vPoint, rightPlane);
            if(fDist + fRadius <= 0.0)
                return false;

            fDist = m3dGetDistanceToPlanef(vPoint, bottomPlane);
            if(fDist + fRadius <= 0.0)
                return false;

            fDist = m3dGetDistanceToPlanef(vPoint, topPlane);
            if(fDist + fRadius <= 0.0)
                return false;

            return true;
            }

    protected:
        // Untransformed corners of the frustum
        M3DVector4f  nearUL, nearLL, nearUR, nearLR;
        M3DVector4f  farUL,  farLL,  farUR,  farLR;

        // Transformed corners of Frustum
        M3DVector4f  nearULT, nearLLT, nearURT, nearLRT;
        M3DVector4f  farULT,  farLLT,  farURT,  farLRT;


        // Base and Transformed plane equations
        M3DVector4f nearPlane, farPlane, leftPlane, rightPlane;
        M3DVector4f topPlane, bottomPlane;
       

    };



#endif