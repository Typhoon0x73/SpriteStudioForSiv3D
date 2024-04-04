#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	class TreeNode
	{

	public:

		/// @brief コンストラクタ
		[[nodiscard]]
		TreeNode();

		/// @brief デストラクタ
		virtual ~TreeNode();

		/// @brief 自身の親にする
		/// @param pNode この引数を親にする 
		void linkToParent(TreeNode* pNode);

		/// @brief 自身の最初の子どもにする
		/// @param pNode この引数を長男にする
		void linkToFirstChild(TreeNode* pNode);

		/// @brief 自身の末っ子にする
		/// @param pNode この引数を末っ子にする
		void linkToEndChild(TreeNode* pNode);

		/// @brief 自身の兄にする
		/// @param pNode この引数を兄にする
		void linkToPrev(TreeNode* pNode);

		/// @brief 自身の弟にする
		/// @param pNode この引数を弟にする
		void linkToNext(TreeNode* pNode);

		/// @brief 親、兄弟を外して自身をルートノードにする
		void toRootNode();

	private:

		/// @brief 親
		TreeNode* m_pParent;

		/// @brief 長男
		TreeNode* m_pFirstChild;

		/// @brief 前
		TreeNode* m_pPrev;

		/// @brief 次
		TreeNode* m_pNext;

	};
}
