#ifndef EDAMI_DATASETINFOPROVIDER_H
#define EDAMI_DATASETINFOPROVIDER_H

#include <map>
#include <string>
#include <memory>
#include "DatasetInfo.h"

typedef std::shared_ptr<DatasetInfo> DatasetInfoPtr;

class DatasetInfoProvider {
public:
    DatasetInfoProvider();
    DatasetInfoPtr getDatasetInfoForFile(const std::string &fileName);

private:
    std::map<std::string, DatasetInfoPtr> infoSets_;
};

#endif //EDAMI_DATASETINFOPROVIDER_H
