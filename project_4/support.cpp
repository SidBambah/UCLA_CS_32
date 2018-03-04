#include "support.h"
#include "provided.h"


bool operator<(const GeoCoord& left, const GeoCoord& right) {
	if (left.latitude < right.latitude) {
		return true;
	}
	if (left.latitude == right.latitude) {
		if (left.longitude < right.longitude)
			return true;
	}
	return false;
}

bool operator>(const GeoCoord& left, const GeoCoord& right) {
	if (left.latitude > right.latitude)
		return true;
	if (left.latitude == right.latitude) {
		if (left.longitude > right.longitude)
			return true;
	}
	return false;
}

bool operator==(const GeoCoord& left, const GeoCoord& right) {
	if(left.latitude == right.latitude && left.longitude == right.longitude)
		return true;
	return false;
}
