#include "SSXMLLoader.hpp"
#include "../../Common/SSOutputDebugLog.hpp"
#include "Parser/Project/SSProjectParser.hpp"
#include "Parser/AnimationPack/SSAnimationPackParser.hpp"

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
		XMLParser::SSPJInfo sspjInfo;
		{
			XMLReader sspj{ filePath };
			DebugLog::Print(DebugLog::LogType::Verbose, U"以下ファイルの読み込み開始。");
			DebugLog::Print(DebugLog::LogType::Verbose, filePath);
			if (sspj.isNull())
			{
				DebugLog::Print(DebugLog::LogType::Error, U"XMLファイルの読み込みに失敗しました。");
				return false;
			}

			// sspjの解析
			if (not(XMLParser::ProjectParser(sspj, sspjInfo)))
			{
				DebugLog::Print(DebugLog::LogType::Error, U"sspjの解析に失敗しました。");
				return false;
			}

			// 解析したデータをプロジェクトデータに追加
			pOut->setProjectSetting(sspjInfo.projectSetting);
		}

		// プロジェクトのフォルダ
		FilePath projectDir = FileSystem::ParentPath(filePath);

		// ssaeファイルの読み込み
		for(const auto& it : sspjInfo.animPackNames)
		{
			FilePath ssaeDir = FileSystem::PathAppend(projectDir, sspjInfo.relativeAnimBaseDir);
			FilePath ssaePath = FileSystem::PathAppend(ssaeDir, it);
			XMLReader ssae{ ssaePath };
			DebugLog::Print(DebugLog::LogType::Verbose, U"以下ファイルの読み込み開始。");
			DebugLog::Print(DebugLog::LogType::Verbose, ssaePath);
			if (ssae.isNull())
			{
				DebugLog::Print(DebugLog::LogType::Error, U"XMLファイルの読み込みに失敗しました。");
				return false;
			}

			// ssaeの解析
			AnimationPack animPack;
			if (not(XMLParser::AnimationPackParser(ssae, animPack)))
			{
				DebugLog::Print(DebugLog::LogType::Error, U"ssaeの解析に失敗しました。");
				return false;
			}

			// セットアップアニメーションを設定しておく
			for (auto& animation : animPack.animations)
			{
				if (animation.isSetup)
				{
					animPack.pSetupAnimation = &animation;
					break;
				}
			}

			// セットアップデータがないのはスプライトスタジオVer6未満のデータである可能性がある。
			// Ver6未満は動作保証するつもりが無いので警告を出しておく。
			if (animPack.pSetupAnimation == nullptr)
			{
				DebugLog::Print(DebugLog::LogType::Werning, U"セットアップデータがありません。");
			}

			// 解析したデータをプロジェクトデータに追加
			pOut->addAnimationPack(std::move(animPack));
		}

		// ssceの解析

		// sseeの解析

		// ssqeの解析
		
		// fntの解析

		// ssseの解析

		// 成功
		DebugLog::Print(DebugLog::LogType::Verbose, U"プロジェクトの読み込みに成功。");
		return true;
	}

}
