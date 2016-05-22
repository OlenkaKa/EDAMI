#ifndef EDAMI_SIMPLEDATASET_H
#define EDAMI_SIMPLEDATASET_H

#include "Attribute.h"
#include "NormalizationParams.h"
#include <memory>
#include <string>
#include <vector>

typedef std::vector<Attribute> Row;
typedef std::shared_ptr<Row> RowPtr;
typedef std::vector<RowPtr> Rows;

class SimpleDataset {
public:
    SimpleDataset();
    void addRow(const RowPtr& row);
    const Rows& getRows() const;
    size_t size() const;
    size_t numberOfColumns() const;
    void normalize(const NormalizationParams& params);

private:
    Rows rows_;
};

void setNormalizationParams(const RowPtr& row, NormalizationParams& params);
std::ostream& operator<<(std::ostream& os, const SimpleDataset& dataset);

#endif /* EDAMI_SIMPLEDATASET_H */

