
#ifndef PATCH_HPP
#define PATCH_HPP

#include "TreeNode.hpp"
#include "Common\Vector3.hpp"

// Depth of variance tree: should be near SQRT(PATCH_SIZE) + 1
#define VARIANCE_DEPTH (9)

// Predefines...
class Landscape;

//
// Patch Class
// Store information needed at the Patch level
//
class Patch
{
public:
	Patch();
	~Patch();

	static void SetPatchSize( int nSize ) { sm_nSize = nSize; }
	static void SetMapSize( int nSize ) { sm_nMapSize = nSize; }
	static void SetRenderScale( float scaleXZ, float scaleY ) { sm_fPlanarScalar = scaleXZ; sm_fYScalar = scaleY; }
	static int GetPatchSize() { return sm_nSize; }
	static int GetPatchSizeInWorld() { return int( sm_nSize * sm_fPlanarScalar ); }

	// Some encapsulation functions & extras
	TreeNode *GetBaseLeft()  { return &m_BaseLeft; }
	TreeNode *GetBaseRight() { return &m_BaseRight; }
	char isDirty()     { return m_VarianceDirty; }
	int  isVisibile( ) { return m_isVisible; }
	void SetVisibility( const Vector3& vPosition, const Vector3& vOrientation );

	void Init( Landscape* pLandscape, int nHeightMapIndexX, int nHeightMapIndexY, unsigned short *hMap );
	void Reset();
	void Tessellate( const Vector3& vCameraPosition, float fFrameVariance );
	int Render();
	void ComputeVariance();

	// The recursive half of the Patch Class
	void Split( TreeNode *tri);
	void RecursTessellate( TreeNode* pTreeNode, const Vector3& vCameraPosition, float fVariance, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY, int node );
	int RecursRender( TreeNode* pTreeNode, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY );
	unsigned short RecursComputeVariance(	int leftX,  int leftY,  unsigned short leftZ,
													int rightX, int rightY, unsigned short rightZ,
													int apexX,  int apexY,  unsigned short apexZ,
													int node);
private:
	Landscape* m_pLandscape;										// Pointer to the parent Landscape object

	unsigned short *m_HeightMap;								// Pointer to height map to use
	int m_WorldX, m_WorldY;										// World coordinate offset of this patch.

	unsigned short m_VarianceLeft[ 1<<(VARIANCE_DEPTH)];			// Left variance tree
	unsigned short m_VarianceRight[1<<(VARIANCE_DEPTH)];			// Right variance tree

	unsigned short *m_CurrentVariance;							// Which varience we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
	unsigned char m_VarianceDirty;								// Does the Varience Tree need to be recalculated for this Patch?
	unsigned char m_isVisible;									// Is this patch visible in the current frame?

	TreeNode m_BaseLeft;	// Left base triangle tree node
	TreeNode m_BaseRight;	// Right base triangle tree node

	static float sm_fPlanarScalar;
	static float sm_fYScalar;
	static int sm_nSize;				// Currently all patches operate at the same size
	static int sm_nMapSize;				// Currently all patches are controlled by a single landscape object
};


#endif
