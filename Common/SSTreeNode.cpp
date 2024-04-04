#include "SSTreeNode.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	TreeNode::TreeNode()
		: m_pParent{ nullptr }
		, m_pFirstChild{ nullptr }
		, m_pPrev{ nullptr }
		, m_pNext{ nullptr }
	{
	}

	//================================================================================
	TreeNode::~TreeNode()
	{
	}

	//================================================================================
	void TreeNode::linkToParent(TreeNode* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}
		toRootNode();
		if (pNode->m_pFirstChild != nullptr)
		{
			pNode->linkToEndChild(this);
		}
	}

	//================================================================================
	void TreeNode::linkToFirstChild(TreeNode* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}
		pNode->toRootNode();
		pNode->m_pParent = this;
		if (m_pFirstChild == nullptr)
		{
			m_pFirstChild = pNode;
			return;
		}
		pNode->m_pNext = m_pFirstChild;
		m_pFirstChild->m_pPrev = pNode;
		m_pFirstChild = pNode;
	}

	//================================================================================
	void TreeNode::linkToEndChild(TreeNode* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}
		pNode->toRootNode();
		pNode->m_pParent = this;
		if (m_pFirstChild == nullptr)
		{
			m_pFirstChild = pNode;
			return;
		}
		for (auto* pChild = m_pFirstChild; pChild != nullptr; pChild = pChild->m_pNext)
		{
			if (pChild->m_pNext != nullptr)
			{
				continue;
			}
			pNode->m_pPrev = pChild;
			pChild->m_pNext = pNode;
			return;
		}
	}

	//================================================================================
	void TreeNode::linkToPrev(TreeNode* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}
		pNode->toRootNode();
		if (m_pParent != nullptr)
		{
			pNode->m_pParent = m_pParent;
			if (m_pPrev == nullptr)
			{
				m_pParent->m_pFirstChild = pNode;
			}
		}
		pNode->m_pNext = this;
		if (m_pPrev != nullptr)
		{
			pNode->m_pPrev = m_pPrev;
			m_pPrev->m_pNext = pNode;
		}
		m_pPrev = pNode;
	}

	//================================================================================
	void TreeNode::linkToNext(TreeNode* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}
		pNode->toRootNode();
		if (m_pParent != nullptr)
		{
			pNode->m_pParent = m_pParent;
		}
		pNode->m_pPrev = this;
		if (m_pNext != nullptr)
		{
			pNode->m_pNext = m_pNext;
			m_pNext->m_pPrev = pNode;
		}
		m_pNext = pNode;
	}

	//================================================================================
	void TreeNode::toRootNode()
	{
		if (m_pParent == nullptr
			and m_pPrev == nullptr
			and m_pNext == nullptr)
		{
			return;
		}
		if (m_pParent != nullptr)
		{
			// 兄弟無し
			if (m_pPrev == nullptr
				and m_pNext == nullptr)
			{
				m_pParent->m_pFirstChild = nullptr;
				m_pParent = nullptr;
				return;
			}
			// 長男
			if (m_pPrev == nullptr
				and m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
				m_pParent->m_pFirstChild = m_pNext;
				m_pNext = nullptr;
				m_pParent = nullptr;
				return;
			}
			// 末っ子
			if (m_pPrev != nullptr
				and m_pNext == nullptr)
			{
				m_pPrev->m_pNext = nullptr;
				m_pPrev = nullptr;
				m_pParent = nullptr;
				return;
			}
			// 間
			if (m_pPrev != nullptr
				and m_pNext != nullptr)
			{
				m_pPrev->m_pNext = m_pNext;
				m_pNext->m_pPrev = m_pPrev;
				m_pPrev = nullptr;
				m_pNext = nullptr;
				m_pParent = nullptr;
				return;
			}
		}
		// 親無し
		else
		{
			// 長男
			if (m_pPrev == nullptr
				and m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
				m_pNext = nullptr;
				return;
			}
			// 末っ子
			if (m_pPrev != nullptr
				and m_pNext == nullptr)
			{
				m_pPrev->m_pNext = nullptr;
				m_pPrev = nullptr;
				return;
			}
			// 間
			if (m_pPrev != nullptr
				and m_pNext != nullptr)
			{
				m_pPrev->m_pNext = m_pNext;
				m_pNext->m_pPrev = m_pPrev;
				m_pPrev = nullptr;
				m_pNext = nullptr;
				return;
			}
		}
	}

}
