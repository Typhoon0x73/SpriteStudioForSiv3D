#pragma once
#include <Siv3D.hpp>
#include "../../../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	using SignalValue = std::variant<
		std::monostate
		, int32
		, float
		, String
	>;

	/// @brief シグナルパラメータ
	struct AttributeValueSignal
	{
		/// @brief シグナルコマンドのパラメータ
		struct CommandParam
		{
			/// @brief 識別ID
			String id{ U"" };

			/// @brief パラメータの種類
			SignalType type{ SignalType::Invalid };

			/// @brief 値
			SignalValue value{ std::monostate{} };
		};

		/// @brief コマンド
		struct Command
		{
			/// @brief 有効か
			bool isActive{ false };

			/// @brief コマンドID
			String id{ U"" };

			/// @brief パラメータリスト
			Array<CommandParam> params{};

			/// @brief メモ
			String note{ U"" };
		};

		/// @brief コマンドリスト
		Array<Command> commands{};
	};
}
