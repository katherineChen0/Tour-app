#include <cassert>
#include <iostream>
#include "geodb.h"

using namespace std;

int main() {
//    //poi location check
//    GeoDatabase g;
//    g.load("mapdata.txt"); // assume this works to avoid error checking
//
//    GeoPoint p;
//    if (g.get_poi_location("Diddy Riese", p))
//    cout << "The PoI is at " << p.sLatitude << ", "
//    << p.sLongitude << endl;
//    else
//    cout << "PoI not found!\n";

    //connected check
    GeoDatabase g;
     g.load("mapdata.txt"); // assume this works to avoid error checking
     std::vector<GeoPoint> pts = g.get_connected_points(
     GeoPoint("34.0731003", "-118.4931016"));
     if (pts.empty())
     cout << "There are no points connected to your specified point\n";
     else {
         for (const auto p: pts)
             cout << p.sLatitude << ", " << p.sLongitude << endl;
     }

         
  
}
