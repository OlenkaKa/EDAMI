#ifndef EDAMI_COVERINGFINDINGSTRATEGY_H
#define EDAMI_COVERINGFINDINGSTRATEGY_H

#include "GranuleCalculator.h"

class CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const GranuleSet& allGranules) = 0;
    virtual ~CoveringFindingStrategy() {}
};

#endif //EDAMI_COVERINGFINDINGSTRATEGY_H
