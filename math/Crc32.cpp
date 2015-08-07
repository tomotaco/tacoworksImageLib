#include "Crc32.h"

Crc32::Crc32()
{
	initializeTable();
}

void Crc32::initializeTable()
{
	if (m_tableCrcs.size() == 0) {
		m_tableCrcs.reserve(sizeTable);
	}
	for (int i = 0; i < sizeTable; i++) {
		unsigned int c = i;
		for (int j = 0; j < 8; j++) {
			c = (c & 1) ? (0xEDB88320 ^ (c >> 1)) : (c >> 1);
		}
		m_tableCrcs.push_back(c);
	}
}

unsigned int Crc32::calcCrc32(const std::string& rStr)
{
	unsigned int c = 0xffffffff;
	for (size_t i = 0; i < rStr.length(); i++) {
		c = m_tableCrcs[(c ^ rStr[i]) & 0xff] ^ (c >> 8);
	}
	return c ^ 0xffffffff;
}

unsigned int Crc32::calcCrc32(const std::wstring& rStr)
{
	unsigned int c = 0xffffffff;
	for (size_t i = 0; i < rStr.length(); i++) {
		wchar_t val = rStr[i];
		unsigned char valUpper = (val & 0xff00) >> 8;
		c = m_tableCrcs[(c ^ valUpper) & 0xff] ^ (c >> 8);
		unsigned char valLower = (val & 0xff);
		c = m_tableCrcs[(c ^ valLower) & 0xff] ^ (c >> 8);
	}
	return c ^ 0xffffffff;
}
