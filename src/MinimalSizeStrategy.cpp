#include "MinimalSizeStrategy.h"
#include <vector>
#include <bits/stl_set.h>

using namespace std;

GranuleSet *MinimalSizeStrategy::selectGranules(const Dataset &dataset, const GranuleSet &allGranules) {
    GranuleSet *result = new GranuleSet();
    for (auto &entry : allGranules) {
        unsigned long allRows = entry.second.size();
        vector<vector<int>> selectedGranules(allRows);
        set<int> indexesToCover = asSetOfIndexes(allRows);
        int granuleSize = 0;
        bool coveringFound = false;
        while (!coveringFound) {
            ++granuleSize;
            for (size_t i = 0; i < allRows; ++i) {
                const vector<int> &granule = entry.second.at(i);
                if (granule.size() == granuleSize && granuleAddsSthNew(indexesToCover, granule)) {
                    selectedGranules[i] = granule;
                    if(indexesToCover.empty()) {
                        coveringFound = true;
                        break;
                    }
                }
            }
        }
        (*result)[entry.first] = selectedGranules;
    }
    return result;
}

bool MinimalSizeStrategy::granuleAddsSthNew(set<int> &indexesToCover, const vector<int> &granule) const {
    bool anythingNew = false;
    for (const int &idx : granule) {
        auto iter = indexesToCover.find(idx);
        if (iter != indexesToCover.end()) {
            anythingNew = true;
            indexesToCover.erase(iter);
            if(indexesToCover.empty()) {
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
