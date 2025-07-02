#include "SSXMLLoader.hpp"
#include "../../Common/SSOutputDebugLog.hpp"
#include "Parser/Project/SSXMLProjectParser.hpp"
#include "Parser/AnimationPack/SSXMLAnimationPackParser.hpp"
#include "Parser/Cellmap/SSXMLCellmapParser.hpp"
#include "Parser/Effect/SSXMLEffectPerser.hpp"

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
			DebugLog::Print(DebugLog::LogType::Warning, U"出力先がnullptrです。");
			return false;
		}

		// sspjファイルの読み込み
		XMLParser::SSPJInfo sspjInfo;
		{
			XMLReader sspj{ filePath };
			DebugLog::Print(DebugLog::LogType::Info, U"以下ファイルの読み込み開始。");
			DebugLog::Print(DebugLog::LogType::Info, filePath);
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
			DebugLog::Print(DebugLog::LogType::Info, U"以下ファイルの解析開始。");
			DebugLog::Print(DebugLog::LogType::Info, ssaePath);
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
			DebugLog::Print(DebugLog::LogType::Info, U"解析完了。");

			// セットアップアニメーションを設定しておく
			for (size_t i = 0; i < animPack.animations.size(); i++)
			{
				if (animPack.animations[i].isSetup)
				{
					animPack.setupIndex = static_cast<int32>(i);
					break;
				}
			}

			// セットアップデータがないのはスプライトスタジオVer6未満のデータである可能性がある。
			// Ver6未満は動作保証するつもりが無いので警告を出しておく。
			if (animPack.getSetupAnimation() == nullptr)
			{
				DebugLog::Print(DebugLog::LogType::Warning, U"セットアップデータがありません。");
			}

			// 解析したデータをプロジェクトデータに追加
			pOut->addAnimationPack(animPack);
		}

		// ssceファイルの読み込み
		for (const auto& it : sspjInfo.cellmapNames)
		{
			FilePath ssceDir = FileSystem::PathAppend(projectDir, sspjInfo.relativeCellmapBaseDir);
			FilePath sscePath = FileSystem::PathAppend(ssceDir, it);
			XMLReader ssce{ sscePath };
			DebugLog::Print(DebugLog::LogType::Info, U"以下ファイルの解析開始。");
			DebugLog::Print(DebugLog::LogType::Info, sscePath);
			if (ssce.isNull())
			{
				DebugLog::Print(DebugLog::LogType::Error, U"XMLファイルの読み込みに失敗しました。");
				return false;
			}

			// ssceの解析
			Cellmap cellmap;
			if (not(XMLParser::CellmapParser(ssce, cellmap)))
			{
				DebugLog::Print(DebugLog::LogType::Error, U"ssceの解析に失敗しました。");
				return false;
			}
			DebugLog::Print(DebugLog::LogType::Info, U"解析完了。");

			// 解析したデータをプロジェクトデータに追加
			pOut->addCellmap(cellmap);
		}

		// sseeファイルの読み込み
		for (const auto& it : sspjInfo.effectNames)
		{
			FilePath sseeDir = FileSystem::PathAppend(projectDir, sspjInfo.relativeEffectBaseDir);
			FilePath sseePath = FileSystem::PathAppend(sseeDir, it);
			XMLReader ssee{ sseePath };
			DebugLog::Print(DebugLog::LogType::Info, U"以下ファイルの解析開始。");
			DebugLog::Print(DebugLog::LogType::Info, sseePath);
			if (ssee.isNull())
			{
				DebugLog::Print(DebugLog::LogType::Error, U"XMLファイルの読み込みに失敗しました。");
				return false;
			}

			// sseeの解析
			Effect effect;
			if (not(XMLParser::EffectParser(ssee, effect)))
			{
				DebugLog::Print(DebugLog::LogType::Error, U"sseeの解析に失敗しました。");
				return false;
			}
			DebugLog::Print(DebugLog::LogType::Info, U"解析完了。");

			// 解析したデータをプロジェクトデータに追加
			pOut->addEffect(effect);
		}

		// ssqeの解析
		
		// fntの解析

		// ssseの解析

		// 素材データの読み込み
		{
			ResourcePack& resourcePackRaw = pOut->getResourcePackRaw();
			DebugLog::Print(DebugLog::LogType::Info, U"リソースの読み込み開始。");

			// 画像データの読み込み
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- texture resources ---");
			FilePath imageDir = FileSystem::PathAppend(projectDir, sspjInfo.relativeImageBaseDir);
			// セルマップ
			const auto& cellmaps = pOut->getCellmaps();
			for (const auto& cellmap : cellmaps)
			{
				FilePath textureFilePath = FileSystem::PathAppend(imageDir, cellmap.imagePath);
				resourcePackRaw.cellmapTextureTable[cellmap.name] = Texture{ textureFilePath };
				DebugLog::Print(DebugLog::LogType::Verbose, U"セルマップ名:{}"_fmt(cellmap.name));
				DebugLog::Print(DebugLog::LogType::Verbose, U"画像ファイル:{}"_fmt(textureFilePath));
			}
			// 外部参照
			// memo: 読み込みを任意にできるようにしたほうが良さそう？
			for (const auto& externalTextureName : sspjInfo.externalTextureNames)
			{
				FilePath textureFilePath = FileSystem::PathAppend(imageDir, externalTextureName);
				resourcePackRaw.externalTextureTable[externalTextureName] = Texture{ textureFilePath };
				DebugLog::Print(DebugLog::LogType::Verbose, U"外部参照キー:{}"_fmt(externalTextureName));
				DebugLog::Print(DebugLog::LogType::Verbose, U"画像ファイル:{}"_fmt(textureFilePath));
			}
			DebugLog::Print(DebugLog::LogType::Info, U"リソースの読み込み完了。");
		}

		// 成功
		DebugLog::Print(DebugLog::LogType::Verbose, U"プロジェクトの読み込みに成功。");
		return true;
	}

}
