#include "ClassificationQuality.h"
#include <algorithm>

using namespace std;

ClassificationQuality::ClassificationQuality() : total_(0), correct_(0) { }

void ClassificationQuality::add(const string& actualClass, const list<string>& classifiedClassed) {
    correct_ += count_if(classifiedClassed.begin(), classifiedClassed.end(),
                         [&actualClass](const string& cls){ return actualClass == cls; });
    total_ += classifiedClassed.size();
}

double ClassificationQuality::getAccuracy() const {
    return (total_ == 0) ? 0 : double(correct_) / total_;
}



