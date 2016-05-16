#ifndef DATASET_H
#define DATASET_H

#include "SimpleDataset.h"
#include <string>
#include <map>

class Dataset {
public:
    Dataset();
    Dataset(const std::string& fileName, int classCol);
    void addClass(const std::string& cls, const SimpleDataset& dataset);
    void addRow(const std::string& cls, const RowPtr& row);
    const std::map<std::string, SimpleDataset>& getClassDatasets() const;
    size_t numberOfColumns() const;

private:
    std::map<std::string, SimpleDataset> classDatasets_;
};

std::ostream& operator<<(std::ostream& os, const Dataset& dataset);

#endif /* DATASET_H */

