// LANDSCAPE.HPP

// A class that manages a block of landscape data

// TODO: Add support for linking multiple Landscape objects together

#ifndef _LANDSCAPE_HPP_
#define _LANDSCAPE_HPP_

#include "Patch.hpp"

// ####################################### CONSTANTS #############################################

// How many TreeNodes should be allocated?
const unsigned int kPoolSize = 25000;

// This value stabilizes the tessellation.
// Every frame the system attempts to match the target tessellation value.
// When the variance is zero, it attempts an exact match which is unlikely, therefore
// the tessellation would continue to change.  As you increase this number, it becomes easier to
// satisfy the target, but the target count becomes more inaccurate.
const unsigned int kTessellationVariance = 50;	

// Beginning frame varience (should be high, it will adjust automatically)
const unsigned int kBeginningFrameVariance = 50;

// Desired number of Binary Triangle tessellations per frame.
// This is not the desired number of triangles rendered!
// There are usually twice as many Binary Triangle structures as there are rendered triangles.
const unsigned int kDesiredTessellations = 20000;

// #############################################################################################

class Vector3;

class Landscape
{
public:
	Landscape();
	~Landscape();

	// A landscape object manages the memory used by the Patches
	TreeNode* AllocateTreeNode();

	// Initializes the Landscape object by destroying any old data and loading new heightmap
	// Also generates all the Patch objects needed to represent the terrain
	// The scale is applied to the XZ planar values to allow proper matching to the heightmap values
	bool Init( const char* pFileName, float fXZScale, float yScale );

	// Resets all the patches, recalculates their visibility, and then recomputes their variance if needed
	void Reset( const Vector3& vPosition, const Vector3& vOrientation );

	// Tessellates all visible patch objects
	void Tessellate( const Vector3& vCameraPosition );

	// Render all visible patch objects and adjusts the frame variance if needed
	void Render();

	// Returns the number of triangles rendered on the last frame rendered
	int GetNumberOfTrianglesRendered() { return m_nTrianglesRendered; }

	// Return the landscape height at the given world coordinates
	float GetHeight( float fWorldPositionX, float fWorldPositionY );

private:
	// Helper function for cleaning up allocations
	void CleanUp();

	// The number of patches each dimension is broken down into
	// A n x n heightmap will be broken down into m x m patches
	int m_nNumberOfPatches;

	// The patch data stored as a two dimensional array
	Patch** m_aPatches;

	// Index to next free TreeNode
	unsigned int m_nNextTreeNode;

	// The pool of TreeNode's managed by the Landscape object 
	TreeNode m_aTreeNodePool[ kPoolSize ];

	// The height map data stored as an array
	unsigned short* m_anHeightMap;

	// These store the visibility information for each frame
	int m_nClipperRightX;
	int m_nClipperRightY;
	int m_nClipperLeftX;
	int m_nClipperLeftY;
	int m_nClipperEyeX;
	int m_nClipperEyeY;

	float m_fFrameVariance;
	int m_nDesiredTessellations;
	int m_nTrianglesRendered;
	int m_nMapSize;

	float m_fHeightMapXZScale;
	float m_fHeightMapYScale;
};


#endif // _LANDSCAPE_HPP_