#pragma once
#include <Siv3D.hpp>
#include "../../../Data/Project/SSProject.hpp"

namespace s3d::SpriteStudio::XMLPerser
{
	/// @brief XMLデータを解析してプロジェクトデータに変換します。
	/// @param projectDir プロジェクトフォルダパス
	/// @param element sspjファイルのデータ
	/// @param out 出力先プロジェクト
	bool Perse(FilePathView projectDir, const XMLElement& element, Project& out);
}
