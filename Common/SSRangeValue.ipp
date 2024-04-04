
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
