#ifndef ImageTraitsAllocatedByNewH
#define ImageTraitsAllocatedByNewH

// 画像領域の確保のポリシー
// - new で確保(row align に 2 通り)

template<typename TypePixel, typename RowTraits>
class ImageTraitsAllocatedByNew : public RowTraits
{
public:
	static unsigned char* allocImage(unsigned int width, unsigned int height)
	{
		unsigned int numBytesRow = RowTraits::getNumBytesRow(width, TypePixel::getNumBytes());
		return new unsigned char[numBytesRow * height];
	}

	static void freeImage(unsigned char* pImage)
	{
		delete[] pImage;
	}
};

#endif

