#include "Dataset.h"
#include "CrossValidation.h"
#include "MinimalSizeStrategy.h"
#include <iostream>
#include <GranuleCalculator.h>
#include <algorithm>

using namespace std;
using namespace boost;

void print(const GranuleSet& granuleSet) {
    cout << "[ GranuleSet ]" << endl;
    for(auto &classEntry : granuleSet.getClassGranuleSets()) {
        cout << "--- class: " << classEntry.first << endl;
        for(auto &granuleEntry : classEntry.second->getGranules()) {
            int rowIdx = granuleEntry.first;
            GranuleMembersPtr members = granuleEntry.second;
            string memberIndexes = "";
            for(auto &idx : (*members)) {
                memberIndexes += to_string(idx) + ",";
            }
            memberIndexes.pop_back();
            cout << "Row " << rowIdx << ", granule (" << memberIndexes << ")" << endl;
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
                case Attribute::NOMINAL:
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
