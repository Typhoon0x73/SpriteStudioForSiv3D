
//================================================================================
inline bool AttributeValueCell::operator ==(const AttributeValueCell& right) noexcept
{
	return (this->mapId == right.mapId
		and this->name == right.name);
}

//================================================================================
inline bool AttributeValueCell::operator !=(const AttributeValueCell& right) noexcept
{
	return not(*this == right);
}
