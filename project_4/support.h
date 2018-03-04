#ifndef SUPPORT_H
#define SUPPORT_H


struct GeoCoord;

bool operator<(const GeoCoord& left, const GeoCoord& right);
bool operator>(const GeoCoord& left, const GeoCoord& right);
bool operator==(const GeoCoord& left, const GeoCoord& right);

#endif