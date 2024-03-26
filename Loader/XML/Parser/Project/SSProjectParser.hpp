#pragma once
#include <Siv3D.hpp>
#include "../../../../Data/Project/Setting/SSProjectSetting.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	/// @brief sspjから取得する必要なデータ群
	struct SSPJInfo
	{
		/// @brief プロジェクトの設定
		ProjectSetting projectSetting;

		/// @brief プロジェクトフォルダからssaeを保存する基準パス
		FilePath relativeAnimBaseDir;

		/// @brief プロジェクトフォルダからssceを保存する基準パス
		FilePath relativeCellmapBaseDir;

		/// @brief プロジェクトフォルダから画像を保存する基準パス
		FilePath relativeImageBaseDir;

		/// @brief プロジェクトフォルダからsseeを保存する基準パス
		FilePath relativeEffectBaseDir;

		/// @brief ssaeの各ファイルパス
		Array<FilePath> animPackNames;

		/// @brief ssceの各ファイルパス
		Array<FilePath> cellmapNames;

		/// @brief sseeの各ファイルパス
		Array<FilePath> effectNames;

		/// @brief ssqeの各ファイルパス
		Array<FilePath> sequencePackNames;

		/// @brief フォント用キャラマップの各ファイルパス(.fnt?)
		Array<FilePath> charmapNames;

		/// @brief ssseの各ファイルパス
		Array<FilePath> soundNames;

		/// @brief 外部参照している画像の各ファイルパス
		Array<FilePath> externalTextureNames;
	};

	/// @brief sspjのパーサー
	/// @param element 解析するデータ
	/// @param out 出力先
	/// @return true 成功, false 失敗
	bool ProjectParser(const XMLElement& element, SSPJInfo& out);
}
