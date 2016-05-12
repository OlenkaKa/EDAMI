#ifndef EDAMI_NORMALIZATIONPARAMS_H
#define EDAMI_NORMALIZATIONPARAMS_H

#include "SimpleDataset.h"
#include <map>

class NormalizationParams {
public:
    class Values {
    public:
        Values();
        Values(double minValue, double maxValue);
        void setValue(double value);
        double getMinValue() const;
        double getMaxValue() const;
    private:
        double minValue_;
        double maxValue_;
    };

    void setValues(const RowPtr& row);
    Values getValues(int col) const;

private:
    std::map<int, Values> values_;
};

#endif /* EDAMI_NORMALIZATIONPARAMS_H */

