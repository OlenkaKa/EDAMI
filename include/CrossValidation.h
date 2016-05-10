#ifndef CROSS_VALIDATION_H
#define CROSS_VALIDATION_H

#include "Dataset.h"
#include <map>
#include <vector>


struct Sets {
    Dataset trainSet;
    Dataset testSet;
};

class CrossValidation {
public:
    CrossValidation(const Dataset& dataset, int subsetsNum);
    Sets getSets(int subsetId) const;

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
    int subsetsNum_;
};

#endif /* CROSS_VALIDATION_H */

