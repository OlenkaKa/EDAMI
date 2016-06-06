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
#include "ClassificationInfo.h"
#include <memory>
#include <ctime>


using namespace std;
using namespace boost;
namespace po = boost::program_options;

/**
 * Program parameters given by user
 */
static struct {
    string fileName;
    int classCol = 5;
    char separator = ',';
    double radius = 0.5;
    double epsilon = 0.1;
    string knn = "5+";
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
void testClassifier(const Classifier& classifier, const Dataset& testSet, ClassificationInfo& info);

int main(int argc, char **argv) {
    // getting parameters
    po::options_description description("Usage");
    description.add_options()
            ("help,h", "print help message")
            ("file,f", po::value<string>(&userParams.fileName)->required()->notifier(&checkIfFileExists), "name of file with dataset (required)")
            ("class,c", po::value<int>(&userParams.classCol), "class column index in dataset")
            ("separator,s", po::value<char>(&userParams.separator), "attribute separator in dataset file")
            ("radius,r", po::value<double>(&userParams.radius), "radius parameter used in granular computing")
            ("epsilon,e", po::value<double>(&userParams.epsilon), "epsilon parameter for numerical attributes in granular computing")
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
    cout << endl << endl
         << "=============================" << endl
         << "     Creating classifiers"     << endl
         << " * file name: "                << userParams.fileName << endl
         << " * radius: "                   << userParams.radius << endl
         << " * epsilon: "                  << userParams.epsilon << endl
         << " * knn version: "              << userParams.knn << endl
         << " * minkowski parameter: "      << userParams.minkowski << endl
         << "=============================" << endl << endl;

    Dataset dataset(userParams.fileName, userParams.classCol, userParams.separator);
    if(userParams.verbose) {
        cout << "Dataset:" << endl
             << dataset << endl << endl;
    }

    int kCrossValidation[] = {5, 10};
    CrossValidationData* data;
    for (int k: kCrossValidation) {
        data = new SimpleCrossValidation(dataset, k);
        calculate(data);
        delete data;
    }
    data = new LeaveOneOutValidation(dataset);
    calculate(data);
    delete data;

    return 0;
}

void calculate(CrossValidationData* data) {
    int numberOfPairs = data->numberOfPairs();

    cout << endl << endl
         << "=============================" << endl
         << "   Cross-validation: k = "     << numberOfPairs << endl
         << "=============================" << endl << endl;

    list<CoveringStrategyPtr> strategies = { CoveringStrategyPtr(new MinimalSizeStrategy()),
                                             CoveringStrategyPtr(new AverageSizeStrategy()),
                                             CoveringStrategyPtr(new MaximalSizeStrategy()) };
    
    // creating classification info map
    map<string, ClassificationInfo> infoMap;
    
    const string NO_STRATEGY = "Original dataset";
    infoMap[NO_STRATEGY] = ClassificationInfo();
    
    for(CoveringStrategyPtr strPtr : strategies) {
        infoMap[strPtr->getName()] = ClassificationInfo();
    }

    
    // creating classifiers and test for each cross-validation data
    for (int i = 0; i < numberOfPairs; ++i) {
        if (userParams.verbose) {
            cout << endl
                 << "-----------------------------" << endl
                 << "Iteration " << i+1 << " from " << numberOfPairs << endl
                 << "-----------------------------" << endl;
        }

        // preparing train and test sets
        Dataset trainSet, testSet;
        NormalizationParams normalization;
        data->getData(i, trainSet, testSet, normalization);

        trainSet.normalize(normalization);
        testSet.normalize(normalization);

        if(userParams.verbose) {
            cout << "Selected train set:" << endl;
            cout << trainSet << endl;
        }

        // create and test classifiers for granule strategies
        GranuleSetPtr granuleSetPtr = calculateGranules(trainSet);
        for (CoveringStrategyPtr strategyPtr : strategies) {
            GranuleSetPtr covering = selectGranulesForCovering(granuleSetPtr, strategyPtr);
            DatasetPtr reflection = createGranularReflection(trainSet, covering);

            Classifier classifier((*reflection), userParams.knn, userParams.minkowski);
            testClassifier(classifier, testSet, infoMap.at(strategyPtr->getName()));
        }
        
        // create and test classifier for original train dataset
        Classifier classifier(trainSet, userParams.knn, userParams.minkowski);
        testClassifier(classifier, testSet, infoMap.at(NO_STRATEGY));
    }
    
    // show results
    for(auto entry : infoMap) {
        cout << "Strategy: " << entry.first << endl;
        cout << " * Accuracy: " << entry.second.getAccuracy() << endl;
        cout << " * Time of classification: " << entry.second.getTime() << endl;

        cout << " * Last classifier size: " << endl;
        for (auto &size: entry.second.getClassifierSize()) {
            cout << "    -- class " << size.first << ": " << size.second << endl;
        }
        cout << "    -- TOTAL: "
             << accumulate(std::begin(entry.second.getClassifierSize()), std::end(entry.second.getClassifierSize()),
                0, [](const int sum, const pair<string, int>& elem) { return sum + elem.second; }) << endl;
        
        cout << endl;
    }
}

GranuleSetPtr calculateGranules(const Dataset &trainSet) {
    if (userParams.verbose) {
        cout << "Calculating all granules..." << endl;
    }
    GranuleCalculator calculator;
    GranuleCalculator::Params params(userParams.epsilon, userParams.radius);
    GranuleSetPtr granuleSetPtr = calculator.calculateGranules(trainSet, params);
    if(userParams.verbose) {
        cout << (*granuleSetPtr) << endl;
    }
    return granuleSetPtr;
}

GranuleSetPtr selectGranulesForCovering(GranuleSetPtr granuleSetPtr, CoveringStrategyPtr strategyPtr) {
    if (userParams.verbose) {
        cout << "Selecting granules for covering using " << strategyPtr->getName() << "..." << endl;
    }
    GranuleSetPtr covering = strategyPtr->selectGranules(*granuleSetPtr);
    if(userParams.verbose) {
        cout << (*covering) << endl;
    }
    return covering;
}

DatasetPtr createGranularReflection(const Dataset &trainSet, GranuleSetPtr covering) {
    if (userParams.verbose) {
        cout << "Creating granular reflection..." << endl;
    }
    GranularReflectionCreator creator;
    DatasetPtr reflection = creator.createGranularReflection(trainSet, *covering);
    if(userParams.verbose) {
        cout << (*reflection) << endl;
    }
    return reflection;
}

void testClassifier(const Classifier& classifier, const Dataset& testSet, ClassificationInfo& info) {
    info.setClassifierSize(classifier);
    for (auto &classDataset: testSet.getClassDatasets()) {
        list<string> result;

        clock_t beginTime = clock();
        classifier.classify(classDataset.second, result);
        clock_t endTime = clock();
        info.addTime(double(endTime - beginTime) / CLOCKS_PER_SEC);

        if(userParams.verbose) {
            cout << "-- class: " << classDataset.first << endl;
            for (auto &cls: result)
                cout << cls << endl;
        }
        info.addResults(classDataset.first, result);
    }
}

