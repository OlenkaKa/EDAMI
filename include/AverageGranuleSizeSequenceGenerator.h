#ifndef EDAMI_AVERAGEGRANULESIZESEQUENCEGENERATOR_H
#define EDAMI_AVERAGEGRANULESIZESEQUENCEGENERATOR_H

#include <list>
#include "GranuleSet.h"
#include "SimpleGranuleSet.h"

class AverageGranuleSizeSequenceGenerator {
public:
    typedef int Size;
    void generateSizeSequence(Size avgGranuleSize, Size numberOfGranules, std::list<Size> &sequence);
    void test();
    void print(std::list<Size> l);

private:
    bool reachedStart_;
    bool reachedEnd_;

    bool shouldGoLeft_;

    Size currentIdx_;
    Size leftIdx_;
    Size rightIdx_;

    void checkArguments(Size avgGranuleSize, Size numberOfGranules);
    void resetState(Size avgGranuleSize, Size numberOfGranules);
    void goLeftOrRight(Size numberOfGranules, std::list<Size> &sequence);
    void goLeft(std::list<Size> &sequence);
    void goRight(Size numberOfGranules, std::list<Size> &sequence);
};

#endif //EDAMI_AVERAGEGRANULESIZESEQUENCEGENERATOR_H
