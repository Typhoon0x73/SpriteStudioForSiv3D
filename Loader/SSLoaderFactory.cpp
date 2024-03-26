#include "SSLoaderFactory.hpp"
#include "XML/SSXMLLoader.hpp"
#include "../Common/SSOutputDebugLog.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	std::unique_ptr<ILoader> LoaderFactory::Create(FileType fileType)
	{
		switch (fileType)
		{
		case FileType::Unknown:
		{
			DebugLog::Print(DebugLog::LogType::Werning, U"不明なファイルの種類の為nullptrを返します。");
			return nullptr;
		}
		case FileType::Xml:
		{
			DebugLog::Print(DebugLog::LogType::Verbose, U"XMLLoaderを作成します。");
			return std::make_unique<XMLLoader>();
		}
		default:
		{
			DebugLog::Print(DebugLog::LogType::Error, U"入力された不明な値:{}"_fmt(FromEnum(fileType)));
			break;
		}
		}
		DebugLog::Print(DebugLog::LogType::Error, U"不明なファイルの種類の為nullptrを返します。");
		return nullptr;
	}

}
