//
//  geodb.cpp
//  Bruin Tour
//
//  Created by Katherine on 3/7/24.
//

#include "geodb.h"
#include "geopoint.h"
#include "geotools.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


bool GeoDatabase::load(const std::string& map_data_file){
    std::ifstream infile(map_data_file);
    if(!infile)
        return false;
    while(true){
        std::string street;
        std::string line;
        std::string lat1, lon1, lat2, lon2;
        
        if(!getline(infile, street)){break;}
        if(!getline(infile, street)){break;}
        std::istringstream iss(line);
        iss >> lat1 >> lon1 >> lat2 >> lon2;
        
        GeoPoint p1(lat1, lon1);
        GeoPoint p2(lat2, lon2);
        
        separate(street, p1, p2);
        
        int pois;
        if(!(infile >> pois)){break;}
        infile.ignore(1000, '\n');
        if(pois > 0){
            GeoPoint mid = midpoint(p1, p2);
            separate(street, p1, mid);
            separate(street, p2, mid);
            
            for(int i = 0; i < pois; i++){
                std::string name;
                std::string lat, lon;
                
                getline(infile, line);
                name = line.substr(0, line.find('|'));
                line.erase(0, name.size() + 1);
                lat = line.substr(0, line.find(' '));
                line.erase(0, lat.size() +1);
                lon = line;
                
                GeoPoint poi(lat, lon);
                poiToPoint.insert(name, poi);
                separate("a path", poi, mid);
            }
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string &poi, GeoPoint &point) const{
    const GeoPoint* location = poiToPoint.find(poi);
    if(location == nullptr)
        return false;
    point = *location;
    return true;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint &pt) const{
    const std::vector<GeoPoint>* points = pointToPoints.find(pt.to_string());
    if(points == nullptr)
        return std::vector<GeoPoint>();
    return *points;
}

std::string GeoDatabase::get_street_name(const GeoPoint &pt1, const GeoPoint &pt2) const{
    const std::string* name = pointsToName.find(pt1.to_string() + "|" + pt2.to_string());
    if(name == nullptr)
        return "";
    return *name;
}

void GeoDatabase::separate(std::string street, const GeoPoint &pt1, const GeoPoint &pt2){
    
    pointToPoints[pt1.to_string()].push_back(pt2);
    pointToPoints[pt2.to_string()].push_back(pt1);
    
    pointsToName.insert(pt1.to_string() + "|" + pt2.to_string(), street);
    pointsToName.insert(pt2.to_string() + "|" + pt1.to_string(), street);
}
