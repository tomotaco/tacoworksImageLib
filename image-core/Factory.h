#ifndef FactoryH
#define FactoryH

#include <map>

template <class AbstractProduct, class IdentifierType,
			class ProductCreator = AbstractProduct* (*)(),
			class ProductDuplicator = AbstractProduct* (*)(const AbstractProduct*) >
class Factory
{
public:
	bool registerCreator(const IdentifierType& rID, ProductCreator creator, ProductDuplicator duplicator)
	{
		return m_mapCreators.insert(std::make_pair(rID, std::make_pair(creator, duplicator))).second;
	}

	bool unRegisterCreator(const IdentifierType& rID)
	{
		return m_mapCreators.erase(rID) == 1;
	}

	AbstractProduct* createObject(const IdentifierType& rID)
	{
		typename std::map<IdentifierType, std::pair<ProductCreator, ProductDuplicator> >::const_iterator it = m_mapCreators.find(rID);
		return (it == m_mapCreators.end()) ? NULL : ((*it).second.first)();
	}

	AbstractProduct* duplicateObject(const IdentifierType& rID, const AbstractProduct* pX)
	{
		typename std::map<IdentifierType, std::pair<ProductCreator, ProductDuplicator> >::const_iterator it = m_mapCreators.find(rID);
		return (it == m_mapCreators.end()) ? NULL : ((*it).second.second)(pX);
	}

	bool isEmpty() const
	{
		return (m_mapCreators.size() == 0) ? true : false;
    }
	
private:
	std::map<IdentifierType, std::pair<ProductCreator, ProductDuplicator> > m_mapCreators;
};

#endif
