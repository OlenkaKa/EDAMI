#ifndef EDAMI_SIMPLECROSSVALIDATION_H
#define EDAMI_SIMPLECROSSVALIDATION_H

#include "CrossValidation.h"
#include "Dataset.h"
#include <map>
#include <vector>

class SimpleCrossValidation: public CrossValidation {
public:
    SimpleCrossValidation(const Dataset& dataset, int subsetsNum);
    void getData(int pairId, Dataset &trainSet, Dataset &testSet,
            NormalizationParams &params) const;
    int numberOfPairs() const;
private:
    
    /**
     * Pointer to original dataset
     */
    const Dataset* dataset_;
    
    /**
     * Map with subset ids for each row in dataset
     */
    std::map<std::string, std::vector<int> > subsets_;
    
    /**
     * Number of all subsets
     */
    int pairsNum_;
};

#endif /* EDAMI_SIMPLECROSSVALIDATION_H */

