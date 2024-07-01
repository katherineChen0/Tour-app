//
//  tour_generator.h
//  Bruin Tour
//
//  Created by Katherine on 3/8/24.
//

#ifndef tour_generator_h
#define tour_generator_h

#include "base_classes.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"

#include <vector>

class TourGenerator: public TourGeneratorBase
{
public:
 TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
 virtual ~TourGenerator();
 virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const RouterBase* m_router;
    const GeoDatabase* m_db;
    std::string dir(const GeoPoint& pt1, const GeoPoint& pt2) const;
};

#endif /* tour_generator_h */
