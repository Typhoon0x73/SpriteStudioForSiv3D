
//================================================================================
template< class T > inline RangeValue<T>::RangeValue(const T& minValue, const T& maxValue)
	: m_type{ RangeType::MinMax }
	, m_left{ minValue }
	, m_right{ maxValue }
{
}

//================================================================================
template< class T > inline RangeValue<T>::RangeValue(const T& v)
	: m_type{ RangeType::PlusMinus }
	, m_left{ -v }
	, m_right{ v }
{
}

//================================================================================
template< class T > inline RangeValue<T>::~RangeValue()
{
}

//================================================================================
//template< class T > inline bool RangeValue<T>::load(const XMLElement& data)
//{
//	m_type = RangeType::MinMax;
//	XMLParser::AttributeTo(data, U"value", m_left);
//	XMLParser::AttributeTo(data, U"subvalue", m_right);
//	return true;
//}

//================================================================================
template< class T > inline void RangeValue<T>::setMinMax(const T& minValue, const T& maxValue)
{
	m_type = RangeType::MinMax;
	m_left = minValue;
	m_right = maxValue;
}

//================================================================================
template< class T > inline void RangeValue<T>::setPlusMinus(const T& v)
{
	m_type = RangeType::PlusMinus;
	m_left = -v;
	m_right = v;
}

//================================================================================
template< class T > inline const T& RangeValue<T>::getLeftValue() const noexcept
{
	return m_left;
}

//================================================================================
template< class T > inline const T& RangeValue<T>::getRightValue() const noexcept
{
	return m_right;
}

//================================================================================
template< class T >
inline void RangeValue<T>::getRange(T& outMin, T& outDiff) const
{
	outMin = Min(m_left, m_right);
	T tmpMax = Max(m_left, m_right);
	outDiff = tmpMax - outMin;
}

//================================================================================
template<>
inline void RangeValue<Color>::getRange(Color& outMin, Color& outDiff) const
{
	outMin.a = Min(m_left.a, m_right.a);
	outMin.r = Min(m_left.r, m_right.r);
	outMin.g = Min(m_left.g, m_right.g);
	outMin.b = Min(m_left.b, m_right.b);
	uint8 tmpMaxA = Max(m_left.a, m_right.a);
	uint8 tmpMaxR = Max(m_left.r, m_right.r);
	uint8 tmpMaxG = Max(m_left.g, m_right.g);
	uint8 tmpMaxB = Max(m_left.b, m_right.b);
	outDiff.a = tmpMaxA - outMin.a;
	outDiff.r = tmpMaxR - outMin.r;
	outDiff.g = tmpMaxG - outMin.g;
	outDiff.b = tmpMaxB - outMin.b;
}
