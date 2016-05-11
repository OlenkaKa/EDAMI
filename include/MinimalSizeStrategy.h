#ifndef EDAMI_MINIMALSIZESTRATEGY_H
#define EDAMI_MINIMALSIZESTRATEGY_H

#include <CoveringFindingStrategy.h>
#include <bits/stl_set.h>
#include "SimpleGranuleSet.h"

class MinimalSizeStrategy : public CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const Dataset& dataset, const GranuleSet& allGranules);

private:
    std::set<int> asSetOfIndexes(unsigned long size) const;
    bool granuleAddsSthNew(std::set<int> &indexesToCover, GranuleMembersPtr members) const;
};

#endif //EDAMI_MINIMALSIZESTRATEGY_H
