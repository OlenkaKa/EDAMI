#ifndef EDAMI_MAXIMALSIZESTRATEGY_H
#define EDAMI_MAXIMALSIZESTRATEGY_H

#include "GranuleSizeStrategy.h"
#include "SimpleGranuleSet.h"
#include <list>

class MaximalSizeStrategy : public GranuleSizeStrategy {
private:
    virtual void createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, std::list<int>& sequence);
    virtual std::string getName() { return "Maximal size strategy"; }
};

#endif //EDAMI_MAXIMALSIZESTRATEGY_H
