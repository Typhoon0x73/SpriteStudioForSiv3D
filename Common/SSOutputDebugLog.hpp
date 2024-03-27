#pragma once
#include <Siv3D.hpp>

// デバッグ用定義
# ifdef _DEBUG
//	デバッグ用ログ出力定義(コメント解除で出力タブにログ出力します。)
#	define SIV3D_SS_USE_LOGGER
# endif // _DEBUG

namespace s3d::SpriteStudio::DebugLog
{
	/// @brief ログの種類
	enum class LogType : size_t
	{
		Werning, ///< 警告（間違った使い方をしている）
		Error,   ///< エラー（問題があり利用できない状態）
		Notice,  ///< 開発者への通知（未実装や不明なパラメータを発見した場合）
		Info,    ///< 基本的な情報
		Verbose, ///< とりあえず出すデバッグ用
	};

	/// @brief ログの有効フラグ
	using LogEnable = YesNo<struct LogEnable_tag>;

	/// @brief 出力先バッファ（デフォルトはLogger）
	using ExportBuffer = std::variant<
		detail::Logger_impl
		, detail::Print_impl
		, detail::Console_impl
	>;

	/// @brief ログの出力先を変更します。（デフォルトはLogger）
	/// @param buffer 変更先出力バッファ
	void ChangeBuffer([[maybe_unused]] ExportBuffer buffer);

	/// @brief ログをファイルへ出力します。
	/// @param pWriter 出力先ポインタ
	void ExportWriter([[maybe_unused]] TextWriter* pWriter);

	/// @brief 出力をログの種類ごとに制御します。
	/// @param type ログの種類
	/// @param enable Yes 出力する, No 出力しない
	void SetExportLog([[maybe_unused]] LogType type, [[maybe_unused]] LogEnable enable);

	/// @brief 出力タブに指定した文字を書き出します。
	/// @param type 出力する種類
	/// @param message 出力する文字列
	/// @remark SIV3D_SS_USE_LOGGER の定義が有効でない場合は何もしません。
	void Print([[maybe_unused]] LogType type, [[maybe_unused]] StringView message);
}
