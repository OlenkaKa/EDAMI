#include "Dataset.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <DatasetInfoProvider.h>

using namespace std;
using namespace boost;

static const char SEPARATOR = ',';

Dataset::Dataset() {
}

Dataset::Dataset(const string& fileName, int classCol) {
    ifstream file;
    string line;
    file.open(fileName);

    DatasetInfoProvider provider;
    DatasetInfoPtr datasetInfo = provider.getDatasetInfoForFile(fileName);
    
    while (std::getline(file, line)) {
        if(line.empty()) {  // skip empty lines that sometimes occur at the end of file
            continue;
        }
        RowPtr row(new Row());
        stringstream lineStream(line);
        string value, cls;
        
        // values in row before class attribute
        for (int i = 0; i < classCol; ++i) {
            if (!getline(lineStream, value, SEPARATOR))
                throw invalid_argument("Cannot read columns before class column.");
            if(datasetInfo) {
                row->push_back(Attribute::createAttribute(value, datasetInfo->getColumnType(i)));
            } else {
                row->push_back(Attribute::createAttribute(value));
            }
        }
        
        // get class
        if (!getline(lineStream, cls, SEPARATOR))
            throw invalid_argument("Cannot read class column.");
        if (classDatasets_.find(cls) == classDatasets_.end()) {
            classDatasets_[cls] = SimpleDataset();
        }
        
        // remaining values from row
        int idx = classCol + 1;
        while (getline(lineStream, value, SEPARATOR)) {
            if(datasetInfo) {
                row->push_back(Attribute::createAttribute(value, datasetInfo->getColumnType(idx)));
            } else {
                row->push_back(Attribute::createAttribute(value));
            }
            ++idx;
        }
        
        classDatasets_[cls].addRow(row);
    }

    file.close();
}

void Dataset::addClass(const std::string& cls, const SimpleDataset& dataset) {
    classDatasets_[cls] = dataset;
}

void Dataset::addRow(const std::string& cls, const RowPtr& row) {
    classDatasets_[cls].addRow(row);
}

const std::map<std::string, SimpleDataset>& Dataset::getClassDatasets() const {
    return classDatasets_;
}

size_t Dataset::numberOfColumns() const {
    return classDatasets_.empty() ? 0 : (*classDatasets_.begin()).second.numberOfColumns();
}

void Dataset::normalize(const NormalizationParams& params) {
    for (auto &classDataset: classDatasets_) {
        classDataset.second.normalize(params);
    }
}

ostream& operator<<(ostream& os, const Dataset& dataset) {
    for (auto &it: dataset.getClassDatasets()) {
        os << "-- class: " << it.first << ", rows:" << endl;
        os << it.second;
    }
    return os;
}

