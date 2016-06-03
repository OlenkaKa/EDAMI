#include "DatasetInfoProvider.h"

using namespace std;

DatasetInfoProvider::DatasetInfoProvider() {
    // Teaching Assistant Evaluation Data Set
    DatasetInfoPtr tae(new DatasetInfo(5));
    tae->setColumnType(0, Attribute::AttributeType::NOMINAL);
    tae->setColumnType(1, Attribute::AttributeType::NOMINAL);
    tae->setColumnType(2, Attribute::AttributeType::NOMINAL);
    tae->setColumnType(3, Attribute::AttributeType::NOMINAL);
    tae->setColumnType(4, Attribute::AttributeType::NUMERICAL);
    infoSets_["tae"] = tae;
}

DatasetInfoPtr DatasetInfoProvider::getDatasetInfoForFile(const std::string &fileName) {
    for(auto entry : infoSets_) {
        if(fileName.find(entry.first) != string::npos) {
            return entry.second;
        }
    }
    return std::shared_ptr<DatasetInfo>();
}



