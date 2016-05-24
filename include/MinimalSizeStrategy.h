#ifndef EDAMI_MINIMALSIZESTRATEGY_H
#define EDAMI_MINIMALSIZESTRATEGY_H

#include "CoveringFindingStrategy.h"
#include "SimpleGranuleSet.h"
#include <set>

class MinimalSizeStrategy : public CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const Dataset& dataset, const GranuleSet& allGranules);

private:
    SimpleGranuleSetPtr selectGranules(SimpleGranuleSetPtr originalSimpleSetPtr);
    std::set<int> asSetOfIndexes(unsigned long size) const;
    bool granuleAddsSthNew(std::set<int> &indexesToCover, GranuleMembersPtr members) const;
};

#endif //EDAMI_MINIMALSIZESTRATEGY_H
