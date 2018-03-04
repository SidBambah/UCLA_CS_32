#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> attractions;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment seg;
	for (unsigned int i = 0; i < ml.getNumSegments(); i++) {
		ml.getSegment(i, seg);
		for (unsigned int z = 0; z < seg.attractions.size(); z++) {
			string lowercaseName;
			for (unsigned int j = 0; j < seg.attractions[z].name.size(); j++)
				lowercaseName += tolower(seg.attractions[z].name[j]);
			attractions.associate(lowercaseName, seg.attractions[z].geocoordinates);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	string lowercaseAttraction;
	for (unsigned int i = 0; i < attraction.size(); i++)
		lowercaseAttraction += tolower(attraction[i]);
	if(attractions.find(lowercaseAttraction) == nullptr)
		return false;
	gc = *(attractions.find(lowercaseAttraction));
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
