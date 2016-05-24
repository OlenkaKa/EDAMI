#ifndef EDAMI_GRANULECALCULATOR_H
#define EDAMI_GRANULECALCULATOR_H

#include <map>
#include <vector>
#include <SimpleDataset.h>
#include "GranuleSet.h"

class Dataset;

class GranuleCalculator {
public:
    class Params {
    public:
        Params() : epsilon_(0), radius_(0) { }

        Params(const double &epsilon, const double &radius) {
            epsilon_ = epsilon;
            radius_ = radius;
        }

        double getEpsilon() const {
            return epsilon_;
        }

        double getRadius() const {
            return radius_;
        }

    private:
        double epsilon_;
        double radius_;
    };

    GranuleSet *calculateGranules(const Dataset &dataset, const Params &params);

private:
    Params params_;

    SimpleGranuleSetPtr calculateSimpleGranulesSet(const SimpleDataset& simpleDataSet);
    bool areSimilar(const Row &r1, const Row &r2);
    bool areEqual(const Attribute &a1, const Attribute &a2);
    bool lessThanOrEqual(double lhs, double rhs);
};

#endif //EDAMI_GRANULECALCULATOR_H
