#include "Dataset.h"
#include <fstream>
#include <sstream>

using namespace std;
using namespace boost;

Dataset::Dataset() {
}

Dataset::Dataset(const string& fileName, int classCol) {
    ifstream file;
    string line;
    file.open(fileName);

    while (std::getline(file, line)) {
        RowPtr row(new Row());
        stringstream lineStream(line);
        string value, cls;
        
        // values in row before class attribute
        for (int i = 0; i < classCol; ++i) {
            if (!getline(lineStream, value, ','))
                throw exception();
            row->push_back(Attribute::createAttribute(value));
        }
        
        // get class
        if (!getline(lineStream, cls, ','))
            throw exception();
        if (classDatasets_.find(cls) == classDatasets_.end()) {
            classDatasets_[cls] = SimpleDataset();
        }
        
        // remaining values from row
        while (getline(lineStream, value, ',')) {
            row->push_back(Attribute::createAttribute(value));
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
