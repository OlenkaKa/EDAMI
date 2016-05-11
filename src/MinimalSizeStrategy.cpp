#include "MinimalSizeStrategy.h"

using namespace std;

GranuleSet *MinimalSizeStrategy::selectGranules(const Dataset &dataset, const GranuleSet &allGranules) {
    GranuleSet *newGranuleSet = new GranuleSet();
    for (auto &entry : allGranules.getClassGranuleSets()) {

        SimpleGranuleSetPtr originalSimpleSetPtr = entry.second;
        set<int> indexesToCover = asSetOfIndexes(originalSimpleSetPtr->size());

        SimpleGranuleSetPtr newSimpleSetPtr(new SimpleGranuleSet());

        Granules::size_type granuleSize = originalSimpleSetPtr->getMinGranuleSize();
        bool coveringFound = false;
        while (!coveringFound) {
            const Granules &granules = originalSimpleSetPtr->getGranules();
            for(auto &granuleEntry : granules) {
                GranuleMembersPtr members = granuleEntry.second;
                if(members->size() == granuleSize && granuleAddsSthNew(indexesToCover, members)) {
                    newSimpleSetPtr->addGranule(granuleEntry.first, members);
                    if(indexesToCover.empty()) {
                        coveringFound = true;
                        break;
                    }
                }
            }
            ++granuleSize;
        }
        newGranuleSet->addClass(entry.first, newSimpleSetPtr);
    }
    return newGranuleSet;
}

bool MinimalSizeStrategy::granuleAddsSthNew(set<int> &indexesToCover, GranuleMembersPtr members) const {
    bool anythingNew = false;
    for (const int &idx : (*members)) {
        auto iter = indexesToCover.find(idx);
        if (iter != indexesToCover.end()) {
            anythingNew = true;
            indexesToCover.erase(iter);
            if(indexesToCover.empty()) {    // selected granules already represent all granules 
                break;
            }
        }
    }
    return anythingNew;
}

set<int> MinimalSizeStrategy::asSetOfIndexes(unsigned long size) const {
    set<int> indexes;
    for (int i = 0; i < size; ++i) {
        indexes.insert(i);
    }
    return indexes;
}
