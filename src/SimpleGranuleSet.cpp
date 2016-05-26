#include "SimpleGranuleSet.h"

void SimpleGranuleSet::addGranule(int rowIdx, GranuleMembersPtr members) {
    if(minGranuleSize_ == NO_VALUE || members->size() < minGranuleSize_) {
        minGranuleSize_ = members->size();
    }
    if(maxGranuleSize_ == NO_VALUE || members->size() > maxGranuleSize_) {
        maxGranuleSize_ = members->size();
    }
    granules_[rowIdx] = members;
}

const Granules& SimpleGranuleSet::getGranules() const {
    return granules_;
}

Granules::size_type SimpleGranuleSet::getMinGranuleSize() const {
    return minGranuleSize_;
}

Granules::size_type SimpleGranuleSet::getMaxGranuleSize() const {
    return maxGranuleSize_;
}

double SimpleGranuleSet::getAverageGranuleSize() const {
    Granules::size_type sum = 0;
    for(auto &entry : granules_) {
        sum += entry.second->size();
    }
    return double(sum) / granules_.size();
}

Granules::size_type SimpleGranuleSet::size() const {
    return granules_.size();
}