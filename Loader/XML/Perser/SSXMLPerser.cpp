#include "SSXMLPerser.hpp"
#include "../../../Common/SSOutputDebugLog.hpp"

namespace
{
	//================================================================================
	/// @brief 読み込み用プロジェクト設定パラメータ
	struct SSPJProjectSetting
	{
		s3d::SpriteStudio::ProjectSetting projectSetting;
		s3d::FilePath relativeAnimBaseDir;
		s3d::FilePath relativeCellmapBaseDir;
		s3d::FilePath relativeImageDir;
		s3d::FilePath relativeEffectBaseDir;
	};

	//================================================================================
	/// @brief 読み込み用プロジェクトパラメータ
	struct SSPJProject
	{
		SSPJProjectSetting sspjProjectSetting;
		s3d::Array<s3d::FilePath> animPackNames;
		s3d::Array<s3d::FilePath> cellmapNames;
		s3d::Array<s3d::FilePath> effectNames;
		s3d::Array<s3d::FilePath> sequencePackNames;
		s3d::Array<s3d::FilePath> soundNames;
		s3d::Array<s3d::FilePath> externalTextureNames;
	};

	//================================================================================
	/// @brief プロジェクト設定の解析
	/// @param element 解析するデータ
	/// @param out 出力先
	/// @return true 解析完了, false 対象外
	bool PerseSettings(const s3d::XMLElement& element, SSPJProjectSetting& out)
	{
		if (element.name() != U"settings")
		{
			return false;
		}
		for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
		{

		}
		return true;
	}
}

namespace s3d::SpriteStudio
{
	//================================================================================
	bool Perse(FilePathView projectDir, const XMLElement& element, Project& out)
	{
		// 空のプロジェクトでない場合は事故の元なのでfalseで一蹴。
		if (not(out.isEmpty()))
		{
			return false;
		}

		SSPJProjectSetting sspjProjectSetting;

		// sspjの解析
		for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
		{
			if (::PerseSettings(child, sspjProjectSetting))
			{
				continue;
			}
			DebugLog::Print(DebugLog::LogType::Notice, U"未反映のパラメータを検知しました。: sspj::{}"_fmt(child.name()));
		}

		// ssaeの解析

		// ssceの解析

		// sseeの解析

		// ssqeの解析

		// ssseの解析

		return true;
	}

}
