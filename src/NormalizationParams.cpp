#include "NormalizationParams.h"
#include <limits>

NormalizationParams::Values::Values():
        minValue_(std::numeric_limits<double>::max()),
        maxValue_(std::numeric_limits<double>::min()) {
}

NormalizationParams::Values::Values(double minValue, double maxValue):
        minValue_(minValue), maxValue_(maxValue) {
}

void NormalizationParams::Values::setValue(double value) {
    if (value < minValue_) {
        minValue_ = value;
    } else if (value > maxValue_) {
        maxValue_ = value;
    }
}

double NormalizationParams::Values::getMinValue() const {
    return minValue_;
}

double NormalizationParams::Values::getMaxValue() const {
    return maxValue_;
}

void NormalizationParams::setValue(int col, double value) {
    if (values_.find(col) == values_.end()) {
        values_[col] = NormalizationParams::Values(value, value);
    } else {
        values_[col].setValue(value);
    }
}

NormalizationParams::Values NormalizationParams::getValues(int col) const {
    return values_.at(col);
}

