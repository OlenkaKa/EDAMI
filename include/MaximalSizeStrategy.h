#ifndef EDAMI_MAXIMALSIZESTRATEGY_H
#define EDAMI_MAXIMALSIZESTRATEGY_H

#include <CoveringFindingStrategy.h>
#include <set>

class MaximalSizeStrategy : public CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const Dataset& dataset, const GranuleSet& allGranules);

private:
    SimpleGranuleSetPtr selectGranules(SimpleGranuleSetPtr originalSimpleSetPtr);
    std::set<int> asSetOfIndexes(unsigned long size);
    bool granuleAddsSthNew(std::set<int> &indexesToCover, GranuleMembersPtr members);
};

#endif //EDAMI_MAXIMALSIZESTRATEGY_H
