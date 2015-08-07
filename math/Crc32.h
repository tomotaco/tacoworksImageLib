#ifndef Crc32H
#define Crc32H

#include <vector>

class Crc32
{
public:
	const static int sizeTable = 256;
	Crc32();

	unsigned int calcCrc32(const std::string& rStr);
	unsigned int calcCrc32(const std::wstring& rStr);

private:
	void initializeTable();

	std::vector<unsigned int> m_tableCrcs;
};

#endif
