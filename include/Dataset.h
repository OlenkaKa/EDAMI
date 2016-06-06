#ifndef EDAMI_DATASET_H
#define EDAMI_DATASET_H

#include "SimpleDataset.h"
#include "NormalizationParams.h"
#include <string>
#include <map>

class Dataset {
public:
    Dataset();
    Dataset(const std::string& fileName, int classCol, char separator);
    void addClass(const std::string& cls, const SimpleDataset& dataset);
    void addRow(const std::string& cls, const RowPtr& row);
    const std::map<std::string, SimpleDataset>& getClassDatasets() const;
    size_t numberOfColumns() const;
    void normalize(const NormalizationParams& normalization);

private:
    std::map<std::string, SimpleDataset> classDatasets_;
};

std::ostream& operator<<(std::ostream& os, const Dataset& dataset);

#endif /* EDAMI_DATASET_H */

