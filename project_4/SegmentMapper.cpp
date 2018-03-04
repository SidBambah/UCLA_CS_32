#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>> segmentMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (unsigned int i = 0; i < ml.getNumSegments(); i++) {
		vector<StreetSegment> start;
		vector<StreetSegment> end;
		StreetSegment seg;
		ml.getSegment(i, seg);
		if (segmentMap.find(seg.segment.start) == nullptr) {
			start.push_back(seg);
			segmentMap.associate(seg.segment.start, start);
		}
		else {
			segmentMap.find(seg.segment.start)->push_back(seg);
		}
		if (segmentMap.find(seg.segment.end) == nullptr) {
			end.push_back(seg);
			segmentMap.associate(seg.segment.start, start);
		}
		else {
			segmentMap.find(seg.segment.end)->push_back(seg);
		}
		for (unsigned int i = 0; i < seg.attractions.size(); i++){
			vector<StreetSegment> attractions;
			if (segmentMap.find(seg.attractions[i].geocoordinates) == nullptr) {
				attractions.push_back(seg);
				segmentMap.associate(seg.attractions[i].geocoordinates, attractions);
			}
			else {
				segmentMap.find(seg.attractions[i].geocoordinates)->push_back(seg);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> emptyVector;
	if (segmentMap.find(gc) == nullptr)
		return emptyVector;
	return *(segmentMap.find(gc));
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
