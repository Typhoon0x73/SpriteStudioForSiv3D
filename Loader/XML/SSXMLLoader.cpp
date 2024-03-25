#include "SSXMLLoader.hpp"
#include "../../Common/SSOutputDebugLog.hpp"
#include "Perser/SSXMLPerser.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	XMLLoader::XMLLoader()
	{
	}

	//================================================================================
	XMLLoader::~XMLLoader()
	{
	}

	//================================================================================
	bool XMLLoader::load(FilePathView filePath, Project* pOut)
	{
		// 出力先の存在確認
		if (pOut == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Werning, U"出力先がnullptrです。");
			return false;
		}

		// sspjファイルの読み込み
		XMLReader sspj{ filePath };
		if (sspj.isNull())
		{
			DebugLog::Print(DebugLog::LogType::Error, U"XMLファイルの読み込みに失敗しました。");
			return false;
		}

		// パーサーでデータに入れる
		if (not(XMLPerser::Perse(FileSystem::ParentPath(filePath), sspj, *pOut)))
		{
			DebugLog::Print(DebugLog::LogType::Error, U"sspjのパースに失敗しました。");
			return false;
		}

		// 成功
		DebugLog::Print(DebugLog::LogType::Verbose, U"sspjのの読み込みに成功。");
		return true;
	}

}
