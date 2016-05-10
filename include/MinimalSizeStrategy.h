#ifndef EDAMI_MINIMALSIZESTRATEGY_H
#define EDAMI_MINIMALSIZESTRATEGY_H

#include <CoveringFindingStrategy.h>
#include <bits/stl_set.h>

class MinimalSizeStrategy : public CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const Dataset& dataset, const GranuleSet& allGranules);

private:
    std::set<int> asSetOfIndexes(unsigned long size) const;
    bool granuleAddsSthNew(std::set<int> &indexesToCover, const std::vector<int> &granule) const;
};

#endif //EDAMI_MINIMALSIZESTRATEGY_H
