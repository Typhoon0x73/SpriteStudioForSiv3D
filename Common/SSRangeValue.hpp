#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 範囲値の種類
	enum class RangeType
	{
		MinMax,    ///< 最小最大
		PlusMinus, ///< -+
	};

	/// @brief 範囲値
	template< class T >
	class RangeValue
	{
	public:

		/// @brief 最小値、最大値を指定して範囲値を作成します。
		/// @param minValue 最小値
		/// @param maxValue 最大値
		[[nodiscard]]
		explicit RangeValue(const T& minValue, const T& maxValue);

		/// @brief 絶対値から-+を指定して範囲値を作成します。
		/// @param v -+の絶対値
		[[nodiscard]]
		explicit RangeValue(const T& v);

		/// @brief デストラクタ
		virtual ~RangeValue();

		/// @brief 最小値、最大値を指定して範囲値を設定します。
		/// @param minValue 最小値
		/// @param maxValue 最大値
		void setMinMax(const T& minValue, const T& maxValue);

		/// @brief 絶対値から-+を指定して範囲値を設定します。
		/// @param v -+の絶対値
		void setPlusMinus(const T& v);

		/// @brief 左値を取得します。
		/// @return 最小値、もしくは絶対値のマイナス値を返します。
		[[nodiscard]]
		const T& getLeftValue() const noexcept; // min or minus

		/// @brief 右値を取得します。
		/// @return 最大値、もしくは絶対値のプラス値を返します。
		[[nodiscard]]
		const T& getRightValue() const noexcept; // max or plus

	private:

		/// @brief 範囲値の種類
		RangeType m_type;

		// left <= right
		T m_left;  // min or minus
		T m_right; // max or plus
	};

#include "SSRangeValue.ipp"

}
