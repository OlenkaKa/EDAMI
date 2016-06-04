#ifndef EDAMI_MINIMALSIZESTRATEGY_H
#define EDAMI_MINIMALSIZESTRATEGY_H

#include "GranuleSizeStrategy.h"
#include "SimpleGranuleSet.h"
#include <list>

class MinimalSizeStrategy : public GranuleSizeStrategy {
private:
    virtual void createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, std::list<int>& sequence);
    virtual std::string getName() { return "Minimal size strategy"; }
};

#endif //EDAMI_MINIMALSIZESTRATEGY_H
