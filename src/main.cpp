#include "Dataset.h"
#include "CrossValidationData.h"
#include "MinimalSizeStrategy.h"
#include "SimpleCrossValidation.h"
#include "LeaveOneOutValidation.h"
#include "GranuleCalculator.h"
#include <iostream>
#include <algorithm>

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

        GranuleCalculator calculator;
        double radius = double(trainSet.numberOfColumns() - 1) / trainSet.numberOfColumns();
        GranuleCalculator::Params params(0.1f, radius);
        GranuleSet *granuleSet = calculator.calculateGranules(trainSet, params);

        cout << endl;
        cout << (*granuleSet);

        CoveringFindingStrategy *strategy = new MinimalSizeStrategy();
        cout << endl << "Selecting granules for covering..." << endl;
        GranuleSet *covering = strategy->selectGranules(trainSet, *granuleSet);

        cout << (*covering);

        delete granuleSet;
        delete strategy;
        delete covering;
    }
}

