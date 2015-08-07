#ifndef ImageTraitsNoAllocateH
#define ImageTraitsNoAllocateH

// メモリ管理をしない(ImageExternal 用)
struct ImageTraitsNoAllocate
{
public:
	static unsigned char* allocImage(unsigned int width, unsigned int height)
	{
		assert(0);
		return NULL;
	}
	static void freeImage(unsigned char* pImage)
	{
		assert(0);
	}
};

#endif

