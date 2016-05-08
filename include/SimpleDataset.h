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
    void addRow(const Row& row);
    std::list<Row> getRows() const;
    size_t size() const;
private:
    std::list<Row> rows_;
};

#endif /* SIMPLE_DATASET_H */

