#include "AverageSizeStrategy.h"
#include <cmath>

using namespace std;

void AverageSizeStrategy::createGranuleSizeSequence(SimpleGranuleSetPtr originalSimpleSetPtr, list<int>& sequence) {
    long avgSize = lround(originalSimpleSetPtr->getAverageGranuleSize());
    sizeSequenceGenerator_.generateSizeSequence(avgSize, originalSimpleSetPtr->size(), sequence);
}
