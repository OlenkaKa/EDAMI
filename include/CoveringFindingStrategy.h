#ifndef EDAMI_COVERINGFINDINGSTRATEGY_H
#define EDAMI_COVERINGFINDINGSTRATEGY_H

#include "GranuleCalculator.h"

class Dataset;

class CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const Dataset& dataset, const GranuleSet& allGranules) = 0;
};

#endif //EDAMI_COVERINGFINDINGSTRATEGY_H
