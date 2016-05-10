#include <Dataset.h>
#include "GranuleCalculator.h"
#include <cmath>

using namespace std;

GranuleSet *GranuleCalculator::calculateGranules(const Dataset &dataset, const Params &params) {
    this->params_ = params;
    GranuleSet *granuleSet = new GranuleSet();
    for (auto &entry : dataset.getClassDatasets()) {
        (*granuleSet)[entry.first] = vector<vector<int>>();
        const list<RowPtr> &rows = entry.second.getRows();
        int rowIdx = 0;
        for (auto &row : rows) {
            vector<int> similarRows;
            int otherRowIdx = 0;
            for (auto &otherRow : rows) {
                if (rowIdx == otherRowIdx || areSimilar(*row, *otherRow)) {
                    similarRows.push_back(otherRowIdx);
                }
                ++otherRowIdx;
            }
            (*granuleSet)[entry.first].push_back(similarRows);
            ++rowIdx;
        }
    }
    return granuleSet;
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
    if(Attribute::SIMPLE == a1.getType()) {
        return get<string>(a1.getValue()) == get<string>(a2.getValue());
    } else if(Attribute::NUMERICAL == a1.getType()) {
        double difference = get<double>(a1.getValue()) - get<double>(a2.getValue());
        return std::abs(difference) <= params_.getEpsilon();
    } else {
        throw invalid_argument("Unknown attribute type: " + a1.getType());
    }
}