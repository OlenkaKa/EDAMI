#ifndef EDAMI_SIMPLEDATASET_H
#define EDAMI_SIMPLEDATASET_H

#include "Attribute.h"
#include "NormalizationParams.h"
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
    void normalize(const NormalizationParams& params);

private:
    std::list<RowPtr> rows_;
};

void setNormalizationParams(const RowPtr& row, NormalizationParams& params);
std::ostream& operator<<(std::ostream& os, const SimpleDataset& dataset);

#endif /* EDAMI_SIMPLEDATASET_H */

