#ifndef __VARIANCE_H_INCLUDED
#define __VARIANCE_H_INCLUDED

#include <cmath>

/**
分散を求める
*/

class Variance
{
public:
	Variance() : m_numValues(0), m_sumValues(0.0f), m_sumSquare(0.0f) {}
	Variance(const Variance& rX) : m_numValues(rX.m_numValues),
		m_sumValues(rX.m_sumValues), m_sumSquare(rX.m_sumSquare) {}

	const Variance& operator=(const Variance& rX)
	{
		m_numValues = rX.m_numValues;
		m_sumValues = rX.m_sumValues;
		m_sumSquare = rX.m_sumSquare;

		return *this;
	}

	const Variance& operator+(const Variance& rX)
	{
		m_numValues += rX.m_numValues;
		m_sumValues += rX.m_sumValues;
		m_sumSquare += rX.m_sumSquare;
	}

	void addValue(float value)
	{
		m_sumValues += value;
		m_sumSquare += value * value;
		m_numValues ++;
	}

	unsigned int getNumValues() const { return m_numValues; }
	float getSumValues() const { m_sumValues; }
	float getSumSquare() const { return m_sumSquare; }

	float calculateAverage() const
	{
		return (m_numValues > 0) ? (m_sumValues / float(m_numValues)) : 0.0f;
	}

	float calculateVariance() const
	{
		float average = calculateAverage();
		return (m_numValues > 0) ? (m_sumSquare / m_numValues - average * average) : 0.0f;
	}

	float calculateDeviation() const { return std::sqrt(calculateVariance()); }

	float calculateScoreVariance(float value) const
	{
		float score = calculateScoreDeviation(value);
		return score * score;
	}

	float calculateScoreDeviation(float value) const
	{
		return value - calculateAverage();
	}

private:
	unsigned int m_numValues;
	float m_sumValues;
	float m_sumSquare;
};

#endif
