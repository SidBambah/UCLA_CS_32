#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> m_segments;
	int countLines(istream& inf);
};

MapLoaderImpl::MapLoaderImpl() {
}

MapLoaderImpl::~MapLoaderImpl() {
}

int MapLoaderImpl::countLines(istream& inf) {
	int lineCount = 0;
	string line;
	while (getline(inf, line))
		lineCount++;
	inf.clear();
	inf.seekg(0, ios::beg);
	return lineCount;
}

bool MapLoaderImpl::load(string mapFile) {
	ifstream infile(mapFile);
	if (!infile)
		return false;
	int looper = 0;
	int numLines = countLines(infile);
	while (looper < numLines) {
		StreetSegment toAdd;
		string streetName;
		getline(infile, streetName);
		looper++;
		toAdd.streetName = streetName;
		string coordinates, start_lat, start_lon, end_lat, end_lon;
		getline(infile, coordinates);
		looper++;
		string comma = ",";
		string space = " ";
		start_lat = coordinates.substr(0, coordinates.find(comma));
		if(coordinates[coordinates.find(comma) + 1] == ' ')
			start_lon = coordinates.substr(coordinates.find(comma) + 2, coordinates.find(space, coordinates.find(comma) + 2) - coordinates.find(comma) - 1);
		else
			start_lon = coordinates.substr(coordinates.find(comma) + 1, coordinates.find(space) - coordinates.find(comma));
		int firstSpace = coordinates.find(space);
		int secondSpace = coordinates.find(space, firstSpace + 1);
		end_lat = coordinates.substr(secondSpace + 1, coordinates.find(comma, secondSpace + 1) - secondSpace - 1);
		int secondComma = coordinates.find(comma, secondSpace + 1);
		if (coordinates[secondComma + 1] == ' ')
			end_lon = coordinates.substr(secondComma + 2);
		else
			end_lon = coordinates.substr(secondComma + 1);
		GeoCoord start(start_lat, start_lon);
		GeoCoord end(end_lat, end_lon);
		GeoSegment addSegment(start, end);
		toAdd.segment = addSegment;
		int numAttractions;
		infile >> numAttractions;
		getline(infile, streetName);
		looper++;
		for (int i = 0; i < numAttractions; i++) {
			Attraction toPush;
			string lat;
			string lon;
			getline(infile, streetName);
			looper++;
			int z = 0;
			string name;
			string pipe = "|";
			name = streetName.substr(0, streetName.find(pipe));
			lat = streetName.substr(streetName.find(pipe) + 1, streetName.find(comma) - streetName.find(pipe) - 1);
			int commaPos = streetName.find(comma, streetName.find(pipe) + 1);
			if(streetName[commaPos + 1] == ' ')
				lon = streetName.substr(commaPos + 2);
			else
				lon = streetName.substr(commaPos + 1);
			toPush.name = name;
			GeoCoord pos(lat, lon);
			toPush.geocoordinates = pos;
			toAdd.attractions.push_back(toPush);
		}
		m_segments.push_back(toAdd);
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const{
	return m_segments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const{
	if (segNum < 0 || segNum > getNumSegments() - 1)
		return false;
	seg = m_segments[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
