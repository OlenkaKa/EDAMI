#include "MaximalSizeStrategy.h"

using namespace std;

void MaximalSizeStrategy::createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, list<int>& sequence) {
    unsigned long minSize = originalSimpleSetPtr->getMinGranuleSize();
    unsigned long maxSize = originalSimpleSetPtr->getMaxGranuleSize();
    for (unsigned long i = maxSize; i >= minSize; --i) {
        sequence.push_back(i);
    }
}
