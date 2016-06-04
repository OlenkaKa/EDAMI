#ifndef EDAMI_CLASSIFICATIONQUALITY_H
#define EDAMI_CLASSIFICATIONQUALITY_H

#include <string>
#include <list>

class ClassificationQuality {
public:
    ClassificationQuality();
    void add(const std::string& actualClass, const std::list<std::string>& classifiedClassed);
    double getAccuracy() const;

private:
    int correct_;
    int total_;
};


#endif //EDAMI_CLASSIFICATIONQUALITY_H
