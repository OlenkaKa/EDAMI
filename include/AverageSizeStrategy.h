#ifndef EDAMI_AVERAGESIZESTRATEGY_H
#define EDAMI_AVERAGESIZESTRATEGY_H

#include "GranuleSizeStrategy.h"
#include "AverageGranuleSizeSequenceGenerator.h"
#include <list>
#include "SimpleGranuleSet.h"

class AverageSizeStrategy : public GranuleSizeStrategy {
private:
    AverageGranuleSizeSequenceGenerator sizeSequenceGenerator_;

    virtual void createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, std::list<int>& sequence);
    virtual std::string getName() { return "Average size strategy"; }
};

#endif //EDAMI_AVERAGESIZESTRATEGY_H
