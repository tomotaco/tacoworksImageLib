#ifndef ImageTraitsH
#define ImageTraitsH

// 画像の 1 ライン分の領域確保のポリシー
struct ImageTraitsRowNotAligned
{
	static unsigned int getNumBytesRow(unsigned int width, unsigned int numBytesPerPixel)
	{
		return width * numBytesPerPixel;
	}
};

struct ImageTraitsRowAlignedBy32Bit
{
	static unsigned int getNumBytesAligned(unsigned int numBytes)
	{
		return (numBytes + 3) & 0xfffffffc;
	}

	static unsigned int getNumBytesRow(unsigned int width, unsigned int numBytesPerPixel)
	{
		return getNumBytesAligned(width * numBytesPerPixel);
	}
};

struct ImageTraitsRowAlignedBy32Byte
{
	static unsigned int getNumBytesAligned(unsigned int numBytes)
	{
		return (numBytes + 31) & 0xffffffe0;
	}

	static unsigned int getNumBytesRow(unsigned int width, unsigned int numBytesPerPixel)
	{
		return getNumBytesAligned(width * numBytesPerPixel);
	}
};

#endif

