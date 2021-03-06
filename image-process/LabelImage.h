#ifndef __LABEL_IMAGE_H_INCLUDED
#define __LABEL_IMAGE_H_INCLUDED

#include <list>
#include "Image.h"
#include "ImageUInt.h"
#include "LabelRect.h"

/**
	二値画像をもとにラベリング
*/
namespace LabelImage
{
	// rImageSrc には 0 か 255 のいずれかの値が入っているものとする
	// rImageDst の値の意味: 0 ... 何もなし、1以降 ... ラベルのインデックス
	// (unsigned int の最大値 - 1 以上の種類は保持できない仕様)
	void labelImage(const ImageGray8& rImageSrc, unsigned int valueToLabel,
		ImageUInt& rImageLabeled, std::list<LabelRect>& rListRectsLabel);

	// ラベル付けされた画像を色づけする(主にデバッグ用)
	void colorLabeledImage(const ImageUInt& rImageLabeled, const std::list<LabelRect>& rListRectsLabel,
		ImageRGB8& rImageColored);
};

#endif
