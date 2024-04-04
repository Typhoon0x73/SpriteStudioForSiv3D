#pragma once
#include <Siv3D.hpp>
#include "../../../../Data/Effect/SSEffect.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	/// @brief sseeのパーサー
	/// @param element 解析するデータ
	/// @param out 出力先
	/// @return true 成功, false 失敗
	bool EffectParser(const XMLElement& element, Effect& out);
}
