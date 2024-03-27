#include "SSXMLProjectParser.hpp"
#include "../../../../Common/SSOutputDebugLog.hpp"
#include "../../SSXMLEnumTable.hpp"
#include "../SSXMLParserUtilities.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	namespace
	{
		//================================================================================
		bool ParseSettings(const XMLElement& element, SSPJInfo& out)
		{
			if (element.name() != U"settings")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- project settings ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToFilePath(child, U"animeBaseDirectory", out.relativeAnimBaseDir))
				{
					continue;
				}
				if (Utilities::TextToFilePath(child, U"cellMapBaseDirectory", out.relativeCellmapBaseDir))
				{
					continue;
				}
				if (Utilities::TextToFilePath(child, U"imageBaseDirectory", out.relativeImageBaseDir))
				{
					continue;
				}
				if (Utilities::TextToFilePath(child, U"effectBaseDirectory", out.relativeEffectBaseDir))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"wrapMode", TextureAddressModeTable, out.projectSetting.addressMode))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"filterMode", TextureFilterTable, out.projectSetting.filterMode))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseExternalTexture(const XMLElement& element, Array<FilePath>& out)
		{
			if (element.name() != U"ExternalTextures")
			{
				return false;
			}
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- external textures ---");
			for (auto files = element.firstChild(); not(files.isNull()); files = files.nextSibling())
			{
				if (files.name() != U"files")
				{
					continue;
				}
				for (auto value = files.firstChild(); not(value.isNull()); value = value.nextSibling())
				{
					if (Utilities::ChildToFilePathArray(value, U"value", U"PathName", out))
					{
						continue;
					}
				}
			}
			return true;
		}

		//================================================================================
		bool ParseProject(const XMLElement& element, SSPJInfo& out)
		{
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- sspj ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseSettings(child, out))
				{
					continue;
				}
				if (Utilities::ChildToFilePathArray(child, U"animepackNames", U"value", out.animPackNames))
				{
					continue;
				}
				if (Utilities::ChildToFilePathArray(child, U"cellmapNames", U"value", out.cellmapNames))
				{
					continue;
				}
				if (Utilities::ChildToFilePathArray(child, U"effectFileNames", U"value", out.effectNames))
				{
					continue;
				}
				if (Utilities::ChildToFilePathArray(child, U"sequencepackNames", U"value", out.sequencePackNames))
				{
					continue;
				}
				if (Utilities::ChildToFilePathArray(child, U"charmapNames", U"value", out.charmapNames))
				{
					continue;
				}
				if (Utilities::ChildToFilePathArray(child, U"audiopackNames", U"value", out.soundNames))
				{
					continue;
				}
				if (ParseExternalTexture(child, out.externalTextureNames))
				{
					continue;
				}
			}
			return true;
		}
	}

	//================================================================================
	bool ProjectParser(const XMLElement& element, SSPJInfo& out)
	{
		try
		{
			return ParseProject(element, out);
		}
		catch (const Error& e)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"解析中にエラーが発生しました。");
			DebugLog::Print(DebugLog::LogType::Error, U"Type:"_fmt(e.type()));
			DebugLog::Print(DebugLog::LogType::Error, U"What:"_fmt(e.what()));
			return false;
		}
	}
}
