#include "GranularReflectionCreator.h"

using namespace std;

Dataset *GranularReflectionCreator::createGranularReflection(const Dataset &originalData, const GranuleSet &granuleCovering) {
    Dataset *reflectionData = new Dataset();
    for (auto classEntry : granuleCovering.getClassGranuleSets()) {
        SimpleGranuleSetPtr simpleGranuleSet = classEntry.second;
        const SimpleDataset &originalSimpleDataset = originalData.getClassDatasets().at(classEntry.first);
        SimpleDataset newSimpleDataset;
        createNewSimpleDataSet(originalSimpleDataset, simpleGranuleSet, newSimpleDataset);
        reflectionData->addClass(classEntry.first, newSimpleDataset);
    }
    return reflectionData;
}

void GranularReflectionCreator::createNewSimpleDataSet(const SimpleDataset& originalSimpleDataset, SimpleGranuleSetPtr simpleGranuleSet, SimpleDataset& newSimpleDataset) {
    for (auto granuleEntry : simpleGranuleSet->getGranules()) {
        vector<RowPtr> granuleRows;
        for (int idx : (*granuleEntry.second)) {
            granuleRows.push_back(originalSimpleDataset.getRows().at(idx));
        }
        newSimpleDataset.addRow(createNewRow(granuleRows));
    }
}

RowPtr GranularReflectionCreator::createNewRow(const vector<RowPtr>& granuleRows) {
    const unsigned long numberOfAttributes = granuleRows.at(0)->size();
    RowPtr newRow(new Row());
    for (Row::size_type attrIdx = 0; attrIdx < numberOfAttributes; ++attrIdx) {
        Attribute::AttributeType type = granuleRows.at(0)->at(attrIdx).getType();
        if (type == Attribute::AttributeType::NOMINAL) {
            newRow->push_back(mostFrequentNominalAttribute(granuleRows, attrIdx));
        } else {
            newRow->push_back(averageNumericalAttribute(granuleRows, attrIdx));
        }
    }
    return newRow;
}

Attribute GranularReflectionCreator::averageNumericalAttribute(const vector<RowPtr> &granuleRows, unsigned long attrIdx) {
    double sum = 0;
    for (RowPtr r : granuleRows) {
        sum += get<double>(r->at(attrIdx).getValue());
    }
    double average = sum / granuleRows.size();
    return Attribute(average);
}

Attribute GranularReflectionCreator::mostFrequentNominalAttribute(const vector<RowPtr> &granuleRows, unsigned long attrIdx) {
    Occurrences occurrences;
    countOccurrences(granuleRows, attrIdx, occurrences);
    vector<string> mostFrequentValues;
    findMostFrequentValues(occurrences, mostFrequentValues);
    return createNewNominalAttribute(mostFrequentValues);
}

void GranularReflectionCreator::countOccurrences(const Rows &rows, Row::size_type attrIdx, Occurrences &occurrences) {
    for (RowPtr r : rows) {
        string attr = get<string>(r->at(attrIdx).getValue());
        if (occurrences.find(attr) == occurrences.end()) {
            occurrences[attr] = 1;
        } else {
            occurrences[attr] += 1;
        }
    }
}

void GranularReflectionCreator::findMostFrequentValues(const Occurrences &occurrences, vector<string> &mostFrequentValues) {
    Rows::size_type mostOccurrences = 0;
    for (auto entry : occurrences) {
        if (entry.second == mostOccurrences) {
            mostFrequentValues.push_back(entry.first);
        } else if (entry.second > mostOccurrences) {
            mostOccurrences = entry.second;
            mostFrequentValues.clear();
            mostFrequentValues.push_back(entry.first);
        }
    }
}

Attribute GranularReflectionCreator::createNewNominalAttribute(const vector<string> &mostFrequentValues) {
    string newValue;
    if (mostFrequentValues.size() > 1) {
        newValue = mostFrequentValues.at(rand() % mostFrequentValues.size());
    } else {
        newValue = mostFrequentValues.at(0);
    }
    return Attribute(newValue);
}