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
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <MinimalSizeStrategy.h>
#include <MaximalSizeStrategy.h>
#include <ClassificationQuality.h>
#include <memory>

using namespace std;
using namespace boost;
namespace po = boost::program_options;

/**
 * Program parameters given by user
 */
static struct {
    string fileName;
    int classCol = 5;
    double radius = 0.5;
    double epsilon = 0.1;
    string knn = "3+";
    int minkowski = 2;
    bool verbose = false;
} userParams;

void verifyKnn(const string& str) {
    const regex reg("[0-9]+\\+?");
    if (!regex_match(str, reg))
        throw regex_error("Invalid k nearest neighbours format.");
}

void checkIfFileExists(const string& fileName) {
    ifstream f(fileName.c_str());
    if (!f.good()) {
        throw invalid_argument("File: " + fileName + " does not exist.");
    }
}

/**
 * Create and verify classifiers with given parameters
 */
void calculate(CrossValidationData *data);
GranuleSetPtr calculateGranules(const Dataset &trainSet);
typedef std::shared_ptr<CoveringFindingStrategy> CoveringStrategyPtr;
GranuleSetPtr selectGranulesForCovering(GranuleSetPtr granuleSetPtr, CoveringStrategyPtr strategyPtr);
DatasetPtr createGranularReflection(const Dataset &trainSet, GranuleSetPtr covering);

int main(int argc, char **argv) {
    // getting parameters
    po::options_description description("Usage");
    description.add_options()
            ("help,h", "print help message")
            ("file,f", po::value<string>(&userParams.fileName)->required()->notifier(&checkIfFileExists), "name of file with dataset (required)")
            ("class,c", po::value<int>(&userParams.classCol), "class column index in dataset")
            ("radius,r", po::value<double>(&userParams.radius), "radius parameter used in granular computing")
            ("eps,e", po::value<double>(&userParams.epsilon), "epsilon parameter for numerical attributes in granular computing")
            ("knn,k", po::value<string>(&userParams.knn)->notifier(&verifyKnn), "k nearest neighbourhood parameter in format \"<num>\" or \"<num>+\"")
            ("mink,m", po::value<int>(&userParams.minkowski), "minkowski parameter used in calculating distance")
            ("verbose,v", "print more output");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, description), vm);
        if (vm.count("help")) {
            cout << description << endl;
            return 0;
        }
        if (vm.count("verbose")) {
            userParams.verbose = true;
        }
        po::notify(vm);
    } catch (const std::exception& e) {
        cout << e.what() << endl;
        cout << description << endl;
        return 1;
    } catch (...) {
        cout << "Invalid arguments!" << endl;
        cout << description << endl;
        return 2;
    }

    // performing main task
    cout << "File name: "
         << userParams.fileName << endl << endl;
    Dataset dataset(userParams.fileName, userParams.classCol);
    if(userParams.verbose) {
        cout << "Dataset:" << endl
             << dataset << endl << endl;
    }

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

    list<CoveringStrategyPtr> strategies = { CoveringStrategyPtr(new MinimalSizeStrategy()),
                                             CoveringStrategyPtr(new AverageSizeStrategy()),
                                             CoveringStrategyPtr(new MaximalSizeStrategy()) };
    map<string, ClassificationQuality> qualityMap;
    for(CoveringStrategyPtr strPtr : strategies) {
        qualityMap[strPtr->getName()] = ClassificationQuality();
    }
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

        if(userParams.verbose) {
            cout << "Selected train set:" << endl;
            cout << trainSet << endl;
        }

        GranuleSetPtr granuleSetPtr = calculateGranules(trainSet);
        for (CoveringStrategyPtr strategyPtr : strategies) {
            cout << strategyPtr->getName() << endl;
            GranuleSetPtr covering = selectGranulesForCovering(granuleSetPtr, strategyPtr);
            DatasetPtr reflection = createGranularReflection(trainSet, covering);

            cout << "CLASSIFICATION:" << endl;
            Classifier classifier((*reflection), userParams.knn, userParams.minkowski);
            for (auto &classDataset: testSet.getClassDatasets()) {
                list<string> result;
                classifier.classify(classDataset.second, result);
                cout << "-- class: " << classDataset.first << endl;
                if(userParams.verbose) {
                    for (auto &cls: result)
                        cout << cls << endl;
                }
                qualityMap.at(strategyPtr->getName()).add(classDataset.first, result);
            }
            cout << endl;
        }
    }
    for(auto entry : qualityMap) {
        cout << "Strategy: " << entry.first << " Accuracy: " << entry.second.getAccuracy() << endl;
    }
}

GranuleSetPtr calculateGranules(const Dataset &trainSet) {
    cout << "Calculating all granules..." << endl;
    GranuleCalculator calculator;
    GranuleCalculator::Params params(userParams.epsilon, userParams.radius);
    GranuleSetPtr granuleSetPtr = calculator.calculateGranules(trainSet, params);
    if(userParams.verbose) {
        cout << (*granuleSetPtr) << endl;
    }
    return granuleSetPtr;
}

GranuleSetPtr selectGranulesForCovering(GranuleSetPtr granuleSetPtr, CoveringStrategyPtr strategyPtr) {
    cout << "Selecting granules for covering..." << endl;
    GranuleSetPtr covering = strategyPtr->selectGranules(*granuleSetPtr);
    if(userParams.verbose) {
        cout << (*covering) << endl;
    }
    return covering;
}

DatasetPtr createGranularReflection(const Dataset &trainSet, GranuleSetPtr covering) {
    cout << "Creating granular reflection..." << endl;
    GranularReflectionCreator creator;
    DatasetPtr reflection = creator.createGranularReflection(trainSet, *covering);
    if(userParams.verbose) {
        cout << (*reflection) << endl;
    }
    return reflection;
}