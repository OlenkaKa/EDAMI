#include "MinimalSizeStrategy.h"

using namespace std;

void MinimalSizeStrategy::createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, list<int>& sequence) {
    unsigned long minSize = originalSimpleSetPtr->getMinGranuleSize();
    unsigned long maxSize = originalSimpleSetPtr->getMaxGranuleSize();
    for (unsigned long i = minSize; i <= maxSize; ++i) {
        sequence.push_back(i);
    }
}
