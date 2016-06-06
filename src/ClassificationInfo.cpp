#include "ClassificationInfo.h"
#include "Dataset.h"
#include <algorithm>

using namespace std;

ClassificationInfo::ClassificationInfo() : total_(0), correct_(0), time_(0.0) { }

void ClassificationInfo::addResults(const string& actualClass, const list<string>& classifiedClassed) {
    correct_ += count_if(classifiedClassed.begin(), classifiedClassed.end(),
                         [&actualClass](const string& cls){ return actualClass == cls; });
    total_ += classifiedClassed.size();
}

void ClassificationInfo::addTime(double secs) {
    time_ += secs;
}

void ClassificationInfo::setClassifierSize(const Classifier& classifier) {
    for (auto &classDataset: classifier.getDataset().getClassDatasets()) {
        classifierSize_[classDataset.first] = classDataset.second.size();
    }
}

double ClassificationInfo::getAccuracy() const {
    return (total_ == 0) ? 0 : double(correct_) / total_;
}

double ClassificationInfo::getTime() const {
    return time_;
}

const map<std::string, int>& ClassificationInfo::getClassifierSize() const {
    return classifierSize_;
}

