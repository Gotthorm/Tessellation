#include <windows.h>
#include <assert.h>
//#include "GLUT\glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Patch.hpp"
#include "Landscape.hpp"
#include "Common\Vector3.hpp"
#include "Common\Math.hpp"

extern Vector3 g_vCameraPosition;
//extern int g_nCurrentRenderMode;


float Patch::sm_fPlanarScalar = 1.0f;
float Patch::sm_fYScalar = 1.0f;
int Patch::sm_nSize = 0;
int Patch::sm_nMapSize = 0;


#define MAX(a,b) ((a < b) ? (b) : (a))

#ifndef NULL
#define NULL 0
#endif

Patch::Patch()
{
}

Patch::~Patch()
{
}

// ---------------------------------------------------------------------
// Split a single Triangle and link it into the mesh.
// Will correctly force-split diamonds.
//
void Patch::Split( TreeNode* pTreeNode )
{
	// We are already split, no need to do it again.
	if( pTreeNode->pLeftChild )
	{
		return;
	}

	// If this triangle is not in a proper diamond, force split our base neighbor
	if( pTreeNode->pBaseNeighbor && ( pTreeNode->pBaseNeighbor->pBaseNeighbor != pTreeNode ) )
	{
		Split( pTreeNode->pBaseNeighbor );
	}

	// Create children and link into mesh
	pTreeNode->pLeftChild  = m_pLandscape->AllocateTreeNode();
	pTreeNode->pRightChild = m_pLandscape->AllocateTreeNode();

	// If creation failed, just exit.
	if( !pTreeNode->pLeftChild )
	{
		return;
	}

	// Fill in the information we can get from the parent (neighbor pointers)
	pTreeNode->pLeftChild->pBaseNeighbor  = pTreeNode->pLeftNeighbor;
	pTreeNode->pLeftChild->pLeftNeighbor  = pTreeNode->pRightChild;

	pTreeNode->pRightChild->pBaseNeighbor  = pTreeNode->pRightNeighbor;
	pTreeNode->pRightChild->pRightNeighbor = pTreeNode->pLeftChild;

	// Link our Left Neighbor to the new children
	if (pTreeNode->pLeftNeighbor != NULL)
	{
		if (pTreeNode->pLeftNeighbor->pBaseNeighbor == pTreeNode)
			pTreeNode->pLeftNeighbor->pBaseNeighbor = pTreeNode->pLeftChild;
		else if (pTreeNode->pLeftNeighbor->pLeftNeighbor == pTreeNode)
			pTreeNode->pLeftNeighbor->pLeftNeighbor = pTreeNode->pLeftChild;
		else if (pTreeNode->pLeftNeighbor->pRightNeighbor == pTreeNode)
			pTreeNode->pLeftNeighbor->pRightNeighbor = pTreeNode->pLeftChild;
		else
			;// Illegal Left Neighbor!
	}

	// Link our Right Neighbor to the new children
	if (pTreeNode->pRightNeighbor != NULL)
	{
		if (pTreeNode->pRightNeighbor->pBaseNeighbor == pTreeNode)
			pTreeNode->pRightNeighbor->pBaseNeighbor = pTreeNode->pRightChild;
		else if (pTreeNode->pRightNeighbor->pRightNeighbor == pTreeNode)
			pTreeNode->pRightNeighbor->pRightNeighbor = pTreeNode->pRightChild;
		else if (pTreeNode->pRightNeighbor->pLeftNeighbor == pTreeNode)
			pTreeNode->pRightNeighbor->pLeftNeighbor = pTreeNode->pRightChild;
		else
			;// Illegal Right Neighbor!
	}

	// Link our Base Neighbor to the new children
	if (pTreeNode->pBaseNeighbor != NULL)
	{
		if ( pTreeNode->pBaseNeighbor->pLeftChild )
		{
			pTreeNode->pBaseNeighbor->pLeftChild->pRightNeighbor = pTreeNode->pRightChild;
			pTreeNode->pBaseNeighbor->pRightChild->pLeftNeighbor = pTreeNode->pLeftChild;
			pTreeNode->pLeftChild->pRightNeighbor = pTreeNode->pBaseNeighbor->pRightChild;
			pTreeNode->pRightChild->pLeftNeighbor = pTreeNode->pBaseNeighbor->pLeftChild;
		}
		else
			Split( pTreeNode->pBaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
	}
	else
	{
		// An edge triangle, trivial case.
		pTreeNode->pLeftChild->pRightNeighbor = NULL;
		pTreeNode->pRightChild->pLeftNeighbor = NULL;
	}
}

// ---------------------------------------------------------------------
// Tessellate a Patch.
// Will continue to split until the variance metric is met.
//
void Patch::RecursTessellate( TreeNode* pTriangleNode,
							 const Vector3& vCameraPosition,
							 float fFrameVariance,
							 int leftX,  int leftY,
							 int rightX, int rightY,
							 int apexX,  int apexY,
							 int node )
{
	float TriVariance;
	int centerX = (leftX + rightX)>>1; // Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY)>>1; // Compute Y coord...

	if ( node < (1<<VARIANCE_DEPTH) )
	{
		// Extremely slow distance metric (sqrt is used).
		// Replace this with a faster one!
		float worldCenterX = centerX * sm_fPlanarScalar;
		float worldCenterY = centerY * sm_fPlanarScalar;

		float distance = 1.0f + Math::Sqrt( Math::Sqr(worldCenterX - vCameraPosition[0]) +
									   Math::Sqr(worldCenterY - vCameraPosition[2]) );
		
		// Egads!  A division too?  What's this world coming to!
		// This should also be replaced with a faster operation.
		TriVariance = ((float)m_CurrentVariance[node] * sm_nMapSize * sm_fPlanarScalar * 2)/distance;	// Take both distance and variance into consideration
	}

	if ( (node >= (1<<VARIANCE_DEPTH)) ||	// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
		 (TriVariance > fFrameVariance))	// OR if we are not below the variance tree, test for variance.
	{
		Split(pTriangleNode);														// Split this triangle.
		
		if (pTriangleNode->pLeftChild &&											// If this triangle was split, try to split it's children as well.
			((Math::FAbs(leftX - rightX) >= 3) || (Math::FAbs(leftY - rightY) >= 3)))	// Tessellate all the way down to one vertex per height field entry
		{
			RecursTessellate( pTriangleNode->pLeftChild, vCameraPosition, fFrameVariance, apexX,  apexY, leftX, leftY, centerX, centerY,    node<<1  );
			RecursTessellate( pTriangleNode->pRightChild, vCameraPosition, fFrameVariance, rightX, rightY, apexX, apexY, centerX, centerY, 1+(node<<1) );
		}
	}
}

// ---------------------------------------------------------------------
// Render the tree.  Simple no-fan method.
//
int Patch::RecursRender( TreeNode* pTriangleNode, int leftXIndex, int leftYIndex, int rightXIndex, int rightYIndex, int apexXIndex, int apexYIndex )
{
	int nTrianglesRendered = 0;

	if( pTriangleNode->pLeftChild )					// All non-leaf nodes have both children, so just check for one
	{
		int centerX = (leftXIndex + rightXIndex)>>1;	// Compute X coordinate of center of Hypotenuse
		int centerY = (leftYIndex + rightYIndex)>>1;	// Compute Y coord...

		nTrianglesRendered += RecursRender( pTriangleNode->pLeftChild,  apexXIndex,   apexYIndex, leftXIndex, leftYIndex, centerX, centerY );
		nTrianglesRendered += RecursRender( pTriangleNode->pRightChild, rightXIndex, rightYIndex, apexXIndex, apexYIndex, centerX, centerY );
	}
	else									// A leaf node!  Output a triangle to be rendered.
	{
		// Actual number of rendered triangles...
		nTrianglesRendered++;

		// Calculate the Heights of the three vertices
		GLfloat leftZ  = float( m_HeightMap[(leftYIndex *sm_nMapSize)+leftXIndex ] ) * sm_fYScalar;
		GLfloat rightZ = float( m_HeightMap[(rightYIndex*sm_nMapSize)+rightXIndex] ) * sm_fYScalar;
		GLfloat apexZ  = float( m_HeightMap[(apexYIndex *sm_nMapSize)+apexXIndex ] ) * sm_fYScalar;

		// The x and y values for the vertices are actually just index values
		// so we need to scale them now to real world coordinates
		GLfloat leftX = leftXIndex * sm_fPlanarScalar;
		GLfloat leftY = leftYIndex * sm_fPlanarScalar;
		GLfloat rightX = rightXIndex * sm_fPlanarScalar;
		GLfloat rightY = rightYIndex * sm_fPlanarScalar;
		GLfloat apexX = apexXIndex * sm_fPlanarScalar;
		GLfloat apexY = apexYIndex * sm_fPlanarScalar;

		// Enable for normal generation
		if( 1 )
		{
			Vector3 vPoint1( leftX, leftZ, leftY );
			Vector3 vPoint2( rightX, rightZ, rightY );
			Vector3 vPoint3( apexX, apexZ, apexY );

			Vector3 vNormal( vPoint2 - vPoint1 );
			vNormal = vNormal.UnitCross( vPoint3 - vPoint1 );

			glNormal3fv( vNormal );
		}

/*
		// Perform lighting calculations if requested.
		if (gDrawMode == DRAW_USE_LIGHTING)
		{
			float v[3][3];
			float out[3];
			
			// Create a vertex normal for this triangle.
			// NOTE: This is an extremely slow operation for illustration purposes only.
			//       You should use a texture map with the lighting pre-applied to the texture.
			v[0][0] = (GLfloat) leftX;
			v[0][1] = (GLfloat) leftZ;
			v[0][2] = (GLfloat) leftY;
			
			v[1][0] = (GLfloat) rightX;
			v[1][1] = (GLfloat) rightZ ;
			v[1][2] = (GLfloat) rightY;
			
			v[2][0] = (GLfloat) apexX;
			v[2][1] = (GLfloat) apexZ ;
			v[2][2] = (GLfloat) apexY;
			
			calcNormal( v, out );
			glNormal3fv( out );
		}
*/

		// Perform polygon coloring based on a height sample
		//float fColor = (60.0f + leftZ) / 256.0f;
		//if ( fColor > 1.0f )  fColor = 1.0f;
		//glColor3f( fColor, fColor, fColor );

#if 0 // Temp reborn
		if( g_nCurrentRenderMode == 2 )
		{
			//glTexCoord2f( 0.0f, 0.0f );
			glTexCoord2f( 1.0f - ( leftXIndex / (float)sm_nSize ), leftYIndex / (float)sm_nSize );
		}
#endif

		// Output the LEFT VERTEX for the triangle
		glVertex3f(		(GLfloat) leftX,
						(GLfloat) leftZ,
						(GLfloat) leftY );

/*
		if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
			 gDrawMode == DRAW_USE_FILL_ONLY )
		{
			float fColor = (60.0f + rightZ) / 256.0f;
			if ( fColor > 1.0f )  fColor = 1.0f;
			glColor3f( fColor, fColor, fColor );
		}
*/

#if 0 // Temp reborn
		if( g_nCurrentRenderMode == 2 )
		{
			//glTexCoord2f( 1.0f, 0.0f );
			glTexCoord2f( 1.0f - ( rightXIndex / (float)sm_nSize ), rightYIndex / (float)sm_nSize );
		}
#endif

		// Output the RIGHT VERTEX for the triangle
		glVertex3f(		(GLfloat) rightX,
						(GLfloat) rightZ,
						(GLfloat) rightY );

/*
		if ( gDrawMode == DRAW_USE_TEXTURE ||	// Gaurad shading based on height samples instead of light normal
			 gDrawMode == DRAW_USE_FILL_ONLY )
		{
			float fColor = (60.0f + apexZ) / 256.0f;
			if ( fColor > 1.0f )  fColor = 1.0f;
			glColor3f( fColor, fColor, fColor );
		}
*/

#if 0 // Temp reborn
		if( g_nCurrentRenderMode == 2 )
		{
			//glTexCoord2f( 0.5f, 1.0f );
			glTexCoord2f( 1.0f - ( apexXIndex / (float)sm_nSize ), apexYIndex / (float)sm_nSize );
		}
#endif

		// Output the APEX VERTEX for the triangle
		glVertex3f(		(GLfloat) apexX,
						(GLfloat) apexZ,
						(GLfloat) apexY );
	}

	return nTrianglesRendered;
}

// ---------------------------------------------------------------------
// Computes Variance over the entire tree.  Does not examine node relationships.
//
unsigned short Patch::RecursComputeVariance( int leftX,  int leftY,  unsigned short leftZ,
										    int rightX, int rightY, unsigned short rightZ,
											int apexX,  int apexY,  unsigned short apexZ,
											int node)
{
	//        /|\
	//      /  |  \
	//    /    |    \
	//  /      |      \
	//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
	//
	int centerX = (leftX + rightX) >>1;		// Compute X coordinate of center of Hypotenuse
	int centerY = (leftY + rightY) >>1;		// Compute Y coord...
	unsigned short myVariance;

	// Get the height value at the middle of the Hypotenuse
	unsigned short centerZ  = m_HeightMap[(centerY * sm_nMapSize) + centerX];

	// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
	// Use values passed on the stack instead of re-accessing the Height Field.
	myVariance = Math::FAbs((int)centerZ - (((int)leftZ + (int)rightZ)>>1));

	// Since we're after speed and not perfect representations,
	//    only calculate variance down to an 8x8 block
	if ( (Math::FAbs(leftX - rightX) >= 8) ||
		 (Math::FAbs(leftY - rightY) >= 8) )
	{
		// Final Variance for this node is the max of it's own variance and that of it's children.
		myVariance = MAX( myVariance, RecursComputeVariance( apexX,   apexY,  apexZ, leftX, leftY, leftZ, centerX, centerY, centerZ,    node<<1 ) );
		myVariance = MAX( myVariance, RecursComputeVariance( rightX, rightY, rightZ, apexX, apexY, apexZ, centerX, centerY, centerZ, 1+(node<<1)) );
	}

	// Store the final variance for this node.  Note Variance is never zero.
	if (node < (1<<VARIANCE_DEPTH))
		m_CurrentVariance[node] = 1 + myVariance;

	return myVariance;
}

// -------------------------------------------------------------------------------------------------
//	PATCH CLASS
// -------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Initialize a patch.
//
void Patch::Init( Landscape* pLandscape, int nHeightMapIndexX, int nHeightMapIndexY, unsigned short *hMap )
{
	assert( pLandscape );
	m_pLandscape = pLandscape;

	// Clear all the relationships
	m_BaseLeft.pRightNeighbor = m_BaseLeft.pLeftNeighbor = m_BaseRight.pRightNeighbor = m_BaseRight.pLeftNeighbor =
	m_BaseLeft.pLeftChild = m_BaseLeft.pRightChild = m_BaseRight.pLeftChild = m_BaseLeft.pLeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.pBaseNeighbor = &m_BaseRight;
	m_BaseRight.pBaseNeighbor = &m_BaseLeft;


	// Store Patch offsets for the world and heightmap.
	m_WorldX = nHeightMapIndexX * sm_nSize;
	m_WorldY = nHeightMapIndexY * sm_nSize;

	// Store pointer to first byte of the height data for this patch.
	m_HeightMap = &hMap[m_WorldY * sm_nMapSize + m_WorldX];

	// Initialize flags
	m_VarianceDirty = 1;
	m_isVisible = 0;
}

// ---------------------------------------------------------------------
// Reset the patch.
//
void Patch::Reset()
{
	// Assume patch is not visible.
	m_isVisible = 0;

	// Reset the important relationships
	m_BaseLeft.pLeftChild = m_BaseLeft.pRightChild = m_BaseRight.pLeftChild = m_BaseLeft.pLeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.pBaseNeighbor = &m_BaseRight;
	m_BaseRight.pBaseNeighbor = &m_BaseLeft;

	// Clear the other relationships.
	m_BaseLeft.pRightNeighbor = m_BaseLeft.pLeftNeighbor = m_BaseRight.pRightNeighbor = m_BaseRight.pLeftNeighbor = NULL;
}

// ---------------------------------------------------------------------
// Compute the variance tree for each of the Binary Triangles in this patch.
//
void Patch::ComputeVariance()
{
	// Compute variance on each of the base triangles...

	m_CurrentVariance = m_VarianceLeft;
	RecursComputeVariance(	0,          sm_nSize, m_HeightMap[sm_nSize * sm_nMapSize],
							sm_nSize, 0,          m_HeightMap[sm_nSize],
							0,          0,          m_HeightMap[0],
							1);

	m_CurrentVariance = m_VarianceRight;
	RecursComputeVariance(	sm_nSize, 0,          m_HeightMap[ sm_nSize],
							0,          sm_nSize, m_HeightMap[ sm_nSize * sm_nMapSize],
							sm_nSize, sm_nSize, m_HeightMap[(sm_nSize * sm_nMapSize) + sm_nSize],
							1);

	// Clear the dirty flag for this patch
	m_VarianceDirty = 0;
}

// ---------------------------------------------------------------------
// Discover the orientation of a triangle's points:
//
// Taken from "Programming Principles in Computer Graphics", L. Ammeraal (Wiley)
//
inline int orientation( int pX, int pY, int qX, int qY, int rX, int rY )
{
	int aX, aY, bX, bY;
	float d;

	aX = qX - pX;
	aY = qY - pY;

	bX = rX - pX;
	bY = rY - pY;

	d = (float)aX * (float)bY - (float)aY * (float)bX;
	return (d < 0) ? (-1) : (d > 0);
}

// ---------------------------------------------------------------------
// Set patch's visibility flag.
//
void Patch::SetVisibility( const Vector3& vPosition, const Vector3& vOrientation )
{
	// Get patch's center point
	float patchCenterX = ( m_WorldX + sm_nSize / 2 ) * sm_fPlanarScalar;
	float patchCenterY = ( m_WorldY + sm_nSize / 2 ) * sm_fPlanarScalar;

	Vector3 patchToPosition = vPosition - Vector3( patchCenterX, 0.0f, patchCenterY );
	
	m_isVisible = vOrientation.Dot( patchToPosition ) < 0.0f;
}

// ---------------------------------------------------------------------
// Create an approximate mesh.
//
void Patch::Tessellate( const Vector3& vCameraPosition, float fFrameVariance )
{
	// Split each of the base triangles
	m_CurrentVariance = m_VarianceLeft;
	RecursTessellate (	&m_BaseLeft,
						vCameraPosition,
						fFrameVariance,
						m_WorldX,				m_WorldY+sm_nSize,
						m_WorldX+sm_nSize,	m_WorldY,
						m_WorldX,				m_WorldY,
						1 );
					
	m_CurrentVariance = m_VarianceRight;
	RecursTessellate(	&m_BaseRight,
						vCameraPosition,
						fFrameVariance,
						m_WorldX+sm_nSize,	m_WorldY,
						m_WorldX,				m_WorldY+sm_nSize,
						m_WorldX+sm_nSize,	m_WorldY+sm_nSize,
						1 );
}

// ---------------------------------------------------------------------
// Render the mesh.
//
int Patch::Render()
{
	int nTrianglesRendered = 0;

	// Store old matrix
	glPushMatrix();
	
	// Translate the patch to the proper world coordinates
	glTranslatef( m_WorldX * sm_fPlanarScalar, 0, m_WorldY * sm_fPlanarScalar );

	glBegin( GL_TRIANGLES );

	nTrianglesRendered += RecursRender( &m_BaseLeft, 0,	sm_nSize, sm_nSize,	0, 0, 0 );
		
	nTrianglesRendered += RecursRender(	&m_BaseRight, sm_nSize,	0, 0, sm_nSize,	sm_nSize, sm_nSize );
	
	glEnd();
	
	// Restore the matrix
	glPopMatrix();

	return nTrianglesRendered;
}


