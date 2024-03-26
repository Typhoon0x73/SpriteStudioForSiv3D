#include "SSXMLParserUtilities.hpp"
#include "../../../Common/SSOutputDebugLog.hpp"

namespace s3d::SpriteStudio::XMLParser::Utilities
{
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

		out.x = Parse<int32>(lines[0]);
		out.y = Parse<int32>(lines[1]);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

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

		out.x = Parse<int32>(lines[0]);
		out.y = Parse<int32>(lines[1]);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

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

		out.x = Parse<float>(lines[0]);
		out.y = Parse<float>(lines[1]);

		DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(out));
		return true;
	}

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

}
