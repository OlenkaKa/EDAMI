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
#include <numeric>
#include <ctime>
#include <FileWriter.h>
#include <NumberStats.h>


using namespace std;
using namespace boost;
namespace po = boost::program_options;

const int LINE_WIDTH = 70;
const string NO_FILE = "---";

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
    bool disableLeaveOneOut = false;
    string outputFile = NO_FILE;
} userParams;

FileWriter *fileWriter = nullptr;

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
void addColumnNamesToFile();

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
            ("minkowski,m", po::value<int>(&userParams.minkowski), "minkowski parameter used in calculating distance")
            ("verbose,v", "print more output")
            ("disable-leave-one-out,d", "disable Leave-one-out validation")
            ("output-file,o", po::value<string>(&userParams.outputFile), "name of file to which results will be exported in csv format");

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
        if (vm.count("disable-leave-one-out")) {
            userParams.disableLeaveOneOut = true;
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

    if(userParams.outputFile != NO_FILE) {
        fileWriter = new FileWriter(userParams.outputFile);
        addColumnNamesToFile();
    }

    // performing main task
    cout << endl << endl
         << string(LINE_WIDTH, '=') << endl
         << "     EDAMI project"     << endl
         << " * file name: "                << userParams.fileName << endl
         << " * radius: "                   << userParams.radius << endl
         << " * epsilon: "                  << userParams.epsilon << endl
         << " * knn version: "              << userParams.knn << endl
         << " * minkowski parameter: "      << userParams.minkowski << endl
         << string(LINE_WIDTH, '=') << endl << endl;

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
    if (!userParams.disableLeaveOneOut) {
        data = new LeaveOneOutValidation(dataset);
        calculate(data);
        delete data;
    }

    if(fileWriter != nullptr) {
        fileWriter->writeToFile();
        delete fileWriter;
    }
    return 0;
}

void addColumnNamesToFile() {
    fileWriter->addString("Dataset")
                ->addString("Radius")->addString("Epsilon")
                ->addString("kNN")->addString("Minkowski")
                ->addString("k (from k-fold CV)")->addString("Granule selection strategy")
                ->addString("Classification time [s]")->addString("Accuracy")
                ->addString("Total number of granules in classifier")
                ->addString("Min num. of granules per class")
                ->addString("Max num. of granules per class")
                ->addString("Avg num. of granules per class")
                ->addString("Std. deviation from numbers of granules per class")
                ->nextLine();
}

void calculate(CrossValidationData* data) {
    int numberOfPairs = data->numberOfPairs();

    cout << endl << endl
         << string(LINE_WIDTH, '=') << endl
         << "   Cross-validation: k = "     << numberOfPairs << endl
         << string(LINE_WIDTH, '=') << endl << endl;

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
                 << string(LINE_WIDTH, '-') << endl
                 << "Iteration " << i+1 << " from " << numberOfPairs << endl
                 << string(LINE_WIDTH, '-') << endl;
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
        cout << string(LINE_WIDTH, '-') << endl
             << "  Strategy: " << entry.first << endl
             << string(LINE_WIDTH, '-') << endl;
        
        cout << " * Time of classification: " << entry.second.getTime() << " [s]" << endl;
        cout << " * Accuracy: " << entry.second.getAccuracy() << endl;
        cout << " * Confusion matrix: " << endl << entry.second.getConfusionMatrix();

        cout << " * Number of granules in classifier per class: " << endl;
        if(userParams.verbose) {
            for (auto &size: entry.second.getClassifierSize()) {
                cout << "    -- class " << size.first << ": " << size.second << endl;
            }
            cout << endl;
        }
        NumberStats stats;
        for (auto &size: entry.second.getClassifierSize()) {
            stats.include(size.second);
        }
        cout << "    -- Min number: " << stats.getMinValue() << endl;
        cout << "    -- Max number: " << stats.getMaxValue() << endl;
        cout << "    -- Average number: " << stats.getAverageValue() << endl;
        cout << "    -- Total: " << stats.getSum() << endl;
        cout << "    -- Standard deviation: " << stats.getStandardDeviation() << endl << endl;

        if(fileWriter != nullptr) {
            fileWriter->addString(userParams.fileName)
                    ->addDouble(userParams.radius)->addDouble(userParams.epsilon)
                    ->addString(userParams.knn)->addInt(userParams.minkowski)
                    ->addInt(numberOfPairs)->addString(entry.first)
                    ->addDouble(entry.second.getTime())->addDouble(entry.second.getAccuracy())
                    ->addInt(stats.getSum())
                    ->addInt(stats.getMinValue())
                    ->addInt(stats.getMaxValue())
                    ->addDouble(stats.getAverageValue())
                    ->addDouble(stats.getStandardDeviation())
                    ->nextLine();
        }
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

