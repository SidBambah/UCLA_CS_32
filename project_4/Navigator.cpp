#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
#include <vector>
#include <iostream>
#include <list>
using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	struct Node {
		Node(StreetSegment seg, Node* parent, GeoCoord location) : m_seg(seg), m_parent(parent), m_location(location) { f = 0; g = 0; h = 0; }
		Node* m_parent;
		StreetSegment m_seg;
		GeoCoord m_location;
		double f, g, h;
	};
	void A_star(string start, string end) const;
	Node* leastF(list<Node*> open_list) const;
	bool isInList(Node* toCheck, list<Node*> closed_list) const;
	bool isGoal(Node* toCheck, string end) const;
	AttractionMapper m_amapper;
	SegmentMapper m_smapper;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	MapLoader ml;
	ml.load(mapFile);
	m_amapper.init(ml);
	m_smapper.init(ml);
	return true;
}

NavigatorImpl::Node* NavigatorImpl::leastF(list<Node*> open_list) const{
	list<Node*>::iterator ic = open_list.begin();
	Node* leastF = open_list.front();
	while (ic != open_list.end()) {
		if ((*ic)->f < leastF->f)
			leastF = (*ic);
		ic++;
	}
	return leastF;
}

bool NavigatorImpl::isInList(Node* toCheck, list<Node*> closed_list) const{
	list<Node*>::iterator ic = closed_list.begin();
	while (ic != closed_list.end()) {
		if (toCheck == (*ic))
			return true;
	}
	return false;
}


bool NavigatorImpl::isGoal(Node* toCheck, string end) const {
	GeoCoord endCoord;
	m_amapper.getGeoCoord(end, endCoord);
	if (toCheck->m_seg.attractions.size() > 0) {
		for (int i = 0; i < toCheck->m_seg.attractions.size(); i++) {
			if (toCheck->m_seg.attractions[i].geocoordinates == endCoord)
				return true;
		}
	}
	return false;
}

void NavigatorImpl::A_star(string start, string end) const{
	//Initializing
	list<Node*> open_list;
	list<Node*> closed_list;
	GeoCoord startCoord;
	GeoCoord endCoord;
	m_amapper.getGeoCoord(start, startCoord);
	m_amapper.getGeoCoord(end, endCoord);
	vector<StreetSegment> startSeg = m_smapper.getSegments(startCoord);
	for (int i = 0; i < startSeg.size(); i++) {
		Node* toAdd = new Node(startSeg[i], nullptr, startCoord);
		toAdd->g = 0;
		toAdd->h = distanceEarthMiles(startCoord, endCoord);
		toAdd->f = toAdd->g + toAdd->h;
		open_list.push_back(toAdd);
	}
	m_amapper.getGeoCoord(start, startCoord);
	m_amapper.getGeoCoord(end, endCoord);
	//Making Babies
	while (!open_list.empty()) {
		Node* q = leastF(open_list);
		cout << "got here" << endl;
		open_list.remove(q);
		vector<StreetSegment> children = m_smapper.getSegments(q->m_seg.segment.start);
		vector<Node*> babies;
		for (int i = 0; i < children.size(); i++) {
			babies.push_back(new Node(children[i], q, q->m_seg.segment.start));
			babies[i]->g = q->g + distanceEarthMiles(babies[i]->m_location, q->m_location);
			babies[i]->h = distanceEarthMiles(babies[i]->m_location, endCoord);
			babies[i]->f = babies[i]->g + babies[i]->h;
			if (isGoal(babies[i], end)) {
				cout << "yes" << endl;
				return;                       //for testing only
			}
			if (isInList(babies[i], closed_list) || isInList(babies[i], open_list)) {
				delete babies[i];
				continue;
			}
			open_list.push_back(babies[i]);
		}
		vector<StreetSegment> children_end = m_smapper.getSegments(q->m_seg.segment.end);
		for (int i = 0; i < children_end.size(); i++) {
			Node* child = new Node(children_end[i], q, q->m_seg.segment.start);
			child->g = q->g + distanceEarthMiles(child->m_location, q->m_location);
			child->h = distanceEarthMiles(child->m_location, endCoord);
			child->f = child->g + child->h;
			if (isGoal(child, end)) {
				cout << "yes" << endl;
				return;
			}
			if (isInList(child, closed_list) || isInList(child, open_list)) {
				delete child;
				continue;
			}
			open_list.push_back(child);
		}
		closed_list.push_back(q);
	}
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	if (!directions.empty())
		directions.clear();
	GeoCoord temp;
	if (m_amapper.getGeoCoord(start, temp) == false)
		return NAV_BAD_SOURCE;
	if (m_amapper.getGeoCoord(end, temp) == false)
		return NAV_BAD_DESTINATION;
	A_star(start, end);
	if(directions.empty())
		return NAV_NO_ROUTE;
	return NAV_SUCCESS;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}