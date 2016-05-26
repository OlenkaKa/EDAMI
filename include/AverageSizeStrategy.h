#ifndef EDAMI_AVERAGESIZESTRATEGY_H
#define EDAMI_AVERAGESIZESTRATEGY_H

#include <CoveringFindingStrategy.h>
#include <list>
#include "SimpleGranuleSet.h"
#include <set>
#include "AverageGranuleSizeSequenceGenerator.h"

class AverageSizeStrategy : public CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const Dataset& dataset, const GranuleSet& allGranules);

private:
    typedef Granules::size_type Size;
    AverageGranuleSizeSequenceGenerator sizeSequenceGenerator_;

    SimpleGranuleSetPtr selectGranules(SimpleGranuleSetPtr originalSimpleSetPtr);
    std::set<int> asSetOfIndexes(unsigned long size);
    bool granuleAddsSthNew(std::set<int> &indexesToCover, GranuleMembersPtr members);
};

#endif //EDAMI_AVERAGESIZESTRATEGY_H
