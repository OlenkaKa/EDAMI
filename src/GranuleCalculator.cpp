#include <Dataset.h>
#include "GranuleCalculator.h"
#include <cmath>

using namespace std;

GranuleSetPtr GranuleCalculator::calculateGranules(const Dataset &dataset, const Params &params) {
    this->params_ = params;
    GranuleSetPtr granuleSet(new GranuleSet());
    for (auto &entry : dataset.getClassDatasets()) {
        granuleSet->addClass(entry.first, calculateSimpleGranulesSet(entry.second));
    }
    return granuleSet;
}

SimpleGranuleSetPtr GranuleCalculator::calculateSimpleGranulesSet(const SimpleDataset& simpleDataSet) {
    SimpleGranuleSetPtr simpleSetPtr(new SimpleGranuleSet());
    int rowIdx = 0;
    for (auto &row : simpleDataSet.getRows()) {
        GranuleMembersPtr granuleMembersPtr(new GranuleMembers());
        int otherRowIdx = 0;
        for (auto &otherRow : simpleDataSet.getRows()) {
            if (rowIdx == otherRowIdx || areSimilar(*row, *otherRow)) {
                granuleMembersPtr->push_back(otherRowIdx);
            }
            ++otherRowIdx;
        }
        simpleSetPtr->addGranule(rowIdx, granuleMembersPtr);
        ++rowIdx;
    }
    return simpleSetPtr;
}

bool GranuleCalculator::areSimilar(const Row &r1, const Row &r2) {
    int sameAttributeValues = 0;
    for(size_t i=0; i<r1.size(); ++i) {
        if(areEqual(r1.at(i), r2.at(i))) {
            ++sameAttributeValues;
        }
    }
    return (double(sameAttributeValues) / r1.size()) >= params_.getRadius();
}

bool GranuleCalculator::areEqual(const Attribute& a1, const Attribute& a2) {
    if(a1.getType() != a2.getType()) {
        throw invalid_argument("Cannot compare attributes of different types");
    }
    if(Attribute::NOMINAL == a1.getType()) {
        return get<string>(a1.getValue()) == get<string>(a2.getValue());
    } else if(Attribute::NUMERICAL == a1.getType()) {
        double difference = get<double>(a1.getValue()) - get<double>(a2.getValue());
        return lessThanOrEqual(std::abs(difference), params_.getEpsilon());
    } else {
        throw invalid_argument("Unknown attribute type: " + a1.getType());
    }
}

bool GranuleCalculator::lessThanOrEqual(double lhs, double rhs) {
    return (std::abs(rhs - lhs) < 0.00000000000001) || lhs < rhs;
}