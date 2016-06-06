#ifndef EDAMI_CLASSIFICATIONINFO_H
#define EDAMI_CLASSIFICATIONINFO_H

#include "Classifier.h"
#include "ConfusionMatrix.h"

#include <string>
#include <list>
#include <map>

class ClassificationInfo {
public:
    ClassificationInfo();
    void addResults(const std::string& actualClass, const std::list<std::string>& classifiedClassed);
    void addTime(double secs);
    void setClassifierSize(const Classifier& classifier);
    double getAccuracy() const;
    double getTime() const;
    const ConfusionMatrix& getConfusionMatrix() const;
    const std::map<std::string, int>& getClassifierSize() const;

private:
    int correct_;
    int total_;
    double time_;
    ConfusionMatrix matrix_;
    std::map<std::string, int> classifierSize_;
};


#endif // EDAMI_CLASSIFICATIONINFO_H
