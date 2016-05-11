#include "GranuleSet.h"

using namespace std;

void GranuleSet::addClass(const string& className, SimpleGranuleSetPtr simpleSet) {
    classGranuleSets_[className] = simpleSet;
}

const ClassGranuleSets& GranuleSet::getClassGranuleSets() const {
    return classGranuleSets_;
}

