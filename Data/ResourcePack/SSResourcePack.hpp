#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 素材データ群
	struct ResourcePack
	{
		/// @brief セルマップと１対１の画像データ キーにはセルマップ名を利用
		HashTable<String, Texture> cellmapTextureTable{};

		/// @brief 外部参照画像 キーにはファイル名（拡張子あり）を利用
		HashTable<String, Texture> externalTextureTable{};

		/// @brief サウンド キーにはファイル名（拡張子あり）を利用
		HashTable<String, Audio> audioTable{};

		/// @brief フォント キーにはファイル名（拡張子あり）を利用
		HashTable<String, Font> fontTable{};

		/// @brief 頂点シェーダー キーにはファイル名（拡張子あり）を利用
		HashTable<String, VertexShader> vertexShaderTable{};

		/// @brief フラグメントシェーダー キーにはファイル名（拡張子あり）を利用
		HashTable<String, PixelShader> pixelShaderTable{};
	};
}
