//
//  tour_generator.cpp
//  Bruin Tour
//
//  Created by Katherine on 3/8/24.
//

#include "tour_generator.h"
#include "tourcmd.h"
#include "geotools.h"

#include <vector>

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const{
    std::vector<TourCommand> instructions;
    std::string nextPoi;
    std::string talk;
    stops.get_poi_data(0, nextPoi, talk);
    for(int i = 0; i < stops.size(); i++){
        TourCommand comment;
        comment.init_commentary(nextPoi, talk);
        instructions.push_back(comment);
        
        if(i<stops.size()-1){
            GeoPoint start;
            m_db->GeoDatabaseBase::get_poi_location(nextPoi, start);
            stops.get_poi_data(i+1, nextPoi, talk);
            GeoPoint end;
            m_db->get_poi_location(nextPoi, end);
            std::vector<GeoPoint> route = m_router->route(start,end);
            for(int i = 0; i < route.size()-1; i++){
                GeoPoint& pt1 = route[i];
                GeoPoint& pt2 = route[i + 1];
                
                TourCommand proceed;
                proceed.init_proceed(dir(pt1, pt2), m_db->get_street_name(pt1, pt2), distance_earth_miles(pt1, pt2), pt1, pt2);
                instructions.push_back(proceed);
                
                if(i < route.size() - 2){
                    GeoPoint& pt3 = route[i+2];
                    if(m_db->get_street_name(pt1, pt2) != m_db->get_street_name(pt2, pt3)){
                        double angle = angle_of_turn(pt1, pt2, pt3);
                        if(angle>=1 && angle < 180){
                            TourCommand turn;
                            turn.init_turn("left", m_db->get_street_name(pt2, pt3));
                            instructions.push_back(turn);
                        }else if(angle>=180 && angle <=359){
                            TourCommand turn;
                            turn.init_turn("right", m_db->get_street_name(pt2, pt3));
                            instructions.push_back(turn);
                        }
                    }
                }
            }
        }
    }
    return instructions;
}

std::string TourGenerator::dir(const GeoPoint& pt1, const GeoPoint& pt2) const{
    double angle = angle_of_line(pt1, pt2);
    if(0<=angle && angle < 22.5){
        return "east";
    }else if (angle < 67.5){
        return "northeast";
    }else if(angle < 112.5){
        return "north";
    }else if(angle<157.5){
        return "northwest";
    }else if(angle<202.5){
        return "west";
    }else if(angle<247.5){
        return "southwest";
    }else if(angle < 292.5){
        return "south";
    }else{
        return "east";
    }
}
