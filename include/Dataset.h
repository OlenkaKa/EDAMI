#ifndef DATASET_H
#define DATASET_H

#include "SimpleDataset.h"
#include <string>
#include <map>

class Dataset {
public:
    Dataset(std::string fileName, int classCol);
    std::map<std::string, SimpleDataset> getClassDatasets();
private:
    std::map<std::string, SimpleDataset> classDatasets_;
};

#endif /* DATASET_H */

