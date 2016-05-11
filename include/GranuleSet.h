#ifndef EDAMI_GRANULESET_H
#define EDAMI_GRANULESET_H

#include "SimpleGranuleSet.h"
#include <map>

typedef std::shared_ptr<SimpleGranuleSet> SimpleGranuleSetPtr;
typedef std::map<std::string, SimpleGranuleSetPtr> ClassGranuleSets;

class GranuleSet {
public:
    void addClass(const std::string& className, SimpleGranuleSetPtr simpleSet);
    const ClassGranuleSets& getClassGranuleSets() const;

private:
    ClassGranuleSets classGranuleSets_;
};

#endif //EDAMI_GRANULESET_H
