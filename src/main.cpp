#include "Dataset.h"
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

int main(int argc, char** argv) {
//    string file = "/mnt/D/New/Semestr 10/EDAMI/project/datasets/wine.data";
    string file = "people.txt";
    cout << file << endl;

    string simpleAttr;
    double numericalAttr;

//    Dataset dataset(file, 0);
    Dataset dataset(file, 4); // people.txt
    for (auto &it: dataset.getClassDatasets()) {
        cout << "-- class: " << it.first << ", rows:" << endl;
        SimpleDataset classDataset = it.second;
        for (auto &row: classDataset.getRows()) {
            for (auto &attr: row) {
                switch (attr.getType()) {
                    case Attribute::SIMPLE:
                        simpleAttr = get<string>(attr.getValue());
                        cout << simpleAttr;
                        break;
                    case Attribute::NUMERICAL:
                        numericalAttr = get<double>(attr.getValue());
                        cout << "!" << numericalAttr;
                        break;
                    default:
                        break;
                }
                cout << ' ';
            }
            cout << endl;
        }
    }

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

