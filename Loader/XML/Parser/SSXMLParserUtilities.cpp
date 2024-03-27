#include "SSXMLParserUtilities.hpp"
#include "../../../Common/SSOutputDebugLog.hpp"

namespace s3d::SpriteStudio::XMLParser::Utilities
{
	//================================================================================
	bool TextToInt32(const XMLElement& element, StringView name, int32& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to int32 ------------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		out = Parse<int32>(element.text());

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToBool(const XMLElement& element, StringView name, bool& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to bool -------------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		int32 tmpInt = Parse<int32>(element.text());
		out = (0 < tmpInt);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToFloat(const XMLElement& element, StringView name, float& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to float ------------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		out = Parse<float>(element.text());

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToPoint(const XMLElement& element, StringView name, Point& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to point ------------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		const auto& lines = element.text().split(U' ');

		if (lines.size() < 2)
		{
			throw Error{ U"XMLのテキストからPoint型へパース失敗。" };
		}

		out.x = Parse<int32>(lines[0]);
		out.y = Parse<int32>(lines[1]);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToSize(const XMLElement& element, StringView name, Size& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to size -------------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		const auto& lines = element.text().split(U' ');

		if (lines.size() < 2)
		{
			throw Error{ U"XMLのテキストからSize型へパース失敗。" };
		}

		out.x = Parse<int32>(lines[0]);
		out.y = Parse<int32>(lines[1]);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToFloat2(const XMLElement& element, StringView name, Float2& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to float2 -----------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		const auto& lines = element.text().split(U' ');

		if (lines.size() < 2)
		{
			throw Error{ U"XMLのテキストからFloat2型へパース失敗。" };
		}

		out.x = Parse<float>(lines[0]);
		out.y = Parse<float>(lines[1]);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToTriangleIndex(const XMLElement& element, StringView name, TriangleIndex& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to triangle index ---");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		const auto& lines = element.text().split(U' ');

		if (lines.size() < 3)
		{
			throw Error{ U"XMLのテキストからTriangleIndex型へパース失敗。" };
		}

		out.i0 = Parse<uint16>(lines[0]);
		out.i1 = Parse<uint16>(lines[1]);
		out.i2 = Parse<uint16>(lines[2]);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :({}, {}, {})"_fmt(out.i0, out.i1, out.i2));
		return true;
	}

	//================================================================================
	bool TextToString(const XMLElement& element, StringView name, String& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to string -----------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		out = element.text();

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToRect(const XMLElement& element, StringView name, RectF& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to rect -------------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		const auto& lines = element.text().split(U' ');

		if (lines.size() < 4)
		{
			throw Error{ U"XMLのテキストからRectF型へパース失敗。" };
		}

		out.x = Parse<double>(lines[0]);
		out.y = Parse<double>(lines[1]);
		out.w = Parse<double>(lines[2]) - out.x;
		out.h = Parse<double>(lines[3]) - out.y;

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToColor(const XMLElement& element, StringView name, Color& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to color ------------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		const auto& text = element.text();

		if (text.length() < 6)
		{
			throw Error{ U"XMLのテキストからColor型へパース失敗。" };
		}

		out = Color{ U"x" + text.substr(2, 6) + text.substr(0, 2) }; // RGBA → ARGB

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool TextToFilePath(const XMLElement& element, StringView name, FilePath& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to filepath ---------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
		DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

		out = element.text();

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool ChildToFilePathArray(const XMLElement& element, StringView name, StringView childName, Array<FilePath>& out)
	{
		if (element.name() != name)
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- child to filepath array --");
		DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));

		for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
		{
			FilePath tmp;
			if (TextToFilePath(child, childName, tmp))
			{
				out.emplace_back(tmp);
			}
		}

		return true;
	}

	//================================================================================
	bool AttributeToInt32(const XMLElement& element, StringView key, int32& out)
	{
		auto attribute = element.attribute(String(key));
		if (not(attribute))
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- attribute to int32 -------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"key :{}"_fmt(key));
		DebugLog::Print(DebugLog::LogType::Verbose, U"val :{}"_fmt(attribute.value()));

		out = Parse<int32>(attribute.value());

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

	//================================================================================
	bool AttributeToFloat(const XMLElement& element, StringView key, float& out)
	{
		auto attribute = element.attribute(String(key));
		if (not(attribute))
		{
			return false;
		}

		DebugLog::Print(DebugLog::LogType::Verbose, U"--- attribute to float -------");
		DebugLog::Print(DebugLog::LogType::Verbose, U"key :{}"_fmt(key));
		DebugLog::Print(DebugLog::LogType::Verbose, U"val :{}"_fmt(attribute.value()));

		out = Parse<float>(attribute.value());

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

}
