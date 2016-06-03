#include "DatasetInfo.h"

DatasetInfo::DatasetInfo(int numberOfColumns) {
    types_.resize(numberOfColumns);
}

void DatasetInfo::setColumnType(int idx, Attribute::AttributeType type) {
    types_[idx] = type;
}

Attribute::AttributeType DatasetInfo::getColumnType(int idx) {
    return types_.at(idx);
}






