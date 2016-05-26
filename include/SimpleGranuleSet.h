#ifndef EDAMI_SIMPLEGRANULESET_H
#define EDAMI_SIMPLEGRANULESET_H

#include <memory>
#include <list>
#include <map>

typedef std::list<int> GranuleMembers;  // contains indexes of rows that are members of a granule
typedef std::shared_ptr<GranuleMembers> GranuleMembersPtr;
typedef std::map<int, GranuleMembersPtr> Granules; // key: idx of some row X, value: indexes of rows that are in X's granule

class SimpleGranuleSet {
public:
    SimpleGranuleSet() : minGranuleSize_(NO_VALUE), maxGranuleSize_(NO_VALUE) {}
    void addGranule(int rowIdx, GranuleMembersPtr members);
    const Granules& getGranules() const;

    Granules::size_type getMinGranuleSize() const;
    Granules::size_type getMaxGranuleSize() const;
    double getAverageGranuleSize() const;

    Granules::size_type size() const;

private:
    static const Granules::size_type NO_VALUE = -1;

    Granules::size_type minGranuleSize_;
    Granules::size_type maxGranuleSize_;
    Granules granules_;
};

#endif //EDAMI_SIMPLEGRANULESET_H
