#include <math.h>
#include "MathUtil.h"
#include "RectPanZoom.h"

RectPanZoom::RectPanZoom()
: m_offset(0.0f, 0.0f), m_zoomLevel(0), m_isDiscreteZoom(false),
	m_zoomFactor(1.0f), m_centerRectSrc(0.0f, 0.0f), m_centerRectDst(0.0f, 0.0f)
{
	// Nothing to be impelmented.
}

void RectPanZoom::setRectSrc(const Rect2i& rSrc)
{
	m_rectSrc = rSrc;
	m_centerRectSrc.set(float(m_rectSrc.getLowerX() + m_rectSrc.getUpperX()) * 0.5f,
						float(m_rectSrc.getLowerY() + m_rectSrc.getUpperY()) * 0.5f);
}

void RectPanZoom::setRectDst(const Rect2i& rDst)
{
	m_rectDst = rDst;
	m_centerRectDst.set(float(m_rectDst.getLowerX() + m_rectDst.getUpperX()) * 0.5f,
						float(m_rectDst.getLowerY() + m_rectDst.getUpperY()) * 0.5f);
}

void RectPanZoom::setZoomLevel(int zoomLevel)
{
	if (m_isDiscreteZoom) {
		if (zoomLevel < 0) zoomLevel = 0;
	}
	float zoomFactorNew = calcZoomFactor(zoomLevel, m_isDiscreteZoom);
	m_offset = m_offset * (zoomFactorNew / m_zoomFactor);

	m_zoomLevel = zoomLevel;
	m_zoomFactor = zoomFactorNew;
}

void RectPanZoom::setZoomDiscrete(bool isDiscrete)
{
	if (isDiscrete == true) {
		if (m_zoomLevel < 0) {
			m_zoomLevel = 0;
		} else {
            m_zoomLevel = int(m_zoomFactor) + 1;
		}
	}
	m_isDiscreteZoom = isDiscrete;
}

void RectPanZoom::setZoomFactor(float zoomFactor)
{
	m_offset = m_offset * (zoomFactor / m_zoomFactor);
	// 計算誤差と、zoomLevel は整数という定義から、
	// 与えられた zoomFactor に一致するとは限らない
	m_zoomLevel = int(log(zoomFactor) / log(1.125f));
	m_zoomFactor = zoomFactor;
}

Vector2f RectPanZoom::getPointSrcCoord(const Vector2f& rPointDstCoord) const
{
	return (rPointDstCoord - m_centerRectDst - m_offset) / m_zoomFactor + m_centerRectSrc; // - Vector2f(0.5f, 0.5f);
}

Vector2f RectPanZoom::getPointDstCoord(const Vector2f& rPointSrcCoord) const
{
	return (rPointSrcCoord - m_centerRectSrc /* + Vector2f(0.5f, 0.5f) */) * m_zoomFactor + m_offset + m_centerRectDst;
}

void RectPanZoom::getRectsVacantVisible(std::vector<Rect2i>& rListRectsVacant)
{
	rListRectsVacant.clear();

	Rect2f rect(getPointDstCoord(m_rectSrc.getLower()), getPointDstCoord(m_rectSrc.getUpper()));
	Rect2i rectSrcOnDst(roundInt(rect.getLowerX()), roundInt(rect.getLowerY()), roundInt(rect.getUpperX()), roundInt(rect.getUpperY()));

	// 描画領域写像した画像が全く見えない場合は、全画面を返す
	Rect2i rectIntersection = rectSrcOnDst * m_rectDst;
	if (rectIntersection.getWidth() == 0 && rectIntersection.getHeight() == 0) {
		rListRectsVacant.push_back(m_rectDst);
		return;
	}

	// 画像の周囲の領域を以下のように定義する。
	// 0 |  1   | 2
	//---+------+---
	// 3 | 画像 | 4
	//---+------+---
	// 5 |  6   | 7

	// 上記の各領域について、見えてるかどうかを　8bit で表現。
	// (1 = 見えている, 0 = 見えない)
	// 初期状態は全て見えているものとする。

	unsigned int flagVisible = 0xff;
	const unsigned int bit0 = 0x01;
	const unsigned int bit1 = 0x01 << 1;
	const unsigned int bit2 = 0x01 << 2;
	const unsigned int bit3 = 0x01 << 3;
	const unsigned int bit4 = 0x01 << 4;
	const unsigned int bit5 = 0x01 << 5;
	const unsigned int bit6 = 0x01 << 6;
	const unsigned int bit7 = 0x01 << 7;

	unsigned int bitsLeftside  = (bit0 | bit3 | bit5);
	unsigned int bitsRightside = (bit2 | bit4 | bit7);
	unsigned int bitsAbove     = (bit0 | bit1 | bit2);
	unsigned int bitsBelow     = (bit5 | bit6 | bit7);

	// 見えない領域のビットを削っていく。
	if (rectSrcOnDst.getLowerX() < m_rectDst.getLowerX()) flagVisible &= ‾bitsLeftside;
	if (m_rectDst.getUpperX() < rectSrcOnDst.getUpperX()) flagVisible &= ‾bitsRightside; 
	if (rectSrcOnDst.getLowerY() < m_rectDst.getLowerY()) flagVisible &= ‾bitsAbove;
	if (m_rectDst.getUpperY() < rectSrcOnDst.getUpperY()) flagVisible &= ‾bitsBelow;


	// 残ったビットに対応する領域を追加(まとめられるものはまとめて追加)
	if ((flagVisible & bitsLeftside) == bitsLeftside) {
		rListRectsVacant.push_back(
			Rect2i(m_rectDst.getLowerX(), m_rectDst.getLowerY(), rectSrcOnDst.getLowerX(), m_rectDst.getUpperY()));
		flagVisible &= ‾bitsLeftside;
	}
	if ((flagVisible & bitsRightside) == bitsRightside) {
		rListRectsVacant.push_back(
			Rect2i(rectSrcOnDst.getUpperX(), m_rectDst.getLowerY(), m_rectDst.getUpperX(), m_rectDst.getUpperY()));
		flagVisible &= ‾bitsRightside;
	}
	if ((flagVisible & bitsAbove) == bitsAbove) {
		rListRectsVacant.push_back(
			Rect2i(m_rectDst.getLowerX(), m_rectDst.getLowerY(), m_rectDst.getUpperX(), rectSrcOnDst.getLowerY()));
		flagVisible &= ‾bitsAbove;
	}
	if ((flagVisible & bitsBelow) == bitsBelow) {
		rListRectsVacant.push_back(
			Rect2i(m_rectDst.getLowerX(), rectSrcOnDst.getUpperY(), m_rectDst.getUpperX(), m_rectDst.getUpperY()));
		flagVisible &= ‾bitsBelow;
	}

	// 残ったビットに対応する領域を追加(1領域ずつ追加)
	if (flagVisible & bit0) {
		Rect2i rect(m_rectDst.getLowerX(), m_rectDst.getLowerY(), rectSrcOnDst.getLowerX(), rectSrcOnDst.getLowerY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
	if (flagVisible & bit1) {
		Rect2i rect(rectSrcOnDst.getLowerX(), m_rectDst.getLowerY(), rectSrcOnDst.getUpperX(), rectSrcOnDst.getLowerY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
	if (flagVisible & bit2) {
		Rect2i rect(rectSrcOnDst.getUpperX(), m_rectDst.getLowerY(), m_rectDst.getUpperX(), rectSrcOnDst.getLowerY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
	if (flagVisible & bit3) {
		Rect2i rect(m_rectDst.getLowerX(), rectSrcOnDst.getLowerY(), rectSrcOnDst.getLowerX(), rectSrcOnDst.getUpperY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
	if (flagVisible & bit4) {
		Rect2i rect(rectSrcOnDst.getUpperX(), rectSrcOnDst.getLowerY(), m_rectDst.getUpperX(), rectSrcOnDst.getUpperY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
	if (flagVisible & bit5) {
		Rect2i rect(m_rectDst.getLowerX(), rectSrcOnDst.getUpperY(), rectSrcOnDst.getLowerX(), m_rectDst.getUpperY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
	if (flagVisible & bit6) {
		Rect2i rect(rectSrcOnDst.getLowerX(), rectSrcOnDst.getUpperY(), rectSrcOnDst.getUpperX(), m_rectDst.getUpperY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
	if (flagVisible & bit7) {
		Rect2i rect(rectSrcOnDst.getUpperX(), rectSrcOnDst.getUpperY(), m_rectDst.getUpperX(), m_rectDst.getUpperY());
		rListRectsVacant.push_back(rect * m_rectDst);
	}
}

float RectPanZoom::calcZoomFactor(int zoomLevel, bool isDiscrete)
{
	if (zoomLevel == 0) return 1.0f;

	if (isDiscrete) {
		if (zoomLevel < 0) return 1.0f;
		return float(zoomLevel) + 1.0f;
	} else {
		float zoomFactor = 1.0f;
		if (zoomLevel > 0) {
			for (int i = 0; i < zoomLevel; i ++) zoomFactor *= 1.125f;
		} else {
			for (int i = 0; i < -zoomLevel; i ++) zoomFactor /= 1.125f;
		}
		return zoomFactor;
	}
}

Rect2f RectPanZoom::getRectOnDstCoord(const Rect2f& rRectOnSrc) const
{
	return Rect2f(getPointDstCoord(rRectOnSrc.getLower()), getPointDstCoord(rRectOnSrc.getUpper()));
}

Rect2f RectPanZoom::getRectOnSrcCoord(const Rect2f& rRectOnDst) const
{
	return Rect2f(getPointSrcCoord(rRectOnDst.getLower()), getPointSrcCoord(rRectOnDst.getUpper()));
}

void RectPanZoom::fitToBoundary()
{
	Rect2f rectSrcOnDstCoord = getRectSrcOnDstCoord();
	Rect2i rectDst = getRectDst();

	if (rectDst.getSizeX() < rectSrcOnDstCoord.getSizeX()) {
		if (rectDst.getLowerX() < rectSrcOnDstCoord.getLowerX()) {
			m_offset.setX(m_offset.getX() - rectSrcOnDstCoord.getLowerX() + rectDst.getLowerX());
		}
		if (rectSrcOnDstCoord.getUpperX() < rectDst.getUpperX()) {
			m_offset.setX(m_offset.getX() + rectDst.getUpperX() - rectSrcOnDstCoord.getUpperX());
		}
	} else { // rectSrcOnDstCoord.getSizeX() < rectDst.getSizeX()
		if (rectSrcOnDstCoord.getLowerX() < rectDst.getLowerX()) {
			m_offset.setX(m_offset.getX() + rectDst.getLowerX() - rectSrcOnDstCoord.getLowerX());
		}
		if (rectDst.getUpperX() < rectSrcOnDstCoord.getUpperX()) {
			m_offset.setX(m_offset.getX() - rectSrcOnDstCoord.getUpperX() + rectDst.getUpperX());
		}
	}
	if (rectDst.getSizeY() < rectSrcOnDstCoord.getSizeY()) {
		if (rectDst.getLowerY() < rectSrcOnDstCoord.getLowerY()) {
			m_offset.setY(m_offset.getY() - rectSrcOnDstCoord.getLowerY() + rectDst.getLowerY());
		}
		if (rectSrcOnDstCoord.getUpperY() < rectDst.getUpperY()) {
			m_offset.setY(m_offset.getY() + rectDst.getUpperY() - rectSrcOnDstCoord.getUpperY());
		}
	} else { // rectSrcOnDstCoord.getSizeY() < rectDst.getSizeY()
		if (rectSrcOnDstCoord.getLowerY() < rectDst.getLowerY()) {
			m_offset.setY(m_offset.getY() + rectDst.getLowerY() - rectSrcOnDstCoord.getLowerY());
		}
		if (rectDst.getUpperY() < rectSrcOnDstCoord.getUpperY()) {
			m_offset.setY(m_offset.getY() - rectSrcOnDstCoord.getUpperY() + rectDst.getUpperY());
		}
	}
}

void RectPanZoom::fitToZoom()
{
	// パネルに収まるように表示領域を拡大縮小(アスペクト比は維持)
	int widthSrc = m_rectSrc.getWidth();
	int heightSrc = m_rectSrc.getHeight();
	int widthDst = m_rectDst.getWidth();
	int heightDst = m_rectDst.getHeight();
	float aspectSrc = float(heightSrc) / float(widthSrc);
	float aspectDst = float(heightDst) / float(widthDst);

	if (aspectDst < aspectSrc) {
		// パネルが横長→高さが一致
		setZoomFactor(float(heightDst) / float(heightSrc));
	} else {
		// パネルが縦長→横幅が一致
		setZoomFactor(float(widthDst) / float(widthSrc));
	}
}

