#pragma once
#include <Siv3D.hpp>
#include "../Data/Project/SSProject.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 読み込みインターフェース
	class ILoader
	{
	public:

		/// @brief デストラクタ
		virtual ~ILoader() = default;

		/// @brief ファイルからプロジェクトの読み込み
		/// @param filePath 読み込むファイルパス
		/// @param pOut 出力するプロジェクトポインタ
		/// @return true: 成功, false: 失敗
		virtual bool load(FilePathView filePath, Project* pOut) = 0;
	};
}
