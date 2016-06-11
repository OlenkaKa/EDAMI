#ifndef EDAMI_NUMBERSTATS_H
#define EDAMI_NUMBERSTATS_H

#include <list>

class NumberStats {
public:
    NumberStats();
    void include(int value);
    int getMinValue() const;
    int getMaxValue() const;
    double getAverageValue() const;
    int getSum() const;
    double getStandardDeviation() const;

private:
    int minValue_;
    int maxValue_;
    int sum_;
    std::list<int> values_;
};

#endif //EDAMI_NUMBERSTATS_H
