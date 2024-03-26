#pragma once
#include <Siv3D.hpp>
#include "../../../../Data/AnimationPack/SSAnimationPack.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	/// @brief ssaeのパーサー
	/// @param element 解析するデータ
	/// @param out 出力先
	/// @return true 成功, false 失敗
	bool AnimationPackParser(const XMLElement& element, AnimationPack& out);
}
