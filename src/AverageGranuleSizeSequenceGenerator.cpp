#include "AverageGranuleSizeSequenceGenerator.h"
#include <iostream>
#include <algorithm>

using namespace std;

void AverageGranuleSizeSequenceGenerator::generateSizeSequence(Size avgGranuleSize, Size numberOfGranules,
                                                               list<Size> &sequence) {
    checkArguments(avgGranuleSize, numberOfGranules);
    resetState(avgGranuleSize, numberOfGranules);
    sequence.clear();
    sequence.push_back(currentIdx_);
    while (!reachedStart_ || !reachedEnd_) {
        goLeftOrRight(numberOfGranules, sequence);
    }
}

void AverageGranuleSizeSequenceGenerator::goLeftOrRight(Size numberOfGranules, list<Size> &sequence) {
    if (shouldGoLeft_) {
        if (reachedStart_) {
            goRight(numberOfGranules, sequence);
        } else {
            goLeft(sequence);
        }
    } else {
        if (reachedEnd_) {
            goLeft(sequence);
        } else {
            goRight(numberOfGranules, sequence);
        }
    }
}

void AverageGranuleSizeSequenceGenerator::checkArguments(Size avgGranuleSize, Size numberOfGranules) {
    if (avgGranuleSize > numberOfGranules) {
        string msg = "Average granule size can't be greater than number of granules: "
                     + to_string(avgGranuleSize) + " > " + to_string(numberOfGranules);
        throw invalid_argument(msg);
    }
}

void AverageGranuleSizeSequenceGenerator::test() {
    list<Size> seq;

    generateSizeSequence(3, 10, seq);
    print(seq); // 3,2,4,1,5,6,7,8,9,10

    generateSizeSequence(7, 10, seq);
    print(seq); // 7,6,8,5,9,4,10,3,2,1

    generateSizeSequence(5, 10, seq);
    print(seq); // 5,4,6,3,7,2,8,1,9,10

    generateSizeSequence(5, 9, seq);
    print(seq); // 5,4,6,3,7,2,8,1,9

    generateSizeSequence(1, 1, seq);
    print(seq); // 1

    generateSizeSequence(1, 2, seq);
    print(seq); // 1,2

    generateSizeSequence(2, 2, seq);
    print(seq); // 2,1
}

void AverageGranuleSizeSequenceGenerator::print(list<Size> l) {
    for_each(l.begin(), l.end(), [](Size &s) { cout << s << ","; });
    cout << endl;
}

void AverageGranuleSizeSequenceGenerator::goRight(Size numberOfGranules, list<Size> &sequence) {
    currentIdx_ = ++rightIdx_;
    sequence.push_back(currentIdx_);
    reachedEnd_ = (currentIdx_ >= numberOfGranules);
    shouldGoLeft_ = true;
}

void AverageGranuleSizeSequenceGenerator::goLeft(list<Size> &sequence) {
    currentIdx_ = --leftIdx_;
    sequence.push_back(currentIdx_);
    reachedStart_ = (currentIdx_ <= 1);
    shouldGoLeft_ = false;
}

void AverageGranuleSizeSequenceGenerator::resetState(Size avgGranuleSize, Size numberOfGranules) {
    shouldGoLeft_ = true;
    currentIdx_ = avgGranuleSize;
    leftIdx_ = currentIdx_;
    rightIdx_ = currentIdx_;
    reachedStart_ = (currentIdx_ <= 1);
    reachedEnd_ = (currentIdx_ >= numberOfGranules);
}