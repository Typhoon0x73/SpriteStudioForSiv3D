#pragma once
#include <Siv3D.hpp>
#include "SSLoader.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 読み込みクラスの生成
	class LoaderFactory
	{
	public:

		LoaderFactory() = delete;
		~LoaderFactory() = delete;

		/// @brief 読み込みクラスの作成
		/// @param fileType 読み込むファイルの種類
		/// @return 読み込みクラスのインスタンス
		static std::unique_ptr<ILoader> Create(FileType fileType);
	};
}
