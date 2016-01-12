// TREENODE.HPP

#ifndef _TREENODE_HPP_
#define _TREENODE_HPP_

// A simple node class for storing triangle tree data

struct TreeNode
{
	TreeNode* pLeftChild;
	TreeNode* pRightChild;
	TreeNode* pBaseNeighbor;
	TreeNode* pLeftNeighbor;
	TreeNode* pRightNeighbor;
};

#endif // _TREENODE_HPP_