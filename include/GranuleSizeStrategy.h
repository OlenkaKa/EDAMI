#ifndef EDAMI_GRANULESIZESTRATEGY_H
#define EDAMI_GRANULESIZESTRATEGY_H

#include <CoveringFindingStrategy.h>
#include <set>

class GranuleSizeStrategy : public CoveringFindingStrategy {
public:
    virtual GranuleSet* selectGranules(const Dataset& dataset, const GranuleSet& allGranules);
    virtual ~GranuleSizeStrategy() {}

private:
    std::set<int> indexesToCover_;
    std::list<int> granuleSizeSequence_;

    SimpleGranuleSetPtr selectGranules(SimpleGranuleSetPtr originalSimpleSetPtr);
    virtual void createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, std::list<int>& sequence) = 0;
    std::set<int> asSetOfIndexes(unsigned long size);
    void addGranulesOfNextSize(const SimpleGranuleSetPtr &originalSimpleSetPtr, SimpleGranuleSetPtr &newSimpleSetPtr);
    bool removeIndexesCoveredByGranule(const GranuleMembersPtr &members);
};

#endif //EDAMI_GRANULESIZESTRATEGY_H
