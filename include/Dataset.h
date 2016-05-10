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
    const std::map<std::string, SimpleDataset>& getClassDatasets() const;
    size_t numberOfColumns() const;

private:
    std::map<std::string, SimpleDataset> classDatasets_;
};

#endif /* DATASET_H */

