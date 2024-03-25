#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio::DebugLog
{
	/// @brief ログの種類
	enum class LogType
	{
		Werning, ///< 警告（間違った使い方をしている）
		Error,   ///< エラー（問題があり利用できない状態）
		Notice,  ///< 開発者への通知（未実装や不明なパラメータを発見した場合）
		Verbose, ///< とりあえず出すデバッグ用
	};

	/// @brief 出力タブに指定した文字を書き出します。
	/// @param type 出力する種類
	/// @param message 出力する文字列
	/// @remark SIV3D_SS_USE_LOGGER の定義が有効でない場合は何もしません。
	void Print([[maybe_unused]] LogType type, [[maybe_unused]] StringView message);
}
