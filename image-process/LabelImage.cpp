#include <list>
#include <set>

#undef OUTPUT_LABEL_IMAGE

#ifdef OUTPUT_LABEL_IMAGE
#include "ImageAccessor.h"
#include "BitmapFile.h"
#endif // OUTPUT_LABEL_IMAGE

#include "Vector2f.h"
#include "ImageUInt.h"
#include "FindLabelInfo.h"
#include "LabelRect.h"
#include "LabelImage.h"

inline bool isPixelToBeLabeled(unsigned int value, unsigned int valueToLabel)
{
	return (value == valueToLabel) ? true : false;
}

namespace {
void findSearchArea(const ImageGray8& rImageSrc, unsigned int valueToLabel,
					int xStart, int xEnd, int yCurrent, int yDir,
					ImageUInt& rImageLabeled, std::set<FindLabelInfo>& rSetFinders)
{
	int widthImage = rImageSrc.getWidth();
	int heightImage = rImageSrc.getHeight();

	int yNeighbor = yCurrent + yDir;
	if (!(0 <= yNeighbor && yNeighbor < heightImage)) return;

	// 直上/直下の点が探索領域かどうか
	unsigned int valueSrcCurrent = rImageSrc.getPixel(xStart, yNeighbor).getA();
	unsigned int valueLabelCurrent = rImageLabeled.getPixel(xStart, yNeighbor).getA();
	bool foundSearchAreaCurrent = false;
	if (isPixelToBeLabeled(valueSrcCurrent, valueToLabel) && valueLabelCurrent == 0) {
		FindLabelInfo info(Point2i(xStart, yNeighbor), yDir);
		rSetFinders.insert(info);
		foundSearchAreaCurrent = true;
	}

	// 直上/直下から xDir 方向に隣の点から探索を開始
	bool foundSearchAreaPrev = foundSearchAreaCurrent;
	for (int xNeighbor = xStart + 1; xNeighbor != xEnd; xNeighbor ++) {
		// 一つ上または下のピクセルを参照して次回の探索領域を登録する
		unsigned int valueSrcNeighbor = rImageSrc.getPixel(xNeighbor, yNeighbor).getA();
		unsigned int valueLabelNeighbor = rImageLabeled.getPixel(xNeighbor, yNeighbor).getA();
		foundSearchAreaCurrent = (isPixelToBeLabeled(valueSrcNeighbor, valueToLabel) && valueLabelNeighbor == 0) ? true : false;
		if (foundSearchAreaCurrent && !foundSearchAreaPrev) {
			FindLabelInfo info(Point2i(xNeighbor, yNeighbor), yDir);
			rSetFinders.insert(info);
		}

		foundSearchAreaPrev = foundSearchAreaCurrent;
	}
}
}

void LabelImage::labelImage(const ImageGray8& rImageSrc, unsigned int valueToLabel,
							ImageUInt& rImageLabeled, std::list<LabelRect>& rListRectsLabel)
{
	rListRectsLabel.clear();

	int widthImage = rImageSrc.getWidth();
	int heightImage = rImageSrc.getHeight();

	rImageLabeled.setSize(widthImage, heightImage);
	rImageLabeled.fillPixel(PixelUInt(0));
	Rect2i rectImage = rImageSrc.getRect();

#ifdef OUTPUT_LABEL_IMAGE
	// Debug 用
	ImageRGB8 imageTemp(widthImage, heightImage);
	ImageAccessorReadOnly<ImageGray8, PixelRGB8> accessor(&rImageSrc);
	std::copy(accessor.range_begin(rectImage), accessor.range_end(rectImage), imageTemp.range_begin(rectImage));
#endif // OUTPUT_LABEL_IMAGE

	unsigned int indexLabel = 1; // 0 は何もなしの意味。ラベルインデックスは 1 からスタート
	ImageGray8::const_iterator itSrc = rImageSrc.range_begin(rectImage);
	for (; itSrc != rImageSrc.range_end(rectImage); ++ itSrc) {
		
		// 黒領域でないか、すでにラベル付けされているならスキップ
		if (!isPixelToBeLabeled((*itSrc).getA(), valueToLabel) || rImageLabeled.getPixel(itSrc.getPosition()).getA() > 0) continue;
		
		LabelRect rectLabel;
		std::set<FindLabelInfo> setFinders;

		// 現在地点から右方向への探索パケットを追加
		FindLabelInfo info(itSrc.getPosition(), 1);
		setFinders.insert(info);

		while (setFinders.size() > 0) {
			std::set<FindLabelInfo>::iterator itTop = setFinders.begin();
			FindLabelInfo infoFind = *itTop;
			setFinders.erase(itTop);

			// 左方向に行き止まりまで探索
			int xCurrent = infoFind.getPosition().getX();
			int yCurrent = infoFind.getPosition().getY();
			int xBoundLeft = xCurrent;
			while (0 <= xBoundLeft) {
				int valueSrcCurrent = rImageSrc.getPixel(xBoundLeft, yCurrent).getA();
				if (!isPixelToBeLabeled(valueSrcCurrent, valueToLabel)) break;
				rImageLabeled.setPixel(xBoundLeft, yCurrent, PixelUInt(indexLabel));

				rectLabel.addPoint(Point2i(xBoundLeft, yCurrent));

#ifdef OUTPUT_LABEL_IMAGE
				imageTemp.setPixel(xBoundLeft, yCurrent, PixelRGB8(255, 0, 0)); // Debug
#endif // OUTPUT_LABEL_IMAGE
				xBoundLeft --;
			}

			// 右方向に行き止まりまで探索
			int xBoundRight = xCurrent + 1;
			while (xBoundRight < widthImage) {
				int valueSrcCurrent = rImageSrc.getPixel(xBoundRight, yCurrent).getA();
				if (!isPixelToBeLabeled(valueSrcCurrent, valueToLabel)) break;
				rImageLabeled.setPixel(xBoundRight, yCurrent, PixelUInt(indexLabel));

				rectLabel.addPoint(Point2i(xBoundRight, yCurrent));

#ifdef OUTPUT_LABEL_IMAGE
				imageTemp.setPixel(xBoundRight, yCurrent, PixelRGB8(255, 0, 0)); // Debug
#endif // OUTPUT_LABEL_IMAGE
				xBoundRight ++;
			}

			int xStart = xBoundLeft + 1;
			int xEnd = xBoundRight;
			int yDir = infoFind.getDirectionSearchY();
			findSearchArea(rImageSrc, valueToLabel, xStart, xEnd, yCurrent, yDir, rImageLabeled, setFinders);
			findSearchArea(rImageSrc, valueToLabel, xStart, xEnd, yCurrent, -yDir, rImageLabeled, setFinders);

/*			// 探索点をプロット
			std::set<FindLabelInfo>::const_iterator itFinder = setFinders.begin();
			for (; itFinder != setFinders.end(); ++ itFinder) {
				imageTemp.setPixel((*itFinder).getPosition(), PixelRGB8(0, 255, 0));
			}
*/
		}

		if (rectLabel.getNumPoints() > 0) {
			rectLabel.updateCOG();
			rListRectsLabel.push_back(rectLabel);
		}
				
#ifdef OUTPUT_LABEL_IMAGE
		saveBitmap("C:¥¥Users¥¥logion¥¥development¥¥tetsutalow¥¥images¥¥tmp¥¥label.bmp", &imageTemp);
#endif // OUTPUT_LABEL_IMAGE
		indexLabel ++;
	}
}

void LabelImage::colorLabeledImage(const ImageUInt& rImageLabeled, const std::list<LabelRect>& rListRectsLabel, ImageRGB8& rImageColored)
{
	// ラベル付けされた画像を色づけして出力(デバッグ用)
	rImageColored.setSize(rImageLabeled.getWidth(), rImageLabeled.getHeight());
	rImageColored.fillPixel(PixelRGB8(0, 0, 0));

	const unsigned int numLabelColors = 6;
	PixelRGB8 tablePixelLabels[numLabelColors] = { PixelRGB8(0, 0, 128), PixelRGB8(128, 0, 0), PixelRGB8(128, 0, 128),
									PixelRGB8(0, 128, 0), PixelRGB8(0, 128, 128), PixelRGB8(128, 128, 0) };
	Rect2i rectLabeled = rImageLabeled.getRect();
	ImageUInt::const_iterator itLabeled = rImageLabeled.range_begin(rectLabeled);
	ImageRGB8::iterator itLabeledColor = rImageColored.range_begin(rectLabeled);
	for (; itLabeled != rImageLabeled.range_end(rectLabeled); ++ itLabeled, ++ itLabeledColor) {
		unsigned int valueLabel = (*itLabeled).getA();
		if (valueLabel == 0) continue;
		*itLabeledColor = tablePixelLabels[(valueLabel - 1) % numLabelColors];
	}
	std::list<LabelRect>::const_iterator itRectLabel = rListRectsLabel.begin();
	for (; itRectLabel != rListRectsLabel.end(); ++ itRectLabel) {
		const Vector2f& rPointCOG = (*itRectLabel).getCOG();
		rImageColored.setPixel(roundInt(rPointCOG.getX()), roundInt(rPointCOG.getY()), PixelRGB8(255, 255, 255));
		const Rect2i& rRect = (*itRectLabel).getBoundingBox();
		for (unsigned int x = rRect.getLowerX(); x <= rRect.getUpperX(); x ++) {
			rImageColored.setPixel(x, rRect.getLowerY(), PixelRGB8(255, 0, 0));
			rImageColored.setPixel(x, rRect.getUpperY(), PixelRGB8(255, 0, 0));
		}
		for (unsigned int y = rRect.getLowerY(); y < rRect.getUpperY(); y ++) {
			rImageColored.setPixel(rRect.getLowerX(), y, PixelRGB8(255, 0, 0));
			rImageColored.setPixel(rRect.getUpperX(), y, PixelRGB8(255, 0, 0));
		}
	}
}