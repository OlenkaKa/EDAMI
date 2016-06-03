#ifndef EDAMI_DATASETINFO_H
#define EDAMI_DATASETINFO_H

#include <vector>
#include "Attribute.h"

class DatasetInfo {
public:
    DatasetInfo(int numberOfColumns);
    void setColumnType(int idx, Attribute::AttributeType type);
    Attribute::AttributeType getColumnType(int idx);

private:
    std::vector<Attribute::AttributeType> types_;
};

#endif //EDAMI_DATASETINFO_H
