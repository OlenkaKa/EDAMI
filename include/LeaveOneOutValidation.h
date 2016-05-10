#ifndef EDAMI_LEAVEONEOUTVALIDATION_H
#define EDAMI_LEAVEONEOUTVALIDATION_H

#include "CrossValidation.h"

class LeaveOneOutValidation: public CrossValidation {
public:
    LeaveOneOutValidation(const Dataset& dataset);
    DatasetPair getPair(int pairId) const;
    int numerOfPairs() const;
private:
    const Dataset* dataset_;
    long rowsNum_;
};

#endif /* EDAMI_LEAVEONEOUTVALIDATION_H */

