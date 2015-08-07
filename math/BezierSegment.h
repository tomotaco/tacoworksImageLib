#ifndef __BEZIER_SEGMENT_H_INCLUDED
#define __BEZIER_SEGMENT_H_INCLUDED

template<class T>
class BezierSegment
{
public:
	enum { numCPs = 4 };
	BezierSegment();
	BezierSegment(const BezierSegment& rX)
	{
		for (int i = 0; i < numCPs; i ++) m_cp[i] = rX.m_cp[i];
	}
	BezierSegment(const T* pCP)
	{
		for (int i = 0; i < numCPs; i ++) m_cp[i] = pCP[i];
	}

	const BezierSegment& operator=(const BezierSegment& rX)
	{
		for (int i = 0; i < numCPs; i ++) m_cp[i] = rX.m_cp[i];
		return *this;
	}

	const T&	getCP(int index) const
	{
		assert(index < 0 || numCPs <= index);
		return m_cp[index];
	}
	T	getPoint(float t) const
	{
		float invt = 1.0f - t;
		return (invt * invt * invt) * m_cp[0] + 3.0f * (invt * invt * t) * m_cp[1] + 3.0f * (invt * t * t) * m_cp[2] + (t * t * t) * m_cp[3];
	}

private:
	T m_cp[numCPs];
};

#endif // __BEZIER_SEGMENT_H_INCLUDED