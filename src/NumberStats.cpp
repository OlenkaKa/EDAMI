#include <limits>
#include "NumberStats.h"
#include <numeric>
#include <cmath>

using namespace std;

NumberStats::NumberStats() :
        minValue_(std::numeric_limits<int>::max()),
        maxValue_(std::numeric_limits<int>::min()),
        sum_(0) { }

void NumberStats::include(int value) {
    values_.push_back(value);
    if(value > maxValue_) {
        maxValue_ = value;
    }
    if(value < minValue_) {
        minValue_ = value;
    }
    sum_ += value;
}

int NumberStats::getMinValue() const {
    return minValue_;
}

int NumberStats::getMaxValue() const {
    return maxValue_;
}

double NumberStats::getAverageValue() const {
    if(values_.empty()) {
        return 0;
    } else {
        return double(sum_) / values_.size();
    }
}

int NumberStats::getSum() const {
    return sum_;
}

double NumberStats::getStandardDeviation() const {
    if(values_.size() < 2) {
        return 0;
    } else {
        const double average = getAverageValue();
        double sumOfSquares = accumulate(begin(values_), end(values_), double(0),
                                         [&average](const double sum, int value){ return sum + std::pow((double(value) - average), 2); });
        return sqrt(sumOfSquares / double(values_.size() - 1));
    }
}