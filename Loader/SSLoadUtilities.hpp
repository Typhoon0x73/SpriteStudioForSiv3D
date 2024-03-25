#pragma once
#include <Siv3D.hpp>
#include "../Data/Project/SSProject.hpp"

namespace s3d::SpriteStudio::Utilities
{
	/// @brief 拡張子からファイルの種類を取得します。
	/// @param filePath ファイルパス
	/// @return ファイルの種類
	FileType GetFileType(FilePathView filePath);

	/// @brief ファイルからプロジェクトデータを作成します。
	/// @param filePath ファイルパス
	/// @return 読み込み完了したプロジェクトデータ or 失敗でnoneが返ります。
	Optional<Project> CreateProject(FilePathView filePath);

	/// @brief ファイルからプロジェクトを読み込みます。
	/// @param filePath 読み込むファイルパス
	/// @param pOut 出力するプロジェクトポインタ
	/// @return true: 成功, false: 失敗
	bool LoadProject(FilePathView filePath, Project* pOut);
}
