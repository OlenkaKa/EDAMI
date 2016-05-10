#include "Dataset.h"
#include "CrossValidation.h"
#include "MinimalSizeStrategy.h"
#include <iostream>
#include <GranuleCalculator.h>

using namespace std;
using namespace boost;

void print(const GranuleSet& granuleSet) {
    cout << "[ GranuleSet ]" << endl;
    for(auto &entry : granuleSet) {
        cout << "--- class: " << entry.first << endl;
        const vector<vector<int>> &granules = entry.second;
        for(size_t i = 0; i < granules.size(); ++i) {
            string granule = "";
            for(size_t j = 0; j < granules.at(i).size(); ++j) {
                if(j != 0) {
                    granule += ",";
                }
                granule += to_string(granules.at(i).at(j));
            }
            cout << "Row " << i << ", granule (" << granule << ")" << endl;
        }
    }
}

void print(const SimpleDataset& dataset);
void print(const Dataset& dataset);

int main(int argc, char** argv) {
    string file = "people.txt";
    cout << file << endl;

    string simpleAttr;
    double numericalAttr;

    Dataset dataset(file, 4); // people.txt
    print(dataset);

    GranuleCalculator calculator;
    double radius = double(dataset.numberOfColumns() - 1) / dataset.numberOfColumns();
    GranuleCalculator::Params params(0.1f, radius);
    GranuleSet *granuleSet = calculator.calculateGranules(dataset, params);
    
    cout << endl;
    print(*granuleSet);

    CoveringFindingStrategy *strategy = new MinimalSizeStrategy();
    cout << endl << "Selecting granules for covering..." << endl;
    GranuleSet *covering = strategy->selectGranules(dataset, *granuleSet);

    print(*covering);
    
    delete granuleSet;
    delete strategy;
    delete covering;
    
    return 0;
}


void print(const SimpleDataset& dataset) {
    for (auto &row: dataset.getRows()) {
        for (auto &attr: *row) {
            switch (attr.getType()) {
                case Attribute::SIMPLE:
                    cout << attr.getValue();
                    break;
                case Attribute::NUMERICAL:
                    cout << "!" << attr.getValue();
                    break;
                default:
                    break;
            }
            cout << ' ';
        }
        cout << endl;
    }
}

void print(const Dataset& dataset) {
    for (auto &it: dataset.getClassDatasets()) {
        cout << "-- class: " << it.first << ", rows:" << endl;
        print(it.second);
    }
}
