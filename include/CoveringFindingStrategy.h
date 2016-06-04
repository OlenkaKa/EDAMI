#ifndef EDAMI_COVERINGFINDINGSTRATEGY_H
#define EDAMI_COVERINGFINDINGSTRATEGY_H

#include "GranuleSet.h"
#include <string>

class CoveringFindingStrategy {
public:
    virtual GranuleSetPtr selectGranules(const GranuleSet& allGranules) = 0;
    virtual std::string getName() = 0;
    virtual ~CoveringFindingStrategy() {}
};

#endif //EDAMI_COVERINGFINDINGSTRATEGY_H
