#ifndef EDAMI_GRANULARREFLECTIONCREATOR_H
#define EDAMI_GRANULARREFLECTIONCREATOR_H

#include <vector>
#include <map>
#include "Dataset.h"
#include "SimpleDataset.h"
#include "Attribute.h"
#include "SimpleGranuleSet.h"
#include "GranuleSet.h"
#include <memory>

typedef std::shared_ptr<Dataset> DatasetPtr;

class GranularReflectionCreator {
public:
    DatasetPtr createGranularReflection(const Dataset& originalData, const GranuleSet& granuleCovering);

private:
    typedef std::map<std::string, Rows::size_type> Occurrences;

    void createNewSimpleDataSet(const SimpleDataset& originalSimpleDataset, SimpleGranuleSetPtr simpleGranuleSet, SimpleDataset& newSimpleDataset);
    RowPtr createNewRow(const std::vector<RowPtr>& granuleRows);
    Attribute mostFrequentNominalAttribute(const std::vector<RowPtr> &granuleRows, unsigned long attrIdx);
    Attribute averageNumericalAttribute(const std::vector<RowPtr> &granuleRows, unsigned long attrIdx);
    void countOccurrences(const Rows& rows, Row::size_type attrIdx, Occurrences& occurrences);
    void findMostFrequentValues(const Occurrences& occurrences, std::vector<std::string>& mostFrequentValues);
    Attribute createNewNominalAttribute(const std::vector<std::string> &mostFrequentValues);
};

#endif //EDAMI_GRANULARREFLECTIONCREATOR_H
