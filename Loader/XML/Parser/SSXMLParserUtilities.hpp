#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSOutputDebugLog.hpp"

namespace s3d::SpriteStudio::XMLParser::Utilities
{
	bool TextToInt32(const XMLElement& element, StringView name, int32& out);

	bool TextToBool(const XMLElement& element, StringView name, bool& out);

	bool TextToFloat(const XMLElement& element, StringView name, float& out);

	bool TextToPoint(const XMLElement& element, StringView name, Point& out);

	bool TextToSize(const XMLElement& element, StringView name, Size& out);

	bool TextToFloat2(const XMLElement& element, StringView name, Float2& out);

	bool TextToTriangleIndex(const XMLElement& element, StringView name, TriangleIndex& out);

	bool TextToString(const XMLElement& element, StringView name, String& out);

	bool TextToRect(const XMLElement& element, StringView name, RectF& out);

	bool TextToColor(const XMLElement& element, StringView name, Color& out);

	bool TextToFilePath(const XMLElement& element, StringView name, FilePath& out);

	bool ChildToFilePathArray(const XMLElement& element, StringView name, StringView childName, Array<FilePath>& out);

	bool AttributeToInt32(const XMLElement& element, StringView key, int32& out);

	bool AttributeToFloat(const XMLElement& element, StringView key, float& out);

	bool AttributeToString(const XMLElement& element, StringView key, String& out);

	bool AttributeToColor(const XMLElement& element, StringView key, Color& out);

	template< class T, size_t N >
	inline bool TextToEnum(const XMLElement& element, StringView name, const std::array<std::pair<const char32_t*, T>, N>& table, T& out);

	template< class T, size_t N >
	inline bool AttributeToEnum(const XMLElement& element, StringView key, const std::array<std::pair<const char32_t*, T>, N>& table, T& out);

#include "SSXMLParserUtilities.ipp"
}
