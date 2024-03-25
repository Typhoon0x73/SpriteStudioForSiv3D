#pragma once
#include <Siv3D.hpp>
#include "../SSLoader.hpp"

namespace s3d::SpriteStudio
{
	/// @brief sspjからプロジェクトデータを読み込む
	class XMLLoader : public ILoader
	{
	public:

		/// @brief コンストラクタ
		[[nodiscard]]
		XMLLoader();

		/// @brief デストラクタ
		virtual ~XMLLoader();

		/// @brief ファイルからプロジェクトの読み込み
		/// @param filePath 読み込むファイルパス
		/// @param pOut 出力するプロジェクトポインタ
		/// @return true: 成功, false: 失敗
		virtual bool load(FilePathView filePath, Project* pOut) override;
	};
}
