#ifndef EDAMI_MINIMALSIZESTRATEGY_H
#define EDAMI_MINIMALSIZESTRATEGY_H

#include "GranuleSizeStrategy.h"
#include "SimpleGranuleSet.h"
#include <list>

class MinimalSizeStrategy : public GranuleSizeStrategy {
private:
    virtual void createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, std::list<int>& sequence);
};

#endif //EDAMI_MINIMALSIZESTRATEGY_H
