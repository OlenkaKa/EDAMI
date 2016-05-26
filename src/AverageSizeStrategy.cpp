#include "AverageSizeStrategy.h"
#include <cmath>

using namespace std;

GranuleSet *AverageSizeStrategy::selectGranules(const Dataset &dataset, const GranuleSet &allGranules) {
    GranuleSet *newGranuleSet = new GranuleSet();
    for (auto &entry : allGranules.getClassGranuleSets()) {
        SimpleGranuleSetPtr newSimpleSetPtr = selectGranules(entry.second);
        newGranuleSet->addClass(entry.first, newSimpleSetPtr);
    }
    return newGranuleSet;
}

SimpleGranuleSetPtr AverageSizeStrategy::selectGranules(SimpleGranuleSetPtr originalSimpleSetPtr) {
    set<int> indexesToCover = asSetOfIndexes(originalSimpleSetPtr->size());
    SimpleGranuleSetPtr newSimpleSetPtr(new SimpleGranuleSet());

    list<Size> sizeSequence;
    Size avgRounded = (Size) lround(originalSimpleSetPtr->getAverageGranuleSize());
    sizeSequenceGenerator_.generateSizeSequence(avgRounded, originalSimpleSetPtr->size(), sizeSequence);

    auto iter = sizeSequence.begin();
    bool coveringFound = false;
    Size granuleSize;
    while (!coveringFound && (iter != sizeSequence.end())) {
        granuleSize = *iter;
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
        ++iter;
    }
    return newSimpleSetPtr;
}

bool AverageSizeStrategy::granuleAddsSthNew(set<int> &indexesToCover, GranuleMembersPtr members) {
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

set<int> AverageSizeStrategy::asSetOfIndexes(unsigned long size) {
    set<int> indexes;
    for (int i = 0; i < size; ++i) {
        indexes.insert(i);
    }
    return indexes;
}

