namespace Endian
{
	inline bool isLittleEndian()
	{
		int val = 0x00000001;
		return *(char*)(&val) ? true : false;
	}

	inline unsigned long reverseEndian(unsigned long value)
	{
		unsigned long retValue = 0;
		retValue |= (value & 0x000000ff) << 24;
		retValue |= (value & 0x0000ff00) << 8;
		retValue |= (value & 0x00ff0000) >> 8;
		retValue |= (value & 0xff000000) >> 24;

		return retValue;
	}
}

