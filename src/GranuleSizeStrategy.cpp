#include "GranuleSizeStrategy.h"

using namespace std;

GranuleSetPtr GranuleSizeStrategy::selectGranules(const GranuleSet &allGranules) {
    GranuleSetPtr newGranuleSet(new GranuleSet());
    for (auto &entry : allGranules.getClassGranuleSets()) {
        SimpleGranuleSetPtr newSimpleSetPtr = selectGranules(entry.second);
        newGranuleSet->addClass(entry.first, newSimpleSetPtr);
    }
    return newGranuleSet;
}

SimpleGranuleSetPtr GranuleSizeStrategy::selectGranules(SimpleGranuleSetPtr originalSimpleSetPtr) {
    indexesToCover_ = asSetOfIndexes(originalSimpleSetPtr->size());
    granuleSizeSequence_.clear();
    createGranuleSizeSequence(originalSimpleSetPtr, granuleSizeSequence_);
    SimpleGranuleSetPtr newSimpleSetPtr(new SimpleGranuleSet());
    while (!indexesToCover_.empty() && !granuleSizeSequence_.empty()) {
        addGranulesOfNextSize(originalSimpleSetPtr, newSimpleSetPtr);
        granuleSizeSequence_.pop_front();
    }
    return newSimpleSetPtr;
}

void GranuleSizeStrategy::addGranulesOfNextSize(const SimpleGranuleSetPtr &originalSimpleSetPtr,
                                                SimpleGranuleSetPtr &newSimpleSetPtr) {
    const Granules &granules = originalSimpleSetPtr->getGranules();
    auto iter = granules.begin();
    while (!indexesToCover_.empty() && iter != granules.end()) {
        GranuleMembersPtr members = (*iter).second;
        if (members->size() == granuleSizeSequence_.front()) {
            if (removeIndexesCoveredByGranule(members)) {
                newSimpleSetPtr->addGranule((*iter).first, members);
            }
        }
        ++iter;
    }
}

bool GranuleSizeStrategy::removeIndexesCoveredByGranule(const GranuleMembersPtr &members) {
    bool granuleAddsSthNew = false;
    auto memberIt = members->begin();
    while (!indexesToCover_.empty() && memberIt != members->end()) {
        auto idxIt = indexesToCover_.find(*memberIt);
        if (idxIt != indexesToCover_.end()) {
            granuleAddsSthNew = true;
            indexesToCover_.erase(idxIt);
        }
        ++memberIt;
    }
    return granuleAddsSthNew;
}

set<int> GranuleSizeStrategy::asSetOfIndexes(unsigned long size) {
    set<int> indexes;
    for (int i = 0; i < size; ++i) {
        indexes.insert(i);
    }
    return indexes;
}