#include "Dataset.h"
#include "CrossValidationData.h"
#include "SimpleCrossValidation.h"
#include "LeaveOneOutValidation.h"
#include "GranuleCalculator.h"
#include "Classifier.h"
#include <iostream>
#include <algorithm>
#include "GranularReflectionCreator.h"
#include "AverageSizeStrategy.h"

using namespace std;
using namespace boost;

void calculate(CrossValidationData* data);

int main(int argc, char** argv) {
    string file = "people.txt";
    cout << file << endl;

    Dataset dataset(file, 4); // people.txt
    cout << dataset;

    int subsetsNum[] = {2, 3};
    CrossValidationData* data;
    for (int k: subsetsNum) {
        data = new SimpleCrossValidation(dataset, k);
        calculate(data);
        delete data;
    }
    data = new LeaveOneOutValidation(dataset);
    calculate(data);
    delete data;

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}

void calculate(CrossValidationData* data) {
    int numberOfPairs = data->numberOfPairs();

    cout << endl << endl
         << "=============================" << endl
         << "     Creating classifiers"     << endl
         << " Parameters:"                  << endl
         << " * cross-validation: k = " << numberOfPairs << endl
         << "=============================" << endl;
    
    for (int i = 0; i < data->numberOfPairs(); ++i) {
        cout << endl
             << "-----------------------------" << endl
             << "Iteration " << i+1 << " from " << numberOfPairs << endl
             << "-----------------------------" << endl;
        
        Dataset trainSet, testSet;
        NormalizationParams normalization;
        data->getData(i, trainSet, testSet, normalization);

        trainSet.normalize(normalization);
        testSet.normalize(normalization);

        cout << endl << "Selected train set:" << endl;
        cout << trainSet;
        
        cout << endl << "Calculating granules..." << endl;
        GranuleCalculator calculator;
        double radius = double(trainSet.numberOfColumns() - 2) / trainSet.numberOfColumns();
        GranuleCalculator::Params params(0.1, radius);
        GranuleSet *granuleSet = calculator.calculateGranules(trainSet, params);
        cout << (*granuleSet);

        cout << endl << "Selecting granules for covering..." << endl;
        CoveringFindingStrategy *strategy = new AverageSizeStrategy();
        GranuleSet *covering = strategy->selectGranules(trainSet, *granuleSet);
        cout << (*covering);

        cout << endl << "Creating granular reflection..." << endl;
        GranularReflectionCreator creator;
        Dataset *reflection = creator.createGranularReflection(trainSet, *covering);
        cout << (*reflection);

        cout << endl << "CLASSIFICATION:" << endl;
        Classifier classifier((*reflection), "3+", 2);
        for (auto &classDataset: testSet.getClassDatasets()) {
            list<string> result;
            classifier.classify(classDataset.second, result);
            cout << "-- class: " << classDataset.first << endl;
            for (auto &cls: result)
                cout << cls << endl;
        }

        delete granuleSet;
        delete strategy;
        delete covering;
        delete reflection;
    }
}

