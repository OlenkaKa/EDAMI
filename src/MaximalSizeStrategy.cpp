#include "MaximalSizeStrategy.h"

using namespace std;

GranuleSet *MaximalSizeStrategy::selectGranules(const Dataset &dataset, const GranuleSet &allGranules) {
    GranuleSet *newGranuleSet = new GranuleSet();
    for (auto &entry : allGranules.getClassGranuleSets()) {
        SimpleGranuleSetPtr newSimpleSetPtr = selectGranules(entry.second);
        newGranuleSet->addClass(entry.first, newSimpleSetPtr);
    }
    return newGranuleSet;
}

SimpleGranuleSetPtr MaximalSizeStrategy::selectGranules(SimpleGranuleSetPtr originalSimpleSetPtr) {
    set<int> indexesToCover = asSetOfIndexes(originalSimpleSetPtr->size());
    SimpleGranuleSetPtr newSimpleSetPtr(new SimpleGranuleSet());
    Granules::size_type granuleSize = originalSimpleSetPtr->getMaxGranuleSize();
    bool coveringFound = false;
    while (!coveringFound) {
        for(auto &granuleEntry : originalSimpleSetPtr->getGranules()) {
            GranuleMembersPtr members = granuleEntry.second;
            if(members->size() == granuleSize && granuleAddsSthNew(indexesToCover, members)) {
                newSimpleSetPtr->addGranule(granuleEntry.first, members);
                if(indexesToCover.empty()) {
                    coveringFound = true;
                    break;
                }
            }
        }
        --granuleSize;
    }
    return newSimpleSetPtr;
}

bool MaximalSizeStrategy::granuleAddsSthNew(set<int> &indexesToCover, GranuleMembersPtr members) {
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

set<int> MaximalSizeStrategy::asSetOfIndexes(unsigned long size) {
    set<int> indexes;
    for (int i = 0; i < size; ++i) {
        indexes.insert(i);
    }
    return indexes;
}