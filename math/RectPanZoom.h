#ifndef __RECT_PAN_ZOOM_H_INCLUDED
#define __RECT_PAN_ZOOM_H_INCLUDED

#include <vector>

#include "Rect2i.h"
#include "Rect2f.h"
#include "Point2i.h"
#include "Vector2f.h"

class RectPanZoom {
public:
	RectPanZoom();

	void			setRectSrc(const Rect2i& rSrc);
	void			setRectDst(const Rect2i& rDst);
	const Rect2i&	getRectSrc() const { return m_rectSrc; }
	const Rect2i&	getRectDst() const { return m_rectDst; }

	void	setZoomLevel(int zoomLevel);
	int		getZoomLevel() const { return m_zoomLevel; }
	void	setZoomDiscrete(bool isDiscrete);
	bool	getZoomDiscrete() const { return m_isDiscreteZoom; }
	void	setZoomFactor(float zoomFactor);
	float	getZoomFactor() const { return m_zoomFactor; }
	void	setOffset(const Vector2f& rOffset) { m_offset = rOffset; }
	const Vector2f&	getOffset() const { return m_offset; }

	Vector2f getPointSrcCoord(const Vector2f& rPointDstCoord) const;
	Vector2f getPointDstCoord(const Vector2f& rPointSrcCoord) const;
	Vector2f getPointSrcCoord(const Point2i& rPointDstCoord) const
	{
		return getPointSrcCoord(Vector2f(float(rPointDstCoord.getX()), float(rPointDstCoord.getY())));
	}
	Vector2f getPointDstCoord(const Point2i& rPointSrcCoord) const
	{
		return getPointDstCoord(Vector2f(float(rPointSrcCoord.getX()), float(rPointSrcCoord.getY())));
	}

	Rect2f getRectOnDstCoord(const Rect2f& rRectOnSrc) const;
	Rect2f getRectOnSrcCoord(const Rect2f& rRectOnDst) const;

	Rect2f getRectSrcOnDstCoord() const
	{
		return getRectOnDstCoord(Rect2f(float(m_rectSrc.getLowerX()), float(m_rectSrc.getLowerY()),
										float(m_rectSrc.getUpperX()), float(m_rectSrc.getUpperY())));
	}
	Rect2f getRectDstOnSrcCoord() const
	{
		return getRectOnSrcCoord(Rect2f(float(m_rectDst.getLowerX()), float(m_rectDst.getLowerY()),
										float(m_rectDst.getUpperX()), float(m_rectDst.getUpperY())));
    }
	void getRectsVacantVisible(std::vector<Rect2i>& rListRectsVacant);

	void fitToBoundary();
	void fitToZoom();

	static float calcZoomFactor(int zoomLevel, bool isDiscrete = false);

	Vector2f	getCenterRectSrc() const { return m_centerRectSrc; }
	Vector2f	getCenterRectDst() const { return m_centerRectDst; }

private:
	// Fundamental attributes
	int m_zoomLevel;
	bool m_isDiscreteZoom;
	Vector2f	m_offset; // Offset of center of srcRect from that of dstRect
	Rect2i	m_rectSrc;
	Rect2i	m_rectDst;

	// Cached values calculated from fundamental attributes
	float m_zoomFactor;
	Vector2f m_centerRectSrc;
	Vector2f m_centerRectDst;

};

#endif // __RECT_PAN_ZOOM_H_INCLUDED