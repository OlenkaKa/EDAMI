#include "LeaveOneOutValidation.h"

using namespace std;

LeaveOneOutValidation::LeaveOneOutValidation(const Dataset& dataset):
        dataset_(&dataset), rowsNum_(0) {
    for (auto clsEntry: dataset_->getClassDatasets()) {
        rowsNum_ += clsEntry.second.size();
    }
}

void LeaveOneOutValidation::getData(int pairId, Dataset &trainSet, Dataset &testSet,
            NormalizationParams &params) const {
    int i = 0;    
    for (auto &clsEntry: dataset_->getClassDatasets()) {
        string cls = clsEntry.first;
        for (auto &row: clsEntry.second.getRows()) {
            if (i == pairId) {
                testSet.addRow(cls, row);
            } else {
                trainSet.addRow(cls, row);
                setNormalizationParams(row, params);
            }
            ++i;
        }
    }
}

int LeaveOneOutValidation::numberOfPairs() const {
    return rowsNum_;
}

