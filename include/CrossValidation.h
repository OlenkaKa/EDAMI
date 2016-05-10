#ifndef EDAMI_CROSSVALIDATION_H
#define EDAMI_CROSSVALIDATION_H

#include "Dataset.h"

struct DatasetPair {
    Dataset trainSet;
    Dataset testSet;
};

class CrossValidation {
public:
    virtual DatasetPair getPair(int pairId) const = 0;
    virtual int numerOfPairs() const = 0;
};

#endif /* EDAMI_CROSSVALIDATION_H */

