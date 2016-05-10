#include "SimpleCrossValidation.h"
#include <algorithm>
#include <random>

using namespace std;

SimpleCrossValidation::SimpleCrossValidation(const Dataset& dataset, int pairsNum):
        dataset_(&dataset), pairsNum_(pairsNum) {
    
    // subset id for remaining rows (remainders after division)
    int remainVal = 0;
    
    for (auto &clsEntry: dataset_->getClassDatasets()) {
        size_t rowsNum = clsEntry.second.size();
        size_t subsetSize = rowsNum / pairsNum_;
        
        vector<int> subsetIds(rowsNum);
        
        auto startIt = subsetIds.begin();
        auto endIt = startIt + subsetSize;
        for (int i = 0; i < pairsNum_; ++i) {
            fill(startIt, endIt, i);
            startIt = endIt;
            endIt = endIt + subsetSize;
        }
        
        for (auto it = startIt; it != subsetIds.end(); ++it) {
            *it = remainVal;
            remainVal = (remainVal + 1) % pairsNum_;
        }
        
        auto engine = default_random_engine {};
        shuffle(begin(subsetIds), end(subsetIds), engine);
        
        subsets_[clsEntry.first] = subsetIds;
    }
}

DatasetPair SimpleCrossValidation::getPair(int pairId) const {
    DatasetPair result;
    for (auto &clsEntry: dataset_->getClassDatasets()) {
        string cls = clsEntry.first;
        const list<RowPtr> clsRows = clsEntry.second.getRows();
        vector<int> ids = subsets_.at(cls);
        
        SimpleDataset clsTrainSet, clsTestSet;
        auto rowsIt = clsRows.begin();
        auto idsIt = ids.begin();
        for (; rowsIt != clsRows.end() && idsIt != ids.end(); ++rowsIt, ++idsIt) {
            if (*idsIt == pairId) {
                clsTestSet.addRow(*rowsIt);
            } else {
                clsTrainSet.addRow(*rowsIt);
            }
        }
        result.trainSet.addClass(cls, clsTrainSet);
        result.testSet.addClass(cls, clsTestSet);
    }
    return result;
}

int SimpleCrossValidation::numerOfPairs() const {
    return pairsNum_;
}

