#include "GranuleSet.h"
#include <ostream>
#include <string>

using namespace std;

void GranuleSet::addClass(const string& className, SimpleGranuleSetPtr simpleSet) {
    classGranuleSets_[className] = simpleSet;
}

const ClassGranuleSets& GranuleSet::getClassGranuleSets() const {
    return classGranuleSets_;
}

ostream& operator<<(ostream& os, const GranuleSet& granuleSet) {
    os << "[ GranuleSet ]" << endl;
    for(auto &classEntry : granuleSet.getClassGranuleSets()) {
        os << "--- class: " << classEntry.first << endl;
        for(auto &granuleEntry : classEntry.second->getGranules()) {
            int rowIdx = granuleEntry.first;
            GranuleMembersPtr members = granuleEntry.second;
            string memberIndexes = "";
            for(auto &idx : (*members)) {
                memberIndexes += to_string(idx) + ",";
            }
            memberIndexes.pop_back();
            os << "Row " << rowIdx << ", granule (" << memberIndexes << ")" << endl;
        }
    }
    return os;
}

