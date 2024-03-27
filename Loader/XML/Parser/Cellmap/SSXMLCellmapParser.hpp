#pragma once
#include <Siv3D.hpp>
#include "../../../../Data/Cellmap/SSCellmap.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	/// @brief sspjのパーサー
	/// @param element 解析するデータ
	/// @param out 出力先
	/// @return true 成功, false 失敗
	bool CellmapParser(const XMLElement& element, Cellmap& out);
}
