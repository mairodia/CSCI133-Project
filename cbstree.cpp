// ============================================================================
// File: cbstree.cpp (Fall 2018)
// ============================================================================
// This file contains the implementation of the CBSTree class. It uses the
// template parameter "NodeType" for the type of values that are stored in the
// tree.
// ============================================================================

#include    <fstream>
#include    <iostream>
#include    <cstdlib>
using namespace std;
#include    "cbstree.h"


// ==== CBSTree::CBSTree ======================================================
//
// This is the copy constructor for the CBSTree class, it just makes a call to
// the CopyTree member function and saves the return value in the root member
// of the calling object.
//
// Access: public
//
// Input:
//      other [IN]  -- a constant reference to a CBSTree object.
//
// ============================================================================

template    <typename  NodeType>
CBSTree<NodeType>::CBSTree(const CBSTree<NodeType>  &other)
{
    CopyTree(other.m_root);

}  // end of "CBSTree<NodeType>::CBSTree"



// ==== CBSTree::CopyTree =====================================================
//
// This recursive function creates a copy of a CBSTree. It receives a pointer
// to the source tree's root and uses a preorder traversal to make recursive
// calls and create a copy of the tree, and then returns a pointer to the root
// of the new copy.
//
// Access: private
//
// Input:
//      sourcePtr [IN]          -- a pointer to the source tree's root
//
//  Output:
//      A pointer to the root of the copied tree.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*    CBSTree<NodeType>::CopyTree(
                                        const CTreeNode<NodeType>  *sourcePtr)
{
    if(sourcePtr)
    {
        InsertItem(sourcePtr->m_value);
        return sourcePtr;
    }
    if(sourcePtr->m_left)
    {
        CopyTree(sourcePtr->m_left);
    }
    if(sourcePtr->m_right)
    {
        CopyTree(sourcePtr->m_right);
    }

}  // end of "CBSTree<NodeType>::CopyTree"



// ==== CBSTree::CountNodes ===================================================
//
// This recursive function derives the current height and number of nodes in
// the tree.  The height is a zero-based integer value, which represents the
// length of the longest path from the root to a leaf (counting the edges, not
// the nodes).  This function is called by public function CBSTree::GetTreeInfo
// so that the caller may determine the total number of nodes and the height of
// the tree.
//
// Access: protected
//
// Input:
//      nodePtr [IN]        -- a pointer to a tree node; initially this is the
//                             root
//
//      currDepth [IN]      -- the depth of the parent node (initial value is
//                             negative one, set by the caller)
//
//      numNodes [IN/OUT]   -- a reference to an int that will contain the
//                             total number of nodes in the tree (initial value
//                             is zero, set by the caller)
//
// Output:
//      The length of the longest path from the root.
//
// ============================================================================

template    <typename  NodeType>
int     CBSTree<NodeType>::CountNodes(const CTreeNode<NodeType>  *nodePtr
                                                        , int  currDepth
                                                        , int  &numNodes )const
{
    if(nodePtr == NULL)
    {
        numNodes = 0;
        return 0;
    }
    else if(nodePtr->left==NULL && nodePtr->right == NULL)
    {
        numNodes++;
        currDepth++;
        return 1;
    }
    else
    {
        numNodes = CountNodes(nodePtr->left, currDepth, &numNodes) + CountNodes(nodePtr->right, currDepth, &numNodes);
        return currDepth;
    }

}  // end of "CBSTree::CountNodes"



// ==== CBSTree::Delete =======================================================
//
// This function deletes a target node from the tree.  The function finds the
// correct location for the target node by calling itself recursively. It then
// returns the address of the (potentially new) root of the tree.
//
// Access: protected
//
// Input:
//      target [IN]         -- a const reference to a NodeType item that
//                             contains the target search key value
//
//      nodePtr [IN]        -- a pointer to a tree node (initially this is
//                             the root).
//
//      bItemDeleted [OUT]  -- a reference to a bool that will indicate if the
//                             target item was actually removed from the tree;
//                             if that's the case it will have a value of true,
//                             else it will have a value of false
//
// Output:
//      A pointer to the (potentially new) root of the tree.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::Delete(
                                            const NodeType  &target
                                            , CTreeNode<NodeType>  *nodePtr
                                            , bool  &bItemDeleted)
{
    CTreeNode child;
    CTreeNode *temp;
    temp = NULL;
    child->m_right = temp;
    child->m_left = temp;

    if(nodePtr == NULL)
    {
        bItemDeleted = false;
        return NULL;
    }

    if(target < nodePtr->m_value)
    {
        nodePtr->left = Delete(target, nodePtr->left);
    }
    else if(target > nodePtr->m_value)
    {
        nodePtr->right = Delete(target, nodePtr->right);
    }
    else
    {
        if(nodePtr->m_right && nodePtr->m_left)
        {
            temp = FindMinNode(nodePtr->m_right);
            nodePtr->m_value = temp->m_value;
            nodePtr->m_right = Delete(nodePtr->m_value, nodePtr->m_right);
        }
        else
        {
            if(nodePtr->m_right && nodePtr->m_left == NULL)
            {
                child = nodePtr->m_right;
            }
            else
            {
                child = nodePtr->m_left;
            {
            delete nodePtr;
            bItemDeleted = true;
            return child;
        }
        bItemDeleted = true;
        return nodePtr;
    }
    bItemDeleted = false;

}  // end of "CBSTree<NodeType>::Delete"



// ==== CBSTree::DeleteItem ===================================================
//
// This function allows the caller to delete a target node from the tree.
//
// Access: public
//
// Input:
//      target [IN]      -- a const reference to a NodeType object
//
// Output:
//      A value of false if the target item is not in the tree, otherwise a
//      value of true is returned and the item is removed from the tree.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::DeleteItem(const NodeType  &target)
{
    bool bItemDeleted = false;
    Delete(target, m_root, &bItemDeleted);
    return bItemDeleted;
}  // end of "CBSTree<NodeType>::DeleteItem"



// ==== CBSTree::DestroyNodes =================================================
//
// This function performs a recursive postorder descent down the tree,
// releasing all allocated memory.
//
// Access: protected
//
// Input:
//      nodePtr [IN/OUT]    -- a pointer to a tree node (initially the root)
//
// Output:
//      Nothing.
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::DestroyNodes(CTreeNode<NodeType>  *const nodePtr)
{
    CTreeNode *temp;
    temp = nodePtr;

    if(temp->m_left == NULL && temp->m_right == NULL)
    {
        free(temp);
        return NULL;
    }
    else
    {
        temp->left = DestroyNodes(nodePtr->left);
        temp->right = DestroyNodes(nodePtr->right);
    }
    return nodePtr;

}  // end of "CBSTree<ItemType>::DestroyNodes"



// ==== CBSTree::FindMinNode ==================================================
//
// This function finds the inorder successor of the node pointed to by the
// parameter, and returns a pointer to the inorder successor node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a tree node
//
// Output:
//      A pointer to the target node.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::FindMinNode(
                            CTreeNode<NodeType>  *nodePtr) const
{
    while(nodePtr->m_left != NULL)
    {
        nodePtr = nodePtr->m_left;
    }
    return nodePtr;

}  // end of "CBSTree<NodeType>::FindMinNode"



// ==== CBSTree::GetTreeInfo ==================================================
//
// This function allows the caller to get the current number of nodes and the
// height of the tree by calling the CBSTree::CountNodes member function.
//
// Access: public
//
// Input:
//      numNodes [OUT]  -- a reference to an int that will contain the total
//                         number of nodes currently in the tree
//
//      height [OUT]    -- a reference to an int that will contain the height
//                         of the tree; this is a zero-based value that
//                         represents the longest path from the root to a leaf
//                         (counting edges, not the nodes)
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::GetTreeInfo(int  &numNodes, int  &height) const
{
    ???

}  // end of "CBSTree::GetTreeInfo"



// ==== CBSTree::InOrder ======================================================
//
// This function performs an in-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a tree node (this is a recursive
//                         function, initially this points to the root).
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a
//                         const reference to a NodeType object as input, and
//                         returns nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::InOrder(const CTreeNode<NodeType> *const nodePtr
                                        , void (*fPtr)(const NodeType&)) const
{


}  // end of "CBSTree<NodeType>::InOrder"



// ==== CBSTree::InOrderTraverse ==============================================
//
// This function allows the caller to execute an in-order traversal through the
// tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const
//                     reference to a NodeType object as input, and returns
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::InOrderTraverse(void  (*fPtr)(const NodeType&)) const
{
    ???

}  // end of "CBSTree<NodeType>::InOrderTraverse"



// ==== CBSTree::Insert =======================================================
//
// This function inserts a new node into the tree.  It finds the correct
// location for the new node by calling itself recursively. If the new item
// is unique, a copy is created and inserted into the tree. Then the address of
// the (potentially new) root of the tree is returned.
//
// If the root data member of this class is NULL upon entry, it is updated
// with the value of the nodePtr parameter.
//
// Access: protected
//
// Input:
//      newItem [IN]    -- a const reference to a NodeType object; it is
//                         assumed that the object is initialized and ready to
//                         be inserted
//
//      nodePtr [IN]    -- a pointer to a tree node (initially this is usually
//                         the root)
//
// Output:
//      A pointer to the (potentially new) root of the tree
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::Insert(const NodeType  &newItem
                                            , CTreeNode<NodeType>  *nodePtr)
{
    if(nodePtr == NULL)
    {
        nodePtr = new CTreeNode;
        nodePtr->m_value = newItem;
    }
    else
    {
        if(newItem < nodePtr->m_value)
        {
            nodePtr->m_left = Insert(newItem, nodePtr->left);
        }
        else if(newItem > nodePtr->m_value)
        {
            nodePtr->m_right = Insert(newItem, nodePtr->m_right);
        }
    }

    return nodePtr;

}  // end of "CBSTree<NodeType>::Insert"



// ==== CBSTree::InsertItem ===================================================
//
// This function allows the caller to insert a new node into the tree.  The
// input parameter is a const reference to the item to insert.
//
// Access: public
//
// Input:
//      newItem [IN]    -- a const reference a NodeType object
//
// Output:
//      A value of true if the item was successfully inserted into the tree,
//      false otherwise.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::InsertItem(const NodeType  &newItem)
{
    if(Insert(newItem, m_root) == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }

}  // end of "CBSTree<NodeType>::InsertItem"



// ==== CBSTree::ItemInTree ===================================================
//
// This function allows the caller to determine if a target item is in the
// tree. The input parameter is a const reference to the target tree node
// value, and this function calls CBSTree::Retrieve to determine if it's in the
// tree or not.
//
// Access: public
//
// Input:
//      target [IN]     -- a const reference to a NodeType object that contains
//                         the target key value to search for
//
// Output:
//      A value of true if the target item is found, false if not.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::ItemInTree(const NodeType  &target) const
{


}  // end of "CBSTree<NodeType>::ItemInTree"



// ==== CBSTree::PostOrder ====================================================
//
// This function performs a post-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a CTreeNode (this is a recursive
//                         function, initially this points to the root)
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a
//                         const reference to a NodeType object as input and
//                         returns nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PostOrder(const CTreeNode<NodeType>  *const nodePtr
                                        , void (*fPtr)(const NodeType&)) const
{
    ???

}  // end of "CBSTree<NodeType>::PostOrder"



// ==== CBSTree::PostOrderTraverse ============================================
//
// This function allows the caller to execute a post-order traversal through
// the tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const
//                     reference to a NodeType object as input, and returns
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PostOrderTraverse(void  (*fPtr)(const NodeType&)) const
{
    ???

}  // end of "CBSTree<NodeType>::PostOrderTraverse"



// ==== CBSTree::PreOrder =====================================================
//
// This function performs a pre-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a CTreeNode (this is a recursive
//                         function, initially this points to the root)
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a
//                         const reference NodeType object as input, and returns
//                         nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PreOrder(const CTreeNode<NodeType>  *const nodePtr
                                    , void  (*fPtr)(const NodeType&)) const
{
    ???

}  // end of "CBSTree<NodeType>::PreOrder"



// ==== CBSTree::PreOrderTraverse =============================================
//
// This function allows the caller to execute a pre-order traversal through the
// tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const
//                     reference to a NodeType object as input, and returns
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PreOrderTraverse(void (*fPtr)(const NodeType&)) const
{
    ???

}  // end of "CBSTree<NodeType>::PreOrderTraverse"



// ==== CBSTree::RebalanceTree ================================================
//
// This function rebalances the tree to an optimal height. It first derives the
// number of nodes in the tree, then allocates a dynamic array to store all of
// the tree's values. After that it saves the tree's values to the dynamic
// array by calling CBSTree::SaveToArray, then releases all of the nodes in the
// tree. Next, it calls the CBSTree::Repopulate function to repopulate the tree
// with the array before releasing the array back to the heap.
//
// Access: public
//
// Input:
//      Nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void        CBSTree<NodeType>::RebalanceTree()
{
    ???

}  // end of "CBSTree<NodeType>::RebalanceTree"



// ==== CBSTree::Repopulate ===================================================
//
// This function uses the contents of a sorted array to repopulate the tree.
// The array is processed using 'divide and conquer' recursion so that the
// middle element of the array is inserted into the tree, resulting in a
// balanced binary tree.
//
// Access: protected
//
// Input:
//      array [IN]      -- the base address of the array; it is assumed that
//                         the values in the array are in sorted ascending order
//
//      first [IN]      -- an index to the first element
//
//      last [IN]       -- an index to the last element
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void        CBSTree<NodeType>::Repopulate(const NodeType array[], int first
                                                                , int last)
{
    ???

}  // end of "CBSTree<NodeType>::Repopulate"



// ==== CBSTree::Retrieve =====================================================
//
// This function finds the node in the tree whose value equals that of the
// tree node reference parameter. The target node is located through recursive
// function calls. If the node does not exist in the tree, a value of NULL is
// returned.
//
// Access: public
//
// Input:
//      target [IN]     -- a reference to a NodeType object; it is assumed that
//                         the object is fully initialized so a search can be
//                         performed
//
//      nodePtr [IN]    -- a pointer to a tree node (initially this is usually
//                         the root)
//
// Output:
//      If a tree node is found that has the same value as the target
//      parameter, then a pointer to the node in the tree is returned.
//      Otherwise, a value of NULL is returned.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::Retrieve(const NodeType  &target
                                          , CTreeNode<NodeType>  *nodePtr) const
{
    if(nodePtr == NULL)
    {
        return NULL;
    }

    if(target < nodePtr->m_value)
    {
        nodePtr = Retrieve(target, nodePtr->m_left);
    }
    else if(target > nodePtr->m_value)
    {
        nodePtr = Retrieve(target, nodePtr->m_right);
    }

    return nodePtr;

}  // end of "CBSTree<NodeType>::Retrieve"



// ==== CBSTree::SaveToArray ==================================================
//
// This function does an inorder traversal of the tree so that the values in
// the nodes can be written to the caller's array in sorted ascending order.
//
// This function performs an inorder traversal of the tree making recursive
// calls so that the values in the nodes can be written to the caller's array
// in sorted ascending order.
//
// Access: protected
//
// Input:
//      nodePtr[IN]     -- a ptr to a tree node, initially the root
//
//      array [IN]      -- the base address of the caller's array
//
//      index [IN/OUT]  -- a reference to the an int to be used for the index
//                         into the array; as node values are copied into the
//                         array, this index is incremented so the other
//                         recursive stack frames immediately have the updated
//                         index value to access the next element in the sorted
//                         array
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::SaveToArray(const CTreeNode<NodeType> *const nodePtr
                                                            , NodeType array[]
                                                            , int &index)
{


}  // end of "CBSTree<NodeType>::SaveToArray"



// ==== CBSTree::operator= ====================================================
//
// This is the overloaded assignment operator for the CBSTree class. It first
// checks for assignment to self, then releases all of the nodes in the calling
// object. Then it uses the parameter as an argument to the CopyTree member
// function so that the parameter's tree is replicated in the calling object.
//
// Access: public
//
// Input:
//      rhs [IN]    -- a const reference to an existing CBSTree object
//
// Output:
//      A reference to the calling object.
//
// ============================================================================

template    <typename  NodeType>
CBSTree<NodeType>&  CBSTree<NodeType>::operator=(const CBSTree<NodeType> &rhs)
{
    if(this != &rhs)
    {
        DestroyTree();
        m_root = CopyTree(rhs.m_root);
    }
    return *this;

}  // end of "CBSTree<NodeType>::operator="
