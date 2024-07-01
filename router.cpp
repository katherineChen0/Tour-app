//
//  router.cpp
//  Bruin Tour
//
//  Created by Katherine on 3/7/24.
//

#include "router.h"
#include "geopoint.h"
#include "geotools.h"

#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>


std::vector<GeoPoint>Router::route(const GeoPoint &pt1, const GeoPoint &pt2) const{
    std::queue<GeoPoint> front;
    std::unordered_map<std::string, std::string> from;
    front.push(pt1);
    from[pt1.to_string()] = "";
    
    bool found = false;
    
    while(!found && !front.empty()){
        GeoPoint cur = front.front();
        front.pop();
        
        for(const auto& next : m_db->get_connected_points(cur)){
            if(from.find(next.to_string()) == from.end()){
                front.push(next);
                from[next.to_string()] = cur.to_string();
            }
            if(next.to_string() == pt2.to_string()){
                found = true;
                break;
            }
        }
    }
    std::vector<GeoPoint> route;
    if(from.find(pt2.to_string()) != from.end()){
        makeRoute(pt2.to_string(), 0, from, route);
    }
    return route;
}

void Router::makeRoute(const std::string& cur, int size, const std::unordered_map<std::string, std::string>& map, std::vector<GeoPoint>& route) const{
    if(cur == "") return;
    makeRoute(map.find(cur)->second, size + 1, map, route);
    route.push_back(GeoPoint(cur.substr(0, cur.find(',')), cur.substr(cur.find(',') + 1)));
}
