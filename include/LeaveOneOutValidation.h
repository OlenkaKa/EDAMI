#ifndef EDAMI_LEAVEONEOUTVALIDATION_H
#define EDAMI_LEAVEONEOUTVALIDATION_H

#include "CrossValidationData.h"

class LeaveOneOutValidation: public CrossValidationData {
public:
    LeaveOneOutValidation(const Dataset& dataset);
    void getData(int pairId, Dataset &trainSet, Dataset &testSet,
            NormalizationParams &params) const;
    int numberOfPairs() const;
private:
    const Dataset* dataset_;
    int rowsNum_;
};

#endif /* EDAMI_LEAVEONEOUTVALIDATION_H */

