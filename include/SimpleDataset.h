#ifndef SIMPLE_DATASET_H
#define	SIMPLE_DATASET_H

#include "Attribute.h"
#include <list>
#include <memory>
#include <string>
#include <vector>

typedef std::vector<Attribute> Row;
typedef std::shared_ptr<Row> RowPtr;

class SimpleDataset {
public:
    SimpleDataset();
    void addRow(const RowPtr& row);
    const std::list<RowPtr>& getRows() const;
    size_t size() const;
    size_t numberOfColumns() const;

private:
    std::list<RowPtr> rows_;
};

std::ostream& operator<<(std::ostream& os, const SimpleDataset& dataset);

#endif /* SIMPLE_DATASET_H */

