inline uint32 XORShift32::generateUint32()
{
	seed = seed ^ (seed << 13);
	seed = seed ^ (seed >> 17);
	seed = seed ^ (seed << 15);
	return seed;
}

inline float XORShift32::generateFloat32()
{
	uint32 v = generateUint32();
	uint32 res = (v >> 9) | 0x3f800000;
	return ((*(reinterpret_cast<float*>(&res))) - 1.0f);
}
