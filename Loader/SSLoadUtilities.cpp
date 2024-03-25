#include "SSLoadUtilities.hpp"
#include "SSLoaderFactory.hpp"
#include "../Common/SSOutputDebugLog.hpp"

namespace
{
	/// @brief 拡張子テーブル キーは.を含まない小文字の拡張子
	std::array<std::pair<const char32_t*, s3d::SpriteStudio::FileType>, 1> ExtTable =
	{
		std::pair{ U"sspj", s3d::SpriteStudio::FileType::Xml }
	};
}

namespace s3d::SpriteStudio::Utilities
{
	//================================================================================
	FileType GetFileType(FilePathView filePath)
	{
		String ext = FileSystem::Extension(filePath);
		for (const auto& it : ExtTable)
		{
			if (it.first == ext)
			{
				return it.second;
			}
		}
		DebugLog::Print(DebugLog::LogType::Error, U"{}は対応していない拡張子です。"_fmt(ext));
		return s3d::SpriteStudio::FileType::Unknown;
	}

	//================================================================================
	Optional<Project> CreateProject(FilePathView filePath)
	{
		Project project;
		if (LoadProject(filePath, &project))
		{
			return project;
		}
		DebugLog::Print(DebugLog::LogType::Error, U"{}からプロジェクトデータの作成に失敗しました。"_fmt(filePath));
		return none;
	}

	//================================================================================
	bool LoadProject(FilePathView filePath, Project* pOut)
	{
		DebugLog::Print(DebugLog::LogType::Verbose, U"読み込むファイルパス: {}"_fmt(filePath));
		if (pOut == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Werning, U"出力先がnullptrです。");
			return false;
		}

		// ファイルの種類を取得
		auto fileType = GetFileType(filePath);
		if (fileType == FileType::Unknown)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"拡張子が未対応です。");
			return false;
		}

		// ローダーを作成
		std::unique_ptr<ILoader> pLoader = std::move(LoaderFactory::Create(fileType));
		if (pLoader == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"Loaderの生成に失敗しました。");
			return false;
		}

		// 読み込み
		if (not(pLoader->load(filePath, pOut)))
		{
			DebugLog::Print(DebugLog::LogType::Error, U"読み込みに失敗しました。");
			return false;
		}

		// 読み込んだ結果nullptrになることは無いと思うが、念のためチェック
		if (pOut == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"読み込み成功が返りましたが、出力先がnullptrです。");
			return false;
		}

		// 成功。
		DebugLog::Print(DebugLog::LogType::Verbose, U"{}の読み込みに成功しました。"_fmt(filePath));
		return true;
	}

}
