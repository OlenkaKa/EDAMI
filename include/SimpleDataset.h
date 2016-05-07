#ifndef SIMPLE_DATASET_H
#define	SIMPLE_DATASET_H

#include "Attribute.h"
#include <list>
#include <string>
#include <vector>

typedef std::vector<Attribute> Row;

class SimpleDataset {
public:
    SimpleDataset();
    void addRow(Row);
    std::list<Row> getRows();
private:
    std::list<Row> rows_;
};

#endif /* SIMPLE_DATASET_H */

