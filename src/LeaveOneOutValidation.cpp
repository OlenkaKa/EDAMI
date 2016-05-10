#include "LeaveOneOutValidation.h"

using namespace std;

LeaveOneOutValidation::LeaveOneOutValidation(const Dataset& dataset):
        dataset_(&dataset), rowsNum_(0) {
    for (auto clsEntry: dataset_->getClassDatasets()) {
        rowsNum_ += clsEntry.second.size();
    }
}

DatasetPair LeaveOneOutValidation::getPair(int pairId) const {
    DatasetPair result;
    int i = 0;    
    for (auto &clsEntry: dataset_->getClassDatasets()) {
        string cls = clsEntry.first;
        for (auto &row: clsEntry.second.getRows()) {
            if (i == pairId) {
                result.testSet.addRow(cls, row);
            } else {
                result.trainSet.addRow(cls, row);
            }
            ++i;
        }
    }
    return result;
}

int LeaveOneOutValidation::numerOfPairs() const {
    return rowsNum_;
}

