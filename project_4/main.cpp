// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{	
	cout.precision(11);
	Navigator nv;
	nv.loadMapData("mapdata.txt");
	vector<NavSegment> directions;
	cout << "HERE" << endl;
	nv.navigate("1031 Broxton avenue", "saint sebastian school", directions);
	return 0;
}
