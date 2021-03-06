#ifndef PixelTypeH
#define PixelTypeH

enum ChannelType
{
	CHANNEL_UNKNOWN,
	CHANNEL_GRAY,
	CHANNEL_RGB,
	CHANNEL_RGBA,
	NUM_CHANNEL_TYPES
};

inline unsigned int getNumElementsFromTypeChannel(ChannelType typeChannel)
{
	switch (typeChannel) {
		case CHANNEL_GRAY:
			return 1;
			break;
		case CHANNEL_RGB:
			return 3;
			break;
		case CHANNEL_RGBA:
			return 4;
			break;
        default: // CHANNEL_UNKNOWN を含む 
            return 0;
	}
}

inline ChannelType getTypeChannelFromNumElements(unsigned int numElements)
{
	switch (numElements) {
		case 1:
			return CHANNEL_GRAY;
		case 3:
			return CHANNEL_RGB;
		case 4:
			return CHANNEL_RGBA;
	}
	return CHANNEL_UNKNOWN;
}

#endif
