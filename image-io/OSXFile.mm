#import <UIKit/UIKit.h>
#import <ImageIO/ImageIO.h>
#import <CoreFoundation/CoreFoundation.h>

#include "PixelType.h"
#include "Image.h"
#include "ImageFactory.h"
#include "ImageUtil.h"

#include "OSXFile.h"

bool loadOSX(const char* pNameFile, ImageBase** ppImage)
{
	if (!(pNameFile && ppImage && !*ppImage)) return false;
	
	NSString* pStrImage = [NSString stringWithUTF8String:pNameFile];

	UIImage* pImage = [[UIImage alloc] initWithContentsOfFile:pStrImage];
	CGImageRef refImage = [pImage CGImage];
			
	unsigned int width = CGImageGetWidth(refImage);
	unsigned int height = CGImageGetHeight(refImage);
	size_t bpp = CGImageGetBitsPerPixel(refImage);
	size_t numBytesPerRow = CGImageGetBytesPerRow(refImage);


	// 生成する画像の種類の判別
	ElementType typeElement = ELEMENT_8;
	ChannelType typeChannel = CHANNEL_UNKNOWN;
	
	switch (bpp) {
		case 32:
			typeChannel = CHANNEL_RGBA;
		break;
		case 24:
			typeChannel = CHANNEL_RGB;
		break;
		case 8:
			typeChannel = CHANNEL_GRAY;
		break;
		default:
			typeChannel = CHANNEL_UNKNOWN;
		break;
	}
	if (typeChannel == CHANNEL_UNKNOWN) return false;

	ImageBase* pImageBase = ImageFactory::createImage(width, height, typeElement, typeChannel);
	
    CFDataRef refData = CGDataProviderCopyData(CGImageGetDataProvider(refImage));
    unsigned int numBytesData = CFDataGetLength(refData);
    
    CFRange range = CFRangeMake(0, CFDataGetLength(refData));
	unsigned char* pBufferRawDst = reinterpret_cast<unsigned char *>(pImageBase->getRawBuffer());
    CFDataGetBytes(refData, range, pBufferRawDst);
        
	CFRelease(refData);
    CGImageRelease(refImage);

	*ppImage = pImageBase;
	
    return true;
}

struct ToLower : public std::unary_function<char, char>
{
	result_type operator()(argument_type c)
	{
		return std::tolower(c);
	}
};

bool saveOSX(const char* pNameFile, const ImageBase* pImage)
{
	if (!(pNameFile && pImage)) return false;
	if (ImageUtil::isFloat(pImage)) return false;
	
	std::string nameFile(pNameFile);
	if (nameFile.length() < 4) return false;
	std::string ext = nameFile.substr(nameFile.length() - 4, 4);
	std::string extLower;
	ToLower funcToLower;
	std::transform(ext.begin(), ext.end(), std::back_inserter(extLower), funcToLower);

	CFStringRef typeImage = NULL;
	CFMutableDictionaryRef options = NULL;
	if (extLower == ".jpg") {
		typeImage = CFSTR("public.jpeg");
		options = CFDictionaryCreateMutable(nil, 0,
											&kCFTypeDictionaryKeyCallBacks,  &kCFTypeDictionaryValueCallBacks);
		
		float compression = 1.0f; // loss-less
		CFNumberRef compressionCF = CFNumberCreate(NULL, kCFNumberFloatType, &compression);
		CFDictionarySetValue(options, kCGImageDestinationLossyCompressionQuality, compressionCF);
	} else if (extLower == ".png") {
		typeImage = CFSTR("public.png");
	}

/* 	else if (extLower == ".tiff") {
		typeImage = CFSTR("public.tiff");
		int compression = CFTIFFCompressionLZW;  // non-lossy LZW compression
		options = CFDictionaryCreateMutable(nil, 0,
											&kCFTypeDictionaryKeyCallBacks,  &kCFTypeDictionaryValueCallBacks);
		CFMutableDictionaryRef tiffProfsMut = CFDictionaryCreateMutable(nil, 0,
												&kCFTypeDictionaryKeyCallBacks,  &kCFTypeDictionaryValueCallBacks);
		CFDictionarySetValue(tiffProfsMut, kCGImagePropertyTIFFCompression, CFNumberCreate(NULL, kCFNumberIntType, &compression));	
		CFDictionarySetValue(options, kCGImagePropertyTIFFDictionary, tiffProfsMut);
	}
*/
	CFStringRef nameFileCF = CFStringCreateWithCString(NULL, pNameFile, kCFStringEncodingUTF8);
	CFURLRef url = CFURLCreateWithFileSystemPathRelativeToBase(NULL,  nameFileCF, kCFURLPOSIXPathStyle, false, NULL);

	CFDataRef refData = CFDataCreate(NULL, reinterpret_cast<const UInt8*>(pImage->getRawBuffer()), pImage->getNumBytesBuffer());
	CGDataProviderRef provider = CGDataProviderCreateWithCFData(refData);
	CFRelease(refData);

	int numBytesPerElement = pImage->getNumBytesPerElement();
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGImageRef refImage = CGImageCreate(pImage->getWidth(), pImage->getHeight(),
			8 * pImage->getNumBytesPerElement(),
			8 * pImage->getNumBytesPixel(), pImage->getNumBytesRow(),
			colorSpace, kCGImageAlphaNone|kCGBitmapByteOrderDefault,
			provider, NULL, false, kCGRenderingIntentDefault);
	CGDataProviderRelease(provider);

	CGImageDestinationRef dstImage = CGImageDestinationCreateWithURL(url, typeImage, 1, NULL);
	CGImageDestinationAddImage(dstImage, refImage, options);
	CGImageDestinationFinalize(dstImage);
	CGImageRelease(refImage);

	return true;
}

