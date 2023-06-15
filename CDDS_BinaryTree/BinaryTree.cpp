/*----------------------------------------
Author: Richard Stern
Description: A simple binary search tree
Date: 17/4/2015
----------------------------------------*/
#include "BinaryTree.h"
#include "TreeNode.h"
#include "raylib.h"
#include <iostream>
#include <cstdlib>
using namespace std;


BinaryTree::BinaryTree()
{
    m_pRoot = nullptr;
}

BinaryTree::~BinaryTree()
{
	while (m_pRoot != nullptr)
	{
		Remove(m_pRoot->GetData());
	}
}

// Return whether the tree is empty
bool BinaryTree::IsEmpty() const 
{ 
	return (m_pRoot == nullptr);
}

// Insert a new element into the tree.
// Smaller elements are placed to the left, larger onces are placed to the right.
void BinaryTree::Insert(int a_nValue)
{
	TreeNode *newNode = new TreeNode(a_nValue);

	if (IsEmpty())
	{
		m_pRoot = newNode;
		return;
	}

	TreeNode *current = m_pRoot;
	TreeNode *parent = nullptr;
	while (current != nullptr)
	{
		if (a_nValue < current->GetData())
		{
			parent = current;
			current = current->GetLeft();
			continue;
		}
		if (a_nValue > current->GetData())
		{
			parent = current;
			current = current->GetRight();
			continue;
		}
		if (a_nValue == current->GetData())
			current = nullptr;
	}

	if (parent != nullptr)
	{
		if (a_nValue < parent->GetData())
			parent->SetLeft(newNode);
		else
			parent->SetRight(newNode);
	}
}

TreeNode* BinaryTree::Find(int a_nValue)
{
	TreeNode* pCurrent = nullptr;
	TreeNode* pParent = nullptr;

	return FindNode(a_nValue, pCurrent, pParent) ? pCurrent : nullptr;
}

bool BinaryTree::FindNode(int a_nSearchValue, TreeNode*& ppOutNode, TreeNode*& ppOutParent)
{
	if (IsEmpty())
		return false;

	TreeNode *current = m_pRoot;
	TreeNode *parent = nullptr;
	while (current != nullptr)
	{
		if (a_nSearchValue == current->GetData())
		{
			ppOutNode = current;
			ppOutParent = parent;
			return true;
		}
		else
		{
			parent = current;
			if (a_nSearchValue < current->GetData())
				current = current->GetLeft();
			else
				current = current->GetRight();
		}
	}
	return false;
}

void BinaryTree::Remove(int a_nValue)
{
	TreeNode *found = nullptr;
	TreeNode *foundParent = nullptr;
	if (!FindNode(a_nValue, found, foundParent))
		return;

	TreeNode *current = found;
	TreeNode *parent = foundParent;
	if (current->HasRight())
	{
		TreeNode *right = current->GetRight();

		TreeNode *minimum = right;
		TreeNode *minimumParent = current;
		while (minimum->HasLeft())
		{
			minimumParent = minimum;
			minimum = minimum->GetLeft();
		}

		current->SetData(minimum->GetData());

		if (minimumParent != nullptr)
		{
			if (minimumParent->GetLeft() == minimum)
				minimumParent->SetLeft(minimum->GetRight());
			else if (minimumParent->GetRight() == minimum)
				minimumParent->SetRight(minimum->GetRight());
		}
	}
	else
	{
		if (parent != nullptr)
		{
			if (parent->GetLeft() == current)
				parent->SetLeft(current->GetLeft());
			if (parent->GetRight() == current)
				parent->SetRight(current->GetLeft());
		}
		if (m_pRoot == current)
		{
			m_pRoot = m_pRoot->GetLeft();
			delete current;
			current = nullptr;
		}
	}
}

void BinaryTree::PrintOrdered()
{
	PrintOrderedRecurse(m_pRoot);
	cout << endl;
}

void BinaryTree::PrintOrderedRecurse(TreeNode* pNode)
{

}

void BinaryTree::PrintUnordered()
{
    PrintUnorderedRecurse(m_pRoot);
	cout << endl;
}

void BinaryTree::PrintUnorderedRecurse(TreeNode* pNode)
{

}

void BinaryTree::Draw(TreeNode* selected)
{
	Draw(m_pRoot, 400, 40, 400, selected);
}

void BinaryTree::Draw(TreeNode* pNode, int x, int y, int horizontalSpacing, TreeNode* selected)
{
	
	horizontalSpacing /= 2;

	if (pNode)
	{
		if (pNode->HasLeft())
		{
			DrawLine(x, y, x - horizontalSpacing, y + 80, RED);
			
			Draw(pNode->GetLeft(), x - horizontalSpacing, y + 80, horizontalSpacing, selected);
		}

		if (pNode->HasRight())
		{
			DrawLine(x, y, x + horizontalSpacing, y + 80, RED);

			Draw(pNode->GetRight(), x + horizontalSpacing, y + 80, horizontalSpacing, selected);
		}

		pNode->Draw(x, y, (selected == pNode));
	}
}