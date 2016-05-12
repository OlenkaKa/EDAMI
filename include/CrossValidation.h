#ifndef EDAMI_CROSSVALIDATION_H
#define EDAMI_CROSSVALIDATION_H

#include "Dataset.h"
#include "NormalizationParams.h"

class CrossValidation {
public:
    virtual void getData(int pairId, Dataset &trainSet, Dataset &testSet,
            NormalizationParams &params) const = 0;
    virtual int numberOfPairs() const = 0;
};

#endif /* EDAMI_CROSSVALIDATION_H */

