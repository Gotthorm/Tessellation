// LANDSCAPE.CPP

#include <windows.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include <TCHAR.H>
//#include "GLUT\glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Common\Math.hpp"
#include "Common\Vector3.hpp"
#include "HeightMap.hpp"
#include "TreeNode.hpp"
#include "landscape.hpp"

// TODO: Get rid of this constant and make this class handle scaling properly
#define MULT_SCALE (1.0f)


Landscape::Landscape() : m_anHeightMap( NULL ), m_aPatches( NULL ), m_nNumberOfPatches( 0 )
{
}

Landscape::~Landscape()
{
	CleanUp();
}

void Landscape::CleanUp()
{
	// Clean up all memory allocations here

	delete [] m_anHeightMap;
	m_anHeightMap = NULL;

    for( int nIndex = 0; nIndex < m_nNumberOfPatches; ++nIndex )
	{
        delete [] m_aPatches[ nIndex ];
	}

    delete [] m_aPatches;
	m_aPatches = NULL;
	m_nNumberOfPatches = 0;
}

TreeNode *Landscape::AllocateTreeNode()
{
	TreeNode *pTreeNode;

	if( m_nNextTreeNode >= kPoolSize )
	{
		return NULL;
	}

	pTreeNode = &( m_aTreeNodePool[ m_nNextTreeNode++ ] );
	pTreeNode->pLeftChild = pTreeNode->pRightChild = NULL;

	return pTreeNode;
}

bool Landscape::Init( const char* pFileName, float fXZScale, float yScale  )
{
	// Deallocates height map data and patch data
	CleanUp();

//	LoadGLTextures();

	HeightMap theHMDFile;

	if( theHMDFile.Load( pFileName ) == false )
	{
		return false;
	}

	m_nMapSize = theHMDFile.GetWidthX();
	assert( m_nMapSize == theHMDFile.GetWidthZ() );

	// This Landscape system relies on the data having one extra row and column
	// of height points, to allow the terrain data to be split into an even number
	// of patches of identical size.

	// This extra data will eventually be the next landscape object's edge, but
	// for now we will simply duplicate the data in the last row and column.

	// The data size is larger by one column and row (see comment above)
	int nDataSize = m_nMapSize + 1;

	assert( m_anHeightMap == NULL );
	m_anHeightMap = new unsigned short[ nDataSize * nDataSize ];
	unsigned short* pSourceData = (unsigned short*)( theHMDFile.GetData() );

	// Now carefully copy the data
	unsigned short* pTargetData = m_anHeightMap;

	memcpy( pTargetData, pSourceData, m_nMapSize * sizeof( unsigned short ) );
	pTargetData[ m_nMapSize ] = pSourceData[ m_nMapSize - 1 ];
	pTargetData += ( m_nMapSize + 1 );

	for( int X = 0; X < m_nMapSize; X++ )
	{
		memcpy( pTargetData, pSourceData, m_nMapSize * sizeof( unsigned short ) );
		pTargetData[ m_nMapSize ] = pSourceData[ m_nMapSize - 1 ];
		pTargetData += ( m_nMapSize + 1 );
		pSourceData += m_nMapSize;
	}

	// Generate a size for the number of patches
	m_nNumberOfPatches = m_nMapSize >> 6;
	if( m_nNumberOfPatches <= 0 )
	{
		m_nNumberOfPatches = 1;
	}

	// Notify the Patch class the map size and patch size
	Patch::SetMapSize( m_nMapSize );
	Patch::SetPatchSize( m_nMapSize / m_nNumberOfPatches );

	// Allocate the Patch objects
	assert( m_aPatches == NULL );
	m_aPatches = new Patch*[ m_nNumberOfPatches ];
    for( int nIndex = 0; nIndex < m_nNumberOfPatches; ++nIndex )
	{
        m_aPatches[ nIndex ] = new Patch[ m_nNumberOfPatches ];
	}

	// Init the tessellation state variables
	m_fFrameVariance = kBeginningFrameVariance;
	m_nDesiredTessellations = kDesiredTessellations;

	m_nTrianglesRendered = 0;

	m_fHeightMapXZScale = fXZScale;
	m_fHeightMapYScale = yScale;

	Patch::SetRenderScale( fXZScale, yScale );

	// Initialize all Patches
	for( int Y = 0; Y < m_nNumberOfPatches; Y++ )
	{
		for( int X = 0; X < m_nNumberOfPatches; X++ )
		{
			m_aPatches[ Y ][ X ].Init( this, X , Y, m_anHeightMap );
			m_aPatches[ Y ][ X ].ComputeVariance();
		}
	}

	return true;
}

void Landscape::Reset( const Vector3& vPosition, const Vector3& vOrientation )
{
	// Perform simple visibility culling on entire patches.

	Vector3 adjustedOrientation = Vector3( vOrientation.X, 0.0f, vOrientation.Z );
	adjustedOrientation.Normalize();

	Vector3 adjustedPosition = Vector3( vPosition.X, 0.0f, vPosition.Z );
	adjustedPosition -= ( adjustedOrientation * Patch::GetPatchSizeInWorld() );

	// Set the next free triangle pointer back to the beginning
	// essentially resetting the memory management
	m_nNextTreeNode = 0;

	// Reset rendered triangle count.
	m_nTrianglesRendered = 0;

	Patch *pPatch;

	// Go through the patches performing resets, compute variances, and linking.
	for( int Y = 0; Y < m_nNumberOfPatches; Y++ )
	{
		for( int X = 0; X < m_nNumberOfPatches; X++ )
		{
			pPatch = &( m_aPatches[ Y ][ X ] );
			
			// Reset the patch
			pPatch->Reset();
			//pPatch->SetVisibility( m_nClipperEyeX, m_nClipperEyeY, m_nClipperLeftX, m_nClipperLeftY, m_nClipperRightX, m_nClipperRightY );
			pPatch->SetVisibility( adjustedPosition, adjustedOrientation );
			
			// Check to see if this patch has been deformed since last frame.
			// If so, recompute the variance tree for it.
			if( pPatch->isDirty() )
			{
				pPatch->ComputeVariance();
			}

			if( pPatch->isVisibile() )
			{
				// Link all the patches together.
				if( X > 0 )
				{
					pPatch->GetBaseLeft()->pLeftNeighbor = m_aPatches[ Y ][ X - 1 ].GetBaseRight();
				}
				else
				{
					// Link to bordering Landscape here..
					pPatch->GetBaseLeft()->pLeftNeighbor = NULL;
				}

				if( X < ( m_nNumberOfPatches - 1 ) )
				{
					pPatch->GetBaseRight()->pLeftNeighbor = m_aPatches[ Y ][ X + 1 ].GetBaseLeft();
				}
				else
				{
					// Link to bordering Landscape here..
					pPatch->GetBaseRight()->pLeftNeighbor = NULL;
				}

				if( Y > 0 )
				{
					pPatch->GetBaseLeft()->pRightNeighbor = m_aPatches[ Y - 1 ][ X ].GetBaseRight();
				}
				else
				{
					// Link to bordering Landscape here..
					pPatch->GetBaseLeft()->pRightNeighbor = NULL;
				}

				if( Y < ( m_nNumberOfPatches - 1 ) )
				{
					pPatch->GetBaseRight()->pRightNeighbor = m_aPatches[ Y + 1 ][ X ].GetBaseLeft();
				}
				else
				{
					// Link to bordering Landscape here..
					pPatch->GetBaseRight()->pRightNeighbor = NULL;
				}
			}
		}
	}
}

void Landscape::Tessellate( const Vector3& vCameraPosition )
{
	// Perform Tessellation
	for( int Y = 0; Y < m_nNumberOfPatches; Y++ )
	{
		for( int X = 0; X < m_nNumberOfPatches; X++ )
		{
			if( m_aPatches[ Y ][ X ].isVisibile() )
			{
				m_aPatches[ Y ][ X ].Tessellate( vCameraPosition, m_fFrameVariance );
			}
		}
	}
}

void Landscape::Render()
{
	// Scale the terrain by the terrain scale specified at compile time.
	glScalef( 1.0f, MULT_SCALE, 1.0f );

	for( int Y = 0; Y < m_nNumberOfPatches; Y++ )
	{
		for( int X = 0; X < m_nNumberOfPatches; X++ )
		{
			if( m_aPatches[ Y ][ X ].isVisibile() )
			{
				m_nTrianglesRendered += m_aPatches[ Y ][ X ].Render();
			}
		}
	}

	// Check to see if we got close to the desired number of triangles.
	// Adjust the frame variance to a better value.
	if( m_nNextTreeNode < ( m_nDesiredTessellations - kTessellationVariance ) || m_nNextTreeNode > ( m_nDesiredTessellations + kTessellationVariance ) )
	{
		m_fFrameVariance += ((int)m_nNextTreeNode - m_nDesiredTessellations) / (float)m_nDesiredTessellations;
	}

	// Bounds checking.
	if ( m_fFrameVariance < 0.0f )
	{
		m_fFrameVariance = 0.0f;
	}
}

float Landscape::GetHeight( float fWorldPositionX, float fWorldPositionZ )
{
	// Using bilinear interpolation, calculate the height at an arbritrary point
	//
	// (0,0) A-------B (1,0)
    //       |       |
    //       |   P   |
    //       |       |
	// (0,1) C-------D (1,1)

	float mapPositionX = fWorldPositionX / m_fHeightMapXZScale;
	float mapPositionZ = fWorldPositionZ / m_fHeightMapXZScale;

	if( mapPositionX < 0.0f || mapPositionX > (float)m_nMapSize || mapPositionZ < 0.0f || mapPositionZ > (float)m_nMapSize )
	{
		return 0.0f;
	}

	int nWorldX = (int)mapPositionX;
	int nWorldZ = (int)mapPositionZ;

	float fFractionalX = mapPositionX - (float)nWorldX;
	float fFractionalZ = mapPositionZ - (float)nWorldZ;

// TODO : Apply the height scalar

	float nHeightA = m_fHeightMapYScale * m_anHeightMap[ nWorldZ * m_nMapSize + nWorldX ];
	float nHeightB = m_fHeightMapYScale * m_anHeightMap[ nWorldZ * m_nMapSize + nWorldX + 1 ];
	float nHeightC = m_fHeightMapYScale * m_anHeightMap[ ( nWorldZ + 1 ) * m_nMapSize + nWorldX ];
	float nHeightD = m_fHeightMapYScale * m_anHeightMap[ ( nWorldZ + 1 ) * m_nMapSize + nWorldX + 1 ];

	return ( ( nHeightA * ( 1.0f - fFractionalX ) + nHeightB * fFractionalX ) * ( 1.0f - fFractionalZ ) + 
			( nHeightC * ( 1.0f - fFractionalX ) + nHeightD * fFractionalX ) * ( fFractionalZ ) ) * MULT_SCALE;
}



