#ifndef __VECTOR_VARIANCE_H_INCLUDED
#define __VECTOR_VARIANCE_H_INCLUDED

#include <assert.h>

/**
Vector2f の分散を求める
*/

template<typename TypeVector>
class VectorVariance
{
public:
	VectorVariance() : m_numVectors(0), m_sumSquare(0.0f) {}
	VectorVariance(const VectorVariance& rX) : m_numVectors(rX.m_numVectors),
		m_sumVector(rX.m_sumVector), m_sumSquare(rX.m_sumSquare) {}

	const VectorVariance& operator=(const VectorVariance& rX)
	{
		m_numVectors = rX.m_numVectors;
		m_sumVector = rX.m_sumVector;
		m_sumSquare = rX.m_sumSquare;

		return *this;
	}

	void addVector(const TypeVector& rV)
	{
		m_sumVector += rV;
		m_sumSquare += rV.getLengthSquared();
		m_numVectors ++;
	}

	unsigned int getNumVectors() const { return m_numVectors; }
	const TypeVector& getSumVector() const { m_sumVector; }
	float getSumSquare() const { return m_sumSquare; }

	TypeVector calculateAverage() const
	{
		return (m_numVectors > 0) ? (m_sumVector / float(m_numVectors)) : Vector2f(0.0f, 0.0f);
	}

	float calculateVariance() const
	{
		return (m_numVectors > 0) ? (m_sumSquare / m_numVectors - calculateAverage().getLengthSquared()) : 0.0f;
	}

	float calculateDeviation() const { return std::sqrt(calculateVariance()); }

	float calculateScoreVariance(const TypeVector& rV) const
	{
		return (rV - calculateAverage()).getLengthSquared();
	}

	float calculateScoreDeviation(const TypeVector& rV) const
	{
		return (rV - calculateAverage()).getLength();
	}

private:
	unsigned int m_numVectors;
	TypeVector m_sumVector;
	float m_sumSquare;
};

#endif
