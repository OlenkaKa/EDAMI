#include "Dataset.h"
#include <iostream>

using namespace std;
using namespace boost;


int main(int argc, char** argv) {
    string file = "/mnt/D/New/Semestr 10/EDAMI/project/datasets/wine.data";
    cout << file << endl;
    
    string simpleAttr;
    double numericalAttr;
    
    Dataset dataset(file, 0);
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
    return 0;
}

